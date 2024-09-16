cmake_minimum_required(VERSION 3.0.2)
project(panther_test)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslaunch
  rospy
  rqt_multiplot
  std_msgs
)

catkin_package()

install(
  DIRECTORY
    src
    launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(
  PROGRAMS
    src/panther_test/scripts/test_mode.py
    src/panther_test/scripts/velocity_logger.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
