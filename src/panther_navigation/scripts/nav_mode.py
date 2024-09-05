#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Bool

msg_nav = Twist()
msg_joy = Twist()
msg_bool = False

class nav_mode:
    def __init__(self):
        # Ros publisher
        self.pub = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
        # Ros subscribers
        rospy.Subscriber('/nav_vel', Twist, self.callback_nav)
        rospy.Subscriber('/joy_vel', Twist, self.callback_joy)
        rospy.Subscriber('/pause_navigation', Bool, self.callback_bool)



    def pub_vel(self,lx,ly,lz,ax,ay,az):
        vel_twist = Twist()
        #add linear vector
        vel_twist.linear.x = lx
        vel_twist.linear.y = ly
        vel_twist.linear.z = lz
        #add angular vector
        vel_twist.angular.x = ax
        vel_twist.angular.y = ay
        vel_twist.angular.z = az

        self.pub.publish(vel_twist)


    def callback_nav(self,data):
        self.msg_nav = data

    def callback_joy(self,data):
        self.msg_joy = data

    def callback_bool(self,data):
        self.msg_bool = data
        #running main through callback    
        msg = Twist()
        
        if self.msg_bool.data:
            msg = self.msg_joy
        else:
            msg = self.msg_nav
        self.pub_vel(msg.linear.x,msg.linear.y,msg.linear.z,msg.angular.x,msg.angular.y,msg.angular.z)


    
if __name__ == '__main__':
    
    rospy.init_node('nav_mode')
    nav_mode()
    try:
        rospy.spin()
        
    except rospy.ROSInterruptException:
        pass