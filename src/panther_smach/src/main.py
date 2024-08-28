#!/usr/bin/env python3

import rospy
import smach
from tutorial_states import Foo
from tutorial_states import Bar


#main
def main():
    rospy.init_node('smach_example_state_machine')

    # Create a SMACH state machine
    sm = smach.StateMachine(outcomes=['outcome4', 'outcome5'])

    # Open the container
    with sm:
        # Add states to the container (what outcome moves to what state)
        smach.StateMachine.add('FOO', Foo(), transitions={'outcome1':'BAR', 'outcome2':'outcome4'})
        smach.StateMachine.add('BAR', Bar(), transitions={'outcome2':'FOO'})

    # Execute SMACH plan
    outcome = sm.execute()

if __name__ == '__main__':
    main()