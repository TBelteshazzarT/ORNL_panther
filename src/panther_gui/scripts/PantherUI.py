#!/usr/bin/env python3
import customtkinter
from PantherLaunchUI import PantherLaunchUI
import roslaunch
import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Float64
from nav_msgs.msg import Odometry

class PantherUI(customtkinter.CTk):
    def __init__(self, name='Panther Control Program', **kwargs):
        super().__init__(**kwargs)
        self.launcher = None
        # Ros subscribers
        rospy.Subscriber('/cmd_vel', Twist, self.callback_cmd)
        rospy.Subscriber('/odom', Odometry, self.callback_odom)
        self.msg_cmd: list = []
        self.odom: list = []


        self.wm_title(name)

        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)

        # Frames
        self.info_panel_frame = customtkinter.CTkFrame(self)
        self.control_panel_frame = customtkinter.CTkFrame(self)

        # Grids
        self.info_panel_frame.grid(column=0, row=0, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.control_panel_frame.grid(column=1, row=0, padx=1, pady=1, sticky=customtkinter.NSEW)

        # Widgets
        self.form_label = customtkinter.CTkLabel(self.info_panel_frame, text='Simulation or actual')
        self.navigation_mode_label = customtkinter.CTkLabel(self.info_panel_frame, text='Nav mode')
        self.control_mode_label = customtkinter.CTkLabel(self.info_panel_frame, text='nav_vel or joy_vel')

        self.motor_speed_label = customtkinter.CTkLabel(self.control_panel_frame, text="Motor Speed:")
        self.motor_speed_value = customtkinter.CTkLabel(self.control_panel_frame, text='')
        self.motor_angular_label = customtkinter.CTkLabel(self.control_panel_frame, text="Motor Angular:")
        self.motor_angular_value = customtkinter.CTkLabel(self.control_panel_frame, text='')
        self.command_speed_label = customtkinter.CTkLabel(self.control_panel_frame, text="Command Speed:")
        self.command_speed_value = customtkinter.CTkLabel(self.control_panel_frame, text='')
        self.command_angular_label = customtkinter.CTkLabel(self.control_panel_frame, text="Command Angular:")
        self.command_angular_value = customtkinter.CTkLabel(self.control_panel_frame, text='')
        self.shutdown_button = customtkinter.CTkButton(self.control_panel_frame, text='Shutdown', command=self.shutdown)

        # Grids
        self.form_label.grid(column=0, row=0, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.navigation_mode_label.grid(column=0, row=1, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.control_mode_label.grid(column=0, row=2, padx=1, pady=1, sticky=customtkinter.NSEW)

        self.motor_speed_label.grid(column=0, row=0, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.motor_speed_value.grid(column=1, row=0, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.motor_angular_label.grid(column=0, row=1, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.motor_angular_value.grid(column=1, row=1, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.command_speed_label.grid(column=0, row=2, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.command_speed_value.grid(column=1, row=2, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.command_angular_label.grid(column=0, row=3, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.command_angular_value.grid(column=1, row=3, padx=1, pady=1, sticky=customtkinter.NSEW)
        self.shutdown_button.grid(column=0, row=4, padx=1, pady=1, sticky=customtkinter.NSEW)

        self.open_launcher()

    def shutdown(self):
        self.destroy()

    def open_launcher(self):
        if self.launcher is None or not self.launcher.is_open:
            self.launcher = PantherLaunchUI(self)

    def update_fields(self, linearx_cmd, angularz_cmd, linearx_measured, angularz_measured):
        #self.linear_cmd.publish(data[0])
        #self.angular_cmd.publish(data[1])
        #self.linear_mes.publish(data[2])
        #self.angular_mes.publish(data[3])
        data = [linearx_cmd, linearx_measured, angularz_measured, angularz_measured]
        self.motor_speed_value.configure(text=str(data[2]))
        self.motor_angular_value.configure(text=str(data[3]))
        self.command_speed_value.configure(text=str(data[0]))
        self.command_angular_value.configure(text=str(data[1]))

    def callback_cmd(self, data):
        self.msg_cmd = data

        # Find measured vels
        linear_xm = round(self.odom.twist.twist.linear.x, 4)
        angular_zm = round(self.odom.twist.twist.angular.z, 4)

        # Find input vels
        linear_xi = round(self.msg_cmd.linear.x, 4)
        angular_zi = round(self.msg_cmd.angular.z, 4)

        # Write to .txt file
        self.update_fields(linear_xi, angular_zi, linear_xm, angular_zm)

    def callback_odom(self, data):
        self.odom = data

