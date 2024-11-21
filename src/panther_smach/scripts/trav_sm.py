#!/usr/bin/env python3

import smach
import smach_ros
import rospy
import tf.transformations as tft
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
import actionlib
from std_msgs.msg import Bool
import camera_picture

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
        rospy.logwarn('Executing state Trav_sm')

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
        self.x = float(location[0])
        self.y = float(location[1])
        self.quaternion_x = float(location[2])
        self.quaternion_y = float(location[3])
        self.quaternion_z = float(location[4])
        self.quaternion_w = float(location[5])
        self.move_base_client = move_base_client

    def execute(self, userdata):
        rospy.logwarn('Executing main_nav substate')
        goal.target_pose.header.frame_id = "map"
        goal.target_pose.header.stamp = rospy.Time.now()
        goal.target_pose.pose.position.x = self.x
        goal.target_pose.pose.position.y = self.y

        
        # quaternion = tft.quaternion_from_euler(0, 0, self.theta)
    
        goal.target_pose.pose.orientation.x = self.quaternion_x
        goal.target_pose.pose.orientation.y = self.quaternion_y
        goal.target_pose.pose.orientation.z = self.quaternion_z
        goal.target_pose.pose.orientation.w = self.quaternion_w
        rospy.logwarn(f"Going to pose x={self.x}, y={self.y}, [{self.quaternion_x}, {self.quaternion_y}, {self.quaternion_z}, {self.quaternion_w}]")
        rospy.logwarn("waiting for move_base server")
        self.move_base_client.wait_for_server()
        rospy.logwarn("sending goal")
        self.move_base_client.send_goal(goal)
        rospy.logwarn("waiting for result ...")
        self.move_base_client.wait_for_result()

        state = self.move_base_client.get_state()
        rospy.logwarn(f"current move base state: {state}")
        state_str = str(state)
        return state_str
    
#record temp
class record_temp(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['recorded'])

    def execute(self, userdata):
        rospy.logwarn('Recording Temperature')
        camera_picture.camera_pic()
        return 'recorded'

# define substate manual_override
#daniel
class manual_override(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['sub_complete'])

    def execute(self, userdata):
        rospy.loginfo('Executing manual_override substate')
        return 'sub_complete'
    