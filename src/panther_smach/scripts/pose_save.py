#!/usr/bin/env python3

import rospy
#from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Pose
import sensor_msgs.msg
import tf
import csv
import os
from panther_smach.srv import PoseSave
import csv
import rospy
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Joy


last_button_x = 0
last_button_start =0
odom = Odometry()
#x, y, quat_x, quat_y, quat_z, quat_w = 0, 0, 0, 0, 0, 0

saved_coords = []
save_bool = False

def home():
    return os.path.expanduser("~")

def pose_callback(data):
    global saved_coords, save_bool

    if save_bool:
        x = float(data.position.x)
        y = float(data.position.y)
        quat_x = float(data.orientation.x)
        quat_y = float(data.orientation.y)
        quat_z = float(data.orientation.z)
        quat_w = float(data.orientation.w)
        rospy.logwarn(f"Saving location x = {x:.2f}, y = {y:.2f}, and quaternion = [{quat_x:.2f}, {quat_y:.2f}, {quat_z:.2f}, {quat_w:.2f}].")
        saved_coords.append([x,y,quat_x, quat_y, quat_z, quat_w])
    save_bool = False

#def handle_save_pose(req):
    #global save_bool
    #save_bool = True
    #return req
    
def shutdownhook():
    rospy.logwarn("Saving Route to csv..")
    global saved_coords
    with open(str(home()) + '/panther_ws/src/panther_smach/saved_route/route.csv', 'w') as f:
        csvwriter = csv.writer(f)
        for k in saved_coords:
            line = [k[0], k[1], k[2]]
            csvwriter.writerow(line)

def joy_callback_2(msg):
    global last_button_x, last_button_start, save_bool, saved_coords
    if msg.buttons[2] == 1 and last_button_x == 0:
        save_bool = True
        # rospy.loginfo(f"Saving location x = {x:.2f}, y = {y:.2f}, and theta = {theta:.2f}.")
    last_button_x = msg.buttons[2]
    if msg.buttons[7] == 1 and last_button_start == 0 and len(saved_coords) > 0:
        # print(location_array)
        with open(str(home()) + '/panther_ws/src/panther_smach/saved_route/route.csv', 'w') as f:
            csvwriter = csv.writer(f)
            for k in saved_coords:
                line = [k[0], k[1], k[2], k[3], k[4], k[5]]
                csvwriter.writerow(line)
        rospy.logwarn("Saving Route to csv..")
        f.close()
    last_button_start = msg.buttons[7]
    
   
def main():
    rospy.init_node('pose_save')
    pose_sub = rospy.Subscriber('/robot_pose', Pose, pose_callback)
    #s = rospy.Service('pose_save', PoseSave, handle_save_pose)
    rospy.Subscriber("/joy", Joy, joy_callback_2)
    rospy.spin()
    #rospy.on_shutdown(shutdownhook)


    



if __name__ == '__main__':
    main()
       

