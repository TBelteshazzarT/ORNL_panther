#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import LaserScan
import sensor_msgs.msg
import tf


def main():
    listener()

def listener():
    rospy.init_node('small_scan_node')
    scan_sub = rospy.Subscriber('/scan', LaserScan, callback)
    rospy.spin()

def tf_listener():
    pose_listener = tf.TransformListener()
    pose_listener.waitForTransform('/base_footprint', '/map', rospy.Time(), rospy.Duration(4.0))
    try:
        
        (trans, rot) = pose_listener.lookupTransform('/base_footprint', '/map', rospy.Time(0))
        print('Translation', trans)
        print('Rotation', rot)
    except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
        pass
    

def callback(msg):
    #print(len(msg.ranges))
    smallest_value = msg.ranges[0]
    for i in msg.ranges:
        if i < smallest_value:
            smallest_value = i
    print('The smallest value is', smallest_value)
    tf_listener()

if __name__ == '__main__':
    main()
       

