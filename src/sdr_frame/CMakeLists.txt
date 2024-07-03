cmake_minimum_required(VERSION 2.8.3)
project(sdr_frame)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS roslaunch)

catkin_package()

roslaunch_add_file_check(launch)

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
