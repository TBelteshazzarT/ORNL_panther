#!/usr/bin/env python3

import rospy
import smach

# define state Foo
class Foo(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome1','outcome2'])
        self.counter = 0
    
    def execute(self, userdata):
        rospy.loginfo('Executing state Foo')
        if self.counter < 3:
            self.counter += 1
            return 'outcome1'
        else:
            return 'outcome2'

# define state Bar
class Bar(smach.State):
    def __init__(self):
        smach.State.__init__(self, outcomes=['outcome2'])

    def execute(sefl, userdata):
        rospy.loginfo('Executing state Bar')
        return 'outcome2'