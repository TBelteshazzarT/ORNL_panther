#!/usr/bin/env python3

import rospy
#from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Pose
import sensor_msgs.msg
import tf
import csv
import os

saved_coords = []

def home():
    return os.path.expanduser("~")

def pose_callback(data):
    global saved_coords
    x = data.position.x
    y = data.position.y
    saved_coords.append((x,y))
    print(saved_coords)

   
def main():
    rospy.init_node('pose_save')
    pose_sub = rospy.Subscriber('/robot_pose', Pose, pose_callback)
    rospy.spin()


    



if __name__ == '__main__':
    main()
       

