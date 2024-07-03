cmake_minimum_required(VERSION 3.0.2)
project(panther_gazebo)

find_package(catkin REQUIRED)

catkin_package()

install(
  DIRECTORY
    launch
    materials
    models
    plugins
    worlds
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

