#!/usr/bin/env python3

import rospy
import actionlib
import time
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal

def move_to_goal(x, y, theta):
    client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
    client.wait_for_server()

    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()
    goal.target_pose.pose.position.x = x
    goal.target_pose.pose.position.y = y
    goal.target_pose.pose.orientation.z = theta  # Set appropriate orientation (quaternion)

    client.send_goal(goal)
    client.wait_for_result()
    return client.get_result()

if __name__ == '__main__':

    cask = 4
    temp = -5.4864
    i=1

    while i < cask:
        rospy.init_node('goal_sender')
        result = move_to_goal(temp, 0.0, 1)
        time.sleep(2)
        temp = temp + 5.4864
        i = i+1
        

    if result:
        rospy.loginfo("Goal reached successfully.")
    else:
        rospy.loginfo("Failed to reach goal.")