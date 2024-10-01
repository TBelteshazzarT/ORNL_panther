echo launching main Panther ...
export ROS_MASTER_URI=http://192.168.5.235:11311
export ROS_IP=192.168.5.235
wait
source /opt/ros/noetic/setup.bash
source ~/panther_ws/devel/setup.bash
echo $ROS_MASTER_URI
echo $ROS_IP
sleep 1
roslaunch sdr_launch main.launch
