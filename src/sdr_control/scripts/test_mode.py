#!/usr/bin/env python3

import rospy

from std_msgs.msg import Bool

def publisher():
    rospy.init_node('test_mode_enable')
    pub = rospy.Publisher('/pause_navigation', Bool, queue_size=1)
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        pub.publish(True)
        rate.sleep()
    pub.publish(False)
if __name__ == '__main__':
    
    
    try:
        publisher()
        
    except rospy.ROSInterruptException:
        pass