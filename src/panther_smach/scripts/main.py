#!/usr/bin/env python3

import rospy
import smach
from dock_sm import Undock, Dock
from trav_sm import Trav_sm
import roslaunch
import csv
import os

def home():
    return os.path.expanduser("~")

#main
def main():
    rospy.init_node('smach_plan')
    with open(str(home()) + '/panther_ws/src/panther_smach/saved_route/route.csv', 'r') as f:
        csv_reader = csv.reader(f)
        location_array = [row for row in csv_reader]       

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
    rospy.on_shutdown(shutdown_tasks)
    
def shutdown_tasks():
	node = roslaunch.core.Node('map_server','map_saver')
	launch = roslaunch.scriptapi.ROSLaunch()
	launch.start()
	process = launch.launch(node)
	print(process.is_alive())
	process.stop()
	

if __name__ == '__main__':
    try:
        main()
    
    except rospy.ROSInterruptException:
        pass
    
