#!/usr/bin/env python3

import smach
import smach_ros
import rospy
import tf.transformations as tft
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
import actionlib
from std_msgs.msg import Bool

goal = MoveBaseGoal()

# Mainstate

# define state Trav_sm
class Trav_sm(smach.State):
    def __init__(self, location_array):
        smach.State.__init__(self, outcomes=['complete','error'])
        self.location_array = location_array
        self.move_base_client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
        self.move_base_client.wait_for_server()
    
    def execute(self, userdata):
        rospy.loginfo('Executing state Trav_sm')

        '''pause test
        rospy.loginfo('Testing nav pause')
        pub = rospy.Publisher('/pause_navigation', Bool, queue_size=1)
        pub.publish(True)
        rospy.sleep(20)
        pub.publish(False)'''


        # Create a SMACH state machine
        sm = smach.StateMachine(outcomes=['complete'])

        # Open with the container
        with sm:
            for location in range(len(self.location_array)):
            
                # Add substates to the container
                smach.StateMachine.add(f'main_nav_{location}', main_nav(self.location_array[location], self.move_base_client), transitions={'3':f'record_temp_{location}', '4':'manual_override'})
                smach.StateMachine.add(f'record_temp_{location}', record_temp(), transitions={'recorded':f'main_nav_{location+1}' if location+1 < len(self.location_array) else 'complete'})

            smach.StateMachine.add('manual_override', manual_override(), transitions={'sub_complete':'complete'})

        # Sub-execute SMACH plan
        outcome = sm.execute()

        return 'complete'


# Substates

# define substate main_nav    
class main_nav(smach.State):
    def __init__(self, location, move_base_client):
        smach.State.__init__(self, outcomes=['3', '4'])
        self.x = location[0]
        self.y = location[1]
        self.theta = location[2]
        self.move_base_client = move_base_client

    def execute(self, userdata):
        rospy.loginfo('Executing main_nav substate')
        goal.target_pose.header.frame_id = "map"
        goal.target_pose.header.stamp = rospy.Time.now()
        goal.target_pose.pose.position.x = self.x
        goal.target_pose.pose.position.y = self.y

        
        quaternion = tft.quaternion_from_euler(0, 0, self.theta)
    
        goal.target_pose.pose.orientation.x = quaternion[0]
        goal.target_pose.pose.orientation.y = quaternion[1]
        goal.target_pose.pose.orientation.z = quaternion[2]
        goal.target_pose.pose.orientation.w = quaternion[3]

        self.move_base_client.wait_for_server()
        self.move_base_client.send_goal(goal)
        self.move_base_client.wait_for_result()

        state = self.move_base_client.get_state()
        state_str = str(state)
        return state_str
    
#record temp
class record_temp(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['recorded'])

    def execute(self, userdata):
        rospy.loginfo('Recording Temperature')
        return 'recorded'

# define substate manual_override
#daniel
class manual_override(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['sub_complete'])

    def execute(self, userdata):
        rospy.loginfo('Executing manual_override substate')
        return 'sub_complete'
    