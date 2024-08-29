#!/usr/bin/env python3

import rospy
import smach
from dock_sm import Undock, Dock
from trav_sm import Trav_sm


#main
def main():
    rospy.init_node('smach_plan')

    # Create a SMACH state machine
    sm = smach.StateMachine(outcomes=['undocking_error', 'travel_error', 'docking_error', 'loop_complete'])

    # Open the container
    with sm:
        # Add states to the container (what outcome moves to what state)
        smach.StateMachine.add('Undock', Undock(), transitions={'complete':'Trav_sm', 'error':'undocking_error'})
        smach.StateMachine.add('Trav_sm', Trav_sm(), transitions={'complete':'Dock', 'error':'travel_error'})
        smach.StateMachine.add('Dock', Dock(), transitions={'complete':'loop_complete', 'error':'docking_error'})

    # Execute SMACH plan
    outcome = sm.execute()

if __name__ == '__main__':
    try:
        main()
    
    except rospy.ROSInterruptException:
        pass
    