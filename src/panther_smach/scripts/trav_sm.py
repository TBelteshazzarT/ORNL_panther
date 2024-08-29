#!/usr/bin/env python3

import rospy
import smach

# Mainstate

# define state Trav_sm
class Trav_sm(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['complete','error'])
    
    def execute(self, userdata):
        rospy.loginfo('Executing state Trav_sm')

        # Create a SMACH state machine
        sm = smach.StateMachine(outcomes=['complete'])

        # Open with the container
        with sm:
            # Add substates to the container
            smach.StateMachine.add('main_nav', main_nav(), transitions={'sub_complete':'complete', 'sub_error':'manual_override'})
            smach.StateMachine.add('manual_override', manual_override(), transitions={'sub_complete':'complete'})

        # Sub-execute SMACH plan
        outcome = sm.execute()

        return 'complete'


# Substates

# define substate main_nav    
class main_nav(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['sub_complete', 'sub_error'])

    def execute(self, userdata):
        rospy.loginfo('Executing main_nav substate')
        return 'sub_error'

# define substate manual_override
class manual_override(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['sub_complete'])

    def execute(self, userdata):
        rospy.loginfo('Executing manual_override substate')
        return 'sub_complete'