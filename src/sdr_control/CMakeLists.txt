cmake_minimum_required(VERSION 2.8.3)
project(sdr_control)

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

find_package(Boost REQUIRED COMPONENTS system)

add_service_files(
  DIRECTORY
  srv
  FILES
  SwitchVelocity.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES sdr_control
  CATKIN_DEPENDS nav_msgs roscpp sensor_msgs tf
  DEPENDS Boost
)

roslaunch_add_file_check(launch)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

#add_library(velocity_switch
#  src/vel_switch/velocity_switch.cpp
#)

# Vel mux build
#add_executable(vel_switch_node src/vel_switch/vel_switch_node.cpp)

#target_link_libraries(vel_switch_node
#  velocity_switch
#  ${catkin_LIBRARIES}
#  ${Boost_LIBRARIES}
#)

#add_dependencies(vel_switch_node sdr_control_generate_messages_cpp)

add_library(rosIOHelper
  src/rosIOHelper.cpp
)

# SyRen aux build
add_library(dimension_interface
  src/dimension/dimensionInterface.cpp
  src/serial_interface.cpp
)

add_executable(syren_aux_node src/dimension/syren/syren_aux_ros.cpp)

target_link_libraries(syren_aux_node
  dimension_interface
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
)

# Sabertooth build
add_executable(sabertooth_driver_node src/dimension/sabertooth/sabertooth_driver_ros.cpp)

target_link_libraries(sabertooth_driver_node
  dimension_interface
  rosIOHelper
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
)

# RoboteQ build
add_library(roboteq_interface
  src/roboteq/roboteq_interface.cpp
  src/serial_interface.cpp
)

add_executable(roboteq_driver_node src/roboteq/roboteq_driver_node.cpp)

target_link_libraries(roboteq_driver_node
  roboteq_interface
  rosIOHelper
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
)

# Roboclaw build
add_library(roboclaw_interface
  src/roboclaw/RoboClaw.cpp
  src/serial_interface.cpp
)

add_executable(roboclaw_driver_node src/roboclaw/roboclaw_driver_node.cpp)

target_link_libraries(roboclaw_driver_node
  roboclaw_interface
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
)

# Install
#install(TARGETS roboteq_driver_node rosIOHelper dimension_interface roboteq_interface roboclaw_driver_node roboclaw_interface vel_switch_node <- removed last target 7/1/24
install(TARGETS roboteq_driver_node rosIOHelper dimension_interface roboteq_interface roboclaw_driver_node roboclaw_interface
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

install(DIRECTORY launch config
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY srv/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/srv
  FILES_MATCHING PATTERN "*.srv"
)
