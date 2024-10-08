#!/usr/bin/env python3
import roslaunch
import rospy
import os

def home():
    return os.path.expanduser("~")

def main():
    rospy.init_node('en_Mapping', anonymous=True)
    uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
    roslaunch.configure_logging(uuid)
    launch = roslaunch.parent.ROSLaunchParent(uuid, [str(home()) + "/panther_ws/src/panther_navigation/launch/map_save.launch"])
    launch.start()
    rospy.loginfo("started")
    rospy.sleep(60)
    main()

if __name__ == '__main__':
    main()