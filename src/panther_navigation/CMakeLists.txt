cmake_minimum_required(VERSION 3.0.2)
project(panther_navigation)

find_package(catkin REQUIRED COMPONENTS
  roslaunch
  rospy
  std_msgs
)
catkin_package()

install(
  DIRECTORY
    config
    launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(PROGRAMS src/panther_navigation/scripts/nav_mode.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)


