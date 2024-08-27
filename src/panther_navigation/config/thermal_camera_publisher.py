#!/usr/bin/env python3
from __future__ import print_function

import rospy
from sensor_msgs.msg import Image
import cv2
from cv_bridge import CvBridge


def main():
    rospy.init_node('thermal_camera_publisher', anonymous=True)
    image_pub = rospy.Publisher('/camera/image_raw', Image, queue_size=10)
    bridge = CvBridge()

    stream_url = 'rtsp://admin:ORNL_123@192.168.0.250:554/stream3'
    cap = cv2.VideoCapture(stream_url)

    if not cap.isOpened():
        rospy.logerr("Cannot open network stream")
        return
        
    rate = rospy.Rate(1000)

    while not rospy.is_shutdown():
        ret, frame = cap.read()
        if not ret:
            rospy.logwarn("Failed to capture frame from the stream")
            continue
                
        ros_image = bridge.cv2_to_imgmsg(frame, encoding="bgr8")
        image_pub.publish(ros_image)
        rate.sleep()
    cap.release()
        
if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
    

        

