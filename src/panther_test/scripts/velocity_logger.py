#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Float64
from nav_msgs.msg import Odometry
import csv
import os

fields = ['Linear Velocity (CMD)','Angular Velocity (CMD)', 'Linear Velocity Measured', 'Angular Velocity Measured']
first = True

def home():
    return os.path.expanduser("~")

class Vel_logger:
    def __init__(self):
        # Ros publishers
        self.linear_cmd = rospy.Publisher('/linear_cmd', Float64, queue_size=10)
        self.angular_cmd = rospy.Publisher('/angular_cmd', Float64, queue_size=10)
        self.linear_mes = rospy.Publisher('/linear_mes', Float64, queue_size=10)
        self.angular_mes = rospy.Publisher('/angular_mes', Float64, queue_size=10)
        # Ros subscribers
        rospy.Subscriber('/cmd_vel', Twist, self.callback_cmd)
        rospy.Subscriber('/odom', Odometry, self.callback_odom)
        with open(str(home()) + '/vel_log.csv', 'w') as f:
            self.csvwriter = csv.writer(f)
            rospy.spin()
   

    def write_to_txt(self, linearx_cmd, angularz_cmd, linearx_measured, angularz_measured):
        global first
        if first == True:
            self.csvwriter.writerow(fields)
            first = False
        else:
            data = [linearx_cmd, linearx_measured, angularz_measured, angularz_measured]
            self.csvwriter.writerow(data)
            self.publishers(data)

    def callback_cmd(self,data):
        self.msg_cmd = data

        # Find measured vels
        linear_xm = self.odom.twist.twist.linear.x
        angular_zm = self.odom.twist.twist.angular.z

        # Find input vels
        linear_xi = self.msg_cmd.linear.x
        angular_zi = self.msg_cmd.angular.z

        # Write to .txt file
        self.write_to_txt(linear_xi, angular_zi, linear_xm, angular_zm)
        

    def callback_odom(self,data):
        self.odom = data
        
    def publishers(self, data):
        self.linear_cmd.publish(data[0])
        self.angular_cmd.publish(data[1])
        self.linear_mes.publish(data[2])
        self.angular_mes.publish(data[3])

    
if __name__ == '__main__':
    
    rospy.init_node('vel_logger')
    
    try:
        Vel_logger()
                
    except rospy.ROSInterruptException:
        pass
