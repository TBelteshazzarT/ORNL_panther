cmake_minimum_required(VERSION 3.0.2)
project(panther_gazebo)
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roslaunch
  nav_msgs
  roscpp
  sensor_msgs
  tf
  message_generation
  message_runtime
)


catkin_package(
  CATKIN_DEPENDS nav_msgs roscpp sensor_msgs tf
)

include_directories(${catkin_INCLUDE_DIRS})

# RoboSim build

add_executable(robosim_driver_node src/robosim/robosim_driver_node.cpp)

target_link_libraries(robosim_driver_node
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
)

# Install
install(TARGETS robosim_driver_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

install(
  DIRECTORY
    launch
    materials
    models
    plugins
    worlds
    src
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

