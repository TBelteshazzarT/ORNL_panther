#!/usr/bin/env python3
from datetime import datetime
import cv2.data
import cv2
import os
import rospy
from sensor_msgs.msg import Joy

last_button = 0

def camera_pic():
    global i, date
    now = datetime.now()
    date = str(now.strftime("%m-%d-%Y"))
    if not os.path.exists(f"/home/ornl-rov/panther_test_images/test-{date}"):
        os.makedirs(f"/home/ornl-rov/panther_test_images/test-{date}")  
    os.chdir(f"/home/ornl-rov/panther_test_images/test-{date}")
    cap_v = cv2.VideoCapture('rtsp://admin:ORNL_123@192.168.5.250:554/stream1')
    cap_v.set(cv2.CAP_PROP_BUFFERSIZE, 0)
    while True:
        for _ in range(5):
            cap_v.grab()
        ret_v, frame_v = cap_v.read()
        if not ret_v:
            break
        cv2.imwrite(f"visual_img-{str(datetime.now())}.jpg", frame_v)
        break
    cap_v.release()

    cap_t = cv2.VideoCapture('rtsp://admin:ORNL_123@192.168.5.250:554/stream3')
    cap_t.set(cv2.CAP_PROP_BUFFERSIZE, 0)
    while True:
        for _ in range(5):
            cap_t.grab()
        ret_t, frame_t = cap_t.read()
        if not ret_t:
            break
        cv2.imwrite(f"thermal_img-{str(datetime.now())}.jpg", frame_t)
        break
    cap_t.release()
    cv2.destroyAllWindows()


def joy_callback(msg):
    global last_button
    if msg.buttons[3] == 1 and last_button == 0:
        camera_pic()
    last_button = msg.buttons[3]

if __name__ == '__main__':
    #now = datetime.now()
    #date = str(now.strftime("%m-%d-%Y"))
    #i = 1
    #while  os.path.exists(f"/home/ornl-rov/panther_test_images/test-{date}"):
        #i += 1
    rospy.init_node('camera_picture')
    rospy.Subscriber("/joy", Joy, joy_callback)
    rospy.spin()
