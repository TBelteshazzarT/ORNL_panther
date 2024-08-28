#!/usr/bin/env python3

import smach
import smach_ros
import rospy
import tf.transformations as tft
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
import actionlib

goal = MoveBaseGoal()

#move to location
class MoveToLocation(smach.state):
    def __init__(self, x, y, theta):
        smach.State.__init__(self, outcomes=['succeeded', 'aborted', 'preempted'])
        self.x = x
        self.y = y
        self.theta = theta

    #also want a call back to track its current position
    
    def execute(self, userdata):

        goal.target_pose.header.frame_id = "map"
        goal.target_pose.header.stamp = rospy.Time.now()
        goal.target_pose.pose.position.x = self.x
        goal.target_pose.pose.position.y = self.y

        
        quaternion = tft.quaternion_from_euler(0, 0, self.theta)
    
        goal.target_pose.pose.orientation.x = quaternion[0]
        goal.target_pose.pose.orientation.y = quaternion[1]
        goal.target_pose.pose.orientation.z = quaternion[2]
        goal.target_pose.pose.orientation.w = quaternion[3]

        move_base_state = smach_ros.SimpleActionState('move_base', MoveBaseAction, goal=goal)

        outcome = move_base_state.execute(None)

        return outcome

#Cancel Goal. This might need to be mapped to a key or something
class CancelGoal(smach.state):
    def __init__(self):
        smach.State.__init__(self, outcomes=['corrected', 'aborted'])

    def execute(self, userdata):
        # save goal data
        x=goal.target_pose.pose.position.x
        y=goal.target_pose.pose.position.y
        theta=goal.target_pose.pose.orientation.z
        # need to ouput this too
        move_base_client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
        move_base_client.wait_for_server()
        rospy.loginfo("Cancelling goal...")
        move_base_client.cancel_goal()



        if move_base_client.get_state() == actionlib.GoalStatus.PREEMPTED:
            return 'canceled'
        else:
            return 'failed'
        
#Align with Vent

       
#main
def main():
    rospy.init_node('motion_plan.py')

    #

    sm = smach.StateMachine(outcomes = ['outcome 1', 'outcome 2'])

    with sm:

        smach.StateMachine.add('MoveBaseGoal', MoveBaseGoal(), transitions={'succeeded':'', 'abort':''})

    outcome = sm.execute()

if __name__ == '__main__':
    main()




