# CMake generated Testfile for 
# Source directory: /home/ornl-rover/panther_ws/src/sdr_control
# Build directory: /home/ornl-rover/panther_ws/build/sdr_control
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_sdr_control_roslaunch-check_launch "/home/ornl-rover/panther_ws/build/sdr_control/catkin_generated/env_cached.sh" "/usr/bin/python3" "/opt/ros/noetic/share/catkin/cmake/test/run_tests.py" "/home/ornl-rover/panther_ws/build/sdr_control/test_results/sdr_control/roslaunch-check_launch.xml" "--return-code" "/usr/bin/cmake -E make_directory /home/ornl-rover/panther_ws/build/sdr_control/test_results/sdr_control" "/opt/ros/noetic/share/roslaunch/cmake/../scripts/roslaunch-check -o \"/home/ornl-rover/panther_ws/build/sdr_control/test_results/sdr_control/roslaunch-check_launch.xml\" \"/home/ornl-rover/panther_ws/src/sdr_control/launch\" ")
set_tests_properties(_ctest_sdr_control_roslaunch-check_launch PROPERTIES  _BACKTRACE_TRIPLES "/opt/ros/noetic/share/catkin/cmake/test/tests.cmake;160;add_test;/opt/ros/noetic/share/roslaunch/cmake/roslaunch-extras.cmake;66;catkin_run_tests_target;/home/ornl-rover/panther_ws/src/sdr_control/CMakeLists.txt;37;roslaunch_add_file_check;/home/ornl-rover/panther_ws/src/sdr_control/CMakeLists.txt;0;")
subdirs("gtest")
