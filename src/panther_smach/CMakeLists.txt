cmake_minimum_required(VERSION 3.0.2)
project(panther_smach)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roslaunch
  rospy
  std_msgs
  smach
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES panther_smach
#  CATKIN_DEPENDS roslaunch rospy std_msgs
#  DEPENDS system_lib
)

install(
  DIRECTORY
    launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(PROGRAMS src/panther_smach/src/goal_sender.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)


