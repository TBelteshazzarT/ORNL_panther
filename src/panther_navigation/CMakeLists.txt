cmake_minimum_required(VERSION 3.0.2)
project(panther_navigation)

find_package(catkin REQUIRED)

catkin_package()

install(
  DIRECTORY
    config
    launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(PROGRAMS src/panther_navigation/config/goal_sender.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)