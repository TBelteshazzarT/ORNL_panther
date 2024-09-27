#include <string>
#include <ros/ros.h>
#include <sdr_control/velocity_switch.h>

struct PARAMS
{
    std::string first_state;
};

PARAMS get_params(ros::NodeHandle n)
{
    PARAMS p;
    // Initial state
    n.param<std::string>("vel_switch/first_state", p.first_state, "joy_vel");

    return p;
}

int main(int argc, char **argv)
{
    // ROS Init
	ros::init(argc,argv,"vel_switch");
	ros::NodeHandle n;
    // ROS Params
    PARAMS p = get_params(n);

    velocity_switch::VelocitySwitch vs(n, p.first_state);
    vs.spin();
}