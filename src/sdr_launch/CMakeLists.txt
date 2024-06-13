cmake_minimum_required(VERSION 2.8.3)
project(sdr_launch)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS 
  roslaunch
  rospy
)

catkin_package()

catkin_install_python(PROGRAMS
  scripts/terminator
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

roslaunch_add_file_check(launch)

install(DIRECTORY launch rules
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
