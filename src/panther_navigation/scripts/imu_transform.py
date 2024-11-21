#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import Imu
import numpy as np

def imu_callback(msg):
    new_msg=Imu()
    new_msg.header = msg.header
    new_msg.orientation = msg.orientation
    x = msg.orientation.x
    y = msg.orientation.y
    z = msg.orientation.z
    w = msg.orientation.w
    new_msg.orientation_covariance = msg.orientation_covariance
    new_msg.angular_velocity = msg.angular_velocity
    new_msg.angular_velocity_covariance = msg.angular_velocity_covariance
    
    #Rotation matrix
    Rxx = 1-2*y**2-2*z**2
    Rxy = 2*x*y-2*z*w
    Rxz = 2*x*z+2*y*w
    Ryx = 2*x*y+2*z*w
    Ryy = 1-2*x**2-2*z**2
    Ryz = 2*y*z+2*x*w
    Rzx = 2*x*z+2*y*w
    Rzy = 2*y*z-2*x*w
    Rzz = 1-2*x**2-2*y**2
    r = np.array([[Rxx, Ryx, Rzx],[Rxy, Ryy, Rzy],[Rxz, Ryz, Rzz]])
    #inverse_r = np.linalg.inv(r)
    
    #accel
    accel_vector = np.array([msg.linear_acceleration.x, msg.linear_acceleration.y, msg.linear_acceleration.z])
    accel_sensor = np.dot(r, accel_vector)
    new_msg.linear_acceleration.x = accel_sensor[0]
    new_msg.linear_acceleration.y = accel_sensor[1]
    new_msg.linear_acceleration.z = accel_sensor[2]
    new_msg.linear_acceleration_covariance = msg.linear_acceleration_covariance

    pub_imu.publish(new_msg)

def main():   
    rospy.init_node('imu_transform')
    sub_imu = rospy.Subscriber('/imu/data', Imu, imu_callback)
    rospy.spin()

if __name__ == '__main__':
    pub_imu = rospy.Publisher('/imu', Imu, queue_size=10)
    main()
