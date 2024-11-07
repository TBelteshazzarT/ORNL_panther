#!/usr/bin/env python3

import rospy
import actionlib
import time
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from geometry_msgs.msg import Quaternion, Pose
import tf.transformations as tft

def move_to_goal(x, y, theta):
    client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
    client.wait_for_server()

    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()
    goal.target_pose.pose.position.x = x
    goal.target_pose.pose.position.y = y
    #goal.target_pose.pose.orientation.z = theta

    #not exactly working at the moment
    quaternion = tft.quaternion_from_euler(0, 0, theta)
    euler = tft.euler_from_qu  
    
    goal.target_pose.pose.orientation.x = quaternion[0]
    goal.target_pose.pose.orientation.y = quaternion[1]
    goal.target_pose.pose.orientation.z = quaternion[2]
    goal.target_pose.pose.orientation.w = quaternion[3]

    client.send_goal(goal)
    client.wait_for_result()
    return client.get_result()

def rotate_in_place(x, y, theta):
    client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
    client.wait_for_server()
    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()
    goal.target_pose.pose.position.x = x
    goal.target_pose.pose.position.y = y
    theta += 1.5708
    for x in range(3):
        quaternion = tft.quaternion_from_euler(0, 0, theta)
        goal.target_pose.pose.orientation.x = quaternion[0]
        goal.target_pose.pose.orientation.y = quaternion[1]
        goal.target_pose.pose.orientation.z = quaternion[2]
        goal.target_pose.pose.orientation.w = quaternion[3]
        client.send_goal(goal)
        client.wait_for_result()
        theta += 1.5708
        time.sleep(0.5)
    return client.get_result()



if __name__ == '__main__':
    rospy.init_node('goal_sender')
    cask_x = 4
    cask_y = 2
    x = -10.9728
    y = -5
    i=0
    j=0
    temp = -1
    loc = Pose()
    home_x = loc.position.x
    home_y = loc.position.y
    theta = 0.785398
    
    while i <= cask_x:
        temp = -temp
        result = move_to_goal(x, y, theta)
        time.sleep(0.5)
        rotate_in_place(x, y, theta)
        while j < cask_y:
            y = y + (5*temp)
            result = move_to_goal(x, y, theta)
            time.sleep(0.5)
            rotate_in_place(x, y, theta)
            j = j + 1
        j = 0
        x = x + 5.4864
        i = i+1
    home = move_to_goal(-12, 0, 0.1)
        

    if result:
        rospy.loginfo("Goal reached successfully.")
    else:
        rospy.loginfo("Failed to reach goal.")