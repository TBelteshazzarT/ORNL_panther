#!/usr/bin/env python3
import customtkinter
import roslaunch
import rospy
import os
import glob
import subprocess
import shlex
import sys
import signal
import psutil


def home():
    return os.path.expanduser("~")

def list_files(path, pattern='*') -> list:
    options = glob.glob(os.path.join(path, pattern))
    file_names = []
    for option in options:
        file_names.append(os.path.basename(option))
    return file_names

def kill_child_processes(parent_pid, sig=signal.SIGTERM):
    try:
        parent = psutil.Process(parent_pid)
        print(parent)
    except psutil.NoSuchProcess:
        print("parent process not existing")
        return
    children = parent.children(recursive=True)
    print(children)
    for process in children:
        print("try to kill child: " + str(process))
        process.send_signal(sig)

class Roscore(object):
    """
    roscore wrapped into a subprocess.
    Singleton implementation prevents from creating more than one instance.
    """
    __initialized = False
    def __init__(self):
        if Roscore.__initialized:
            raise Exception("You can't create more than 1 instance of Roscore.")
        Roscore.__initialized = True
    def run(self):
        try:
            self.roscore_process = subprocess.Popen(['roscore'])
            self.roscore_pid = self.roscore_process.pid  # pid of the roscore process (which has child processes)
        except OSError as e:
            sys.stderr.write('roscore could not be run')
            raise e
    def terminate(self):
        print("try to kill child pids of roscore pid: " + str(self.roscore_pid))
        kill_child_processes(self.roscore_pid)
        self.roscore_process.terminate()
        self.roscore_process.wait()  # important to prevent from zombie process
        Roscore.__initialized = False

class PantherLaunchUI(customtkinter.CTkToplevel):
    def __init__(self, parent, name='Panther Control Launcher', **kwargs):
        super().__init__(**kwargs)
        self._parent = parent
        self.is_open: bool = True
        self.maps: list = ['']
        self.nav_mode: str = ''
        self.type_name: str = ''
        self.map_name: str = ''
        self.mission: str = ''
        self.roscore = None

        self.wm_title(name)

        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)

        # Widgets
        self.form_dropdown = customtkinter.CTkOptionMenu(self, values=['Simulation','Main'])
        self.navigation_mode_dropdown = customtkinter.CTkOptionMenu(self, values=['SLAM', 'AMCL'], command= lambda *args:self.update_nav_mode())
        self.map_select_info_label = customtkinter.CTkLabel(self, text='Map Selection:')
        self.map_select = customtkinter.CTkOptionMenu(self, values=self.maps)
        self.mission_select = customtkinter.CTkOptionMenu(self, values=['Select Mission'])
        self.start_button = customtkinter.CTkButton(self, text='Launch ROS System', command=self.launch)

        # Grids
        self.form_dropdown.grid(column=0, row=0, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.navigation_mode_dropdown.grid(column=0, row=1, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.map_select_info_label.grid(column=1, row=0, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.map_select.grid(column=1, row=1, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.mission_select.grid(column=0, row=2, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.start_button.grid(column=0, row=3, padx=1, pady=1, sticky=customtkinter.NSEW)

        self.update_map_options()

    def get(self) -> str:
        return self._current_value

    def update_map_options(self):
        options = list_files((str(home())  + "/panther_ws/src/map_server/map"), pattern='*.yaml')
        names = []
        for option in options:
            names.append(option[:-5])
        self.maps = names
        self.map_select.configure(values=self.maps)

    def launch(self):
        type_name = self.form_dropdown.get()
        nav_mode = self.navigation_mode_dropdown.get()
        map_name = self.map_select.get()
        mission = ''
        # load params
        self.param_load(type_name=type_name ,nav_mode=nav_mode, map_name=map_name, mission=mission)
        self.launch_system()
        self.destroy()

    def update_nav_mode(self):
        mode = self.navigation_mode_dropdown.get()
        if mode == 'SLAM':
            self.map_select.configure(state='disabled')
        else:
            self.map_select.configure(state='enabled')

    def param_load(self, type_name:str, nav_mode:str, map_name:str, mission:str):
        self.type_name = type_name
        self.nav_mode = nav_mode
        self.map_name = map_name
        self.mission = mission

    def launch_system(self):
        self.roscore = Roscore()
        self.roscore.run()
        rospy.sleep(2)

        if self.type_name == 'Simulation':
            self.sim_launch(self.nav_mode, self.map_name, self.mission)
        elif self.type_name == 'Main':
            self.main_launch(self.nav_mode, self.map_name, self.mission)


    def shutdown_system(self):
        self.roscore.terminate()

    def sim_launch(self, nav_mode: str, map_name: str, mission: str):
        rospy.init_node('launcher_sim', anonymous=True)
        uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
        roslaunch.configure_logging(uuid)
        launch = roslaunch.parent.ROSLaunchParent(uuid,
                                                  [(str(home()) + "/panther_ws/src/sdr_launch/launch/main_sim.launch")])
        launch.start()
        rospy.loginfo("started sim")
        self.nav_launch(nav_mode, map_name)

    def main_launch(self, nav_mode: str, map_name: str, mission: str):
        rospy.init_node('launcher_main', anonymous=True)
        uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
        roslaunch.configure_logging(uuid)
        launch = roslaunch.parent.ROSLaunchParent(uuid,
                                                  [(str(home()) + "/panther_ws/src/sdr_launch/launch/main.launch")])
        launch.start()
        rospy.loginfo("started main")
        self.nav_launch(nav_mode, map_name)

    def load_map(self, map_name:str):
        arg_file = (str(home()) + '/panther_ws/src/map_server/map/'+ map_name + '.yaml')
        arg = str('"' + arg_file + '"')
        xml_file = (str(home()) + "/panther_ws/src/panther_navigation/launch/amcl.launch")
        data = ('<?xml version="1.0"?>\n\n' +
                '<launch>\n\n'
                '  <arg name="map_file" value=' + arg + ' ></arg>\n' +
                '  <node pkg="map_server" type="map_server" name="map_server_load" args="$(arg map_file)" />\n\n' +
                '  <node name="amcl"\n' +
                '    pkg="amcl"\n' +
                '    type="amcl"\n' +
                '    clear_params="true">\n' +
                '    <rosparam command="load" file="$(find panther_navigation)/config/amcl.yaml" />\n' +
                '  </node>\n\n' +
                '</launch>'
                )
        with open(xml_file, 'w') as f:
            f.write(data)

    def nav_launch(self, nav_mode: str, map_name: str):
        if nav_mode == 'SLAM':
            uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
            roslaunch.configure_logging(uuid)
            launch = roslaunch.parent.ROSLaunchParent(uuid,
                                              [(str(home()) + "/panther_ws/src/sdr_launch/launch/slam.launch")])
            launch.start()
            rospy.loginfo("started slam")

        elif nav_mode == 'AMCL':
            self.load_map(map_name)
            uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
            roslaunch.configure_logging(uuid)
            launch = roslaunch.parent.ROSLaunchParent(uuid, [str((home()) + "/panther_ws/src/sdr_launch/launch/amcl.launch")])
            launch.start()
            rospy.loginfo("started amcl")

        else:
            return

