#!/usr/bin/env python3

import rospy
import smach

# define state Undock
class Undock(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['complete','error'])
        self.dock_status = True
    
    def execute(self, userdata):
        rospy.loginfo('Executing state Undock')
        if self.dock_status:
            self.dock_status = False
            return 'complete'
        else:
            return 'error'

# define state Dock
class Dock(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['complete','error'])

    def execute(sefl, userdata):
        rospy.loginfo('Executing state Dock')
        return 'complete'