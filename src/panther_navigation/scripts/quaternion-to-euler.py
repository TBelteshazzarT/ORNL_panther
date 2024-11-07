#!/usr/bin/env python3
import rospy
import tf.transformations as tft
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Imu
from std_msgs.msg import Float32, Int32
import math
roll = 0
pitch = 0
yaw_odom = 0
yaw_filter = 0
yaw_imu = 0
t1 = 1

def get_rotation_odom(msg):
    global roll, pitch, yaw_odom, t1
    orientation = msg.pose.pose.orientation
    o_list = [orientation.x, orientation.y, orientation.z, orientation.w]
    (roll, pitch, yaw_odom) = tft.euler_from_quaternion(o_list)
    yaw_odom = (yaw_odom*180)/math.pi
    #print(f"odom: {yaw_odom}")
    pub_odom.publish(yaw_odom)
    t1_msg = Int32()
    t1_msg.data = t1
    pub_counter.publish(t1_msg)
    t1=t1+1

def get_rotation_filtered(msg):
    global roll, pitch, yaw_filter
    orientation = msg.pose.pose.orientation
    o_list = [orientation.x, orientation.y, orientation.z, orientation.w]
    (roll, pitch, yaw_filter) = tft.euler_from_quaternion(o_list)
    yaw_filter = (yaw_filter*180)/math.pi
    #print(f"odom filtered: {yaw_filter}")
    pub_filter.publish(yaw_filter)

def get_rotation_imu(msg):
    global roll, pitch, yaw_imu
    orientation = msg.orientation
    o_list = [orientation.x, orientation.y, orientation.z, orientation.w]
    (roll, pitch, yaw_imu) = tft.euler_from_quaternion(o_list)
    yaw_imu = (yaw_imu*180)/math.pi
    #print(f"imu: {yaw_imu}")
    pub_imu.publish(yaw_imu)

def main():
    
    rospy.init_node('quaternion_to_euler')
    sub_odom = rospy.Subscriber('/odom', Odometry, get_rotation_odom)
    sub_filter = rospy.Subscriber('/odometry/filtered', Odometry, get_rotation_filtered)
    sub_imu = rospy.Subscriber('/imu', Imu, get_rotation_imu)
    rospy.spin()

if __name__ == '__main__':
    pub_odom = rospy.Publisher('/euler_odom', Float32, queue_size=10)
    pub_counter = rospy.Publisher('/euler_counter', Int32, queue_size=10)
    pub_filter = rospy.Publisher('/euler_filter', Float32, queue_size=10)
    pub_imu = rospy.Publisher('/euler_imu', Float32, queue_size=10)
    main()
