#!/usr/bin/env python3

import rospy
#from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Pose
import sensor_msgs.msg
import tf
import csv
import os
from panther_smach.srv import PoseSave

saved_coords = []
save_bool = False

def home():
    return os.path.expanduser("~")

def pose_callback(data):
    global saved_coords, save_bool
    if save_bool:
    	x = data.position.x
    	y = data.position.y
    	theta = data.orientation.z
    	saved_coords.append([x,y,theta])
    save_bool = False

def handle_save_pose(req):
    global save_bool
    save_bool = True
    return req
    
def shutdownhook():
    rospy.loginfo("Saving Route to csv..")
    global saved_coords
    with open(str(home()) + '/panther_ws/src/panther_smach/saved_route/route.csv', 'w') as f:
        csvwriter = csv.writer(f)
        for k in saved_coords:
            line = [k[0], k[1], k[2]]
            csvwriter.writerow(line)
   
def main():
    rospy.init_node('pose_save')
    pose_sub = rospy.Subscriber('/robot_pose', Pose, pose_callback)
    s = rospy.Service('pose_save', PoseSave, handle_save_pose)
    rospy.spin()
    rospy.on_shutdown(shutdownhook)


    



if __name__ == '__main__':
    main()
       

