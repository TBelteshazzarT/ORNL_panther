#!/usr/bin/env python3

import rospy
import smach
from dock_sm import Undock, Dock
from trav_sm import Trav_sm


#main
def main():
    rospy.init_node('smach_plan')
    cask_x = 4
    cask_y = 2
    x = -10.9728
    y = -5
    i=0
    j=0
    k =0
    temp = -1
    theta = 0.785398
    location_array = []
    
    while i <= cask_x:
        temp = -temp
        location_array.append([x, y, theta])
        k += 1
        while j < cask_y:
            y = y + (5*temp)
            location_array.append([x, y, theta])
            j = j + 1
            k+= 1
        j = 0
        x = x + 5.4864
        i = i+1

    # Create a SMACH state machine
    sm = smach.StateMachine(outcomes=['undocking_error', 'travel_error', 'docking_error', 'loop_complete'])

    # Open the container
    with sm:
        # Add states to the container (what outcome moves to what state)
        smach.StateMachine.add('Undock', Undock(), transitions={'complete':'Trav_sm', 'error':'undocking_error'})
        smach.StateMachine.add('Trav_sm', Trav_sm(location_array), transitions={'complete':'Dock', 'error':'travel_error'})
        smach.StateMachine.add('Dock', Dock(), transitions={'complete':'loop_complete', 'error':'docking_error'})

    # Execute SMACH plan
    outcome = sm.execute()

if __name__ == '__main__':
    try:
        main()
    
    except rospy.ROSInterruptException:
        pass
    