#include <string>
#include <math.h>
#include <ros/ros.h>
#include <sdr_control/dimensionInterface.h>
#include <sdr_control/rosIOHelper.h>
#include <boost/assign/list_of.hpp>

enum ROBOT_TYPE
{
    DIFF = 0,
    SKID_DAISYCHAIN = 1,
    MEC_DAISYCHAIN = 2
};

struct PARAMS
{
    std::string port;
    int32_t baud_rate;
    int32_t address;
    int32_t address_2;
    float timeout;
    std::string robot_type_str;
    std::map<std::string, ROBOT_TYPE> robot_map = boost::assign::map_list_of
        ("DIFF", DIFF)("SKID_DAISYCHAIN", SKID_DAISYCHAIN)("MEC_DAISYCHAIN", MEC_DAISYCHAIN);
    int32_t drive_factor;
    int32_t turn_factor;
    int32_t strafe_factor;
};

PARAMS set_params(ros::NodeHandle n)
{
    PARAMS p;
    // Serial port
    n.param<std::string>("sabertooth_driver/port", p.port, "/dev/ttyACM0");
    // Serial baud rate
    n.param("sabertooth_driver/baud_rate", p.baud_rate, 9600);
    // Address of the first controller
    n.param("sabertooth_driver/address", p.address, 128);
    // Address of the second controller (usage depends on robot type)
    n.param("sabertooth_driver/address_2", p.address_2, 129);
    // Control timeout
    n.param<float>("sabertooth_driver/timeout", p.timeout, 1.5F);
    // Platform type
    n.param<std::string>("sabertooth_driver/robot_type", p.robot_type_str, "DIFF");
    // Multiplier for linear x velocity to sabertooth commands
    n.param("sabertooth_driver/drive_factor", p.drive_factor, 126);
    // Multiplier for angular z velocity to sabertooth commands
    n.param("sabertooth_driver/turn_factor", p.turn_factor, 126);
    // Multiplier for linear y velocity to sabertooth commands
    n.param("sabertooth_driver/strafe_factor", p.strafe_factor, 126);

    return p;
}

void cmd_motors(dimensionInterface *di, geometry_msgs::Twist cmd, PARAMS p)
{
    int16_t dimDrive = p.drive_factor * cmd.linear.x;
    int16_t dimTurn = p.turn_factor * cmd.angular.z;
    int16_t dimStrafe = p.strafe_factor * cmd.linear.y;

    switch(p.robot_map[p.robot_type_str])
    {
        case DIFF:
            di->driveTurnSabertooth(p.address, dimDrive, dimTurn);
            break;

        case SKID_DAISYCHAIN:
            di->driveTurnSabertooth(p.address, dimDrive, dimTurn);
            di->driveTurnSabertooth(p.address_2, dimDrive, dimTurn);
            break;

        case MEC_DAISYCHAIN:
            int16_t fr = dimDrive + dimTurn + dimStrafe;
            int16_t fl = dimDrive - dimTurn - dimStrafe;
            int16_t rr = dimDrive + dimTurn - dimStrafe;
            int16_t rl = dimDrive - dimTurn + dimStrafe;
            di->mecSabertooth(p.address, p.address_2, fr, fl, rr, rl);
            break;
    }
}

int main(int argc, char **argv)
{
    // ROS Init
    ros::init(argc, argv, "sabertooth_driver");
    ros::NodeHandle n;
    ros::Rate r(20);        // 20 Hz
    // ROS Params
    PARAMS p = set_params(n);
    // Publisher and subscriber helper
    rosIOHelper ros_io(n);
    geometry_msgs::Twist cmd_vel;
    geometry_msgs::Twist zero_vel = {};
    ros::Time last_cmd_stamp;
    // Controller setup
    dimensionInterface *di = new dimensionInterface(p.port, p.baud_rate);
    if(!di->initialize()) { return 0; }

    // Main loop
    while(ros::ok())
    {
        ros::spinOnce();
        // Check velocity commands
        if(ros_io.grab_velocity(cmd_vel))
        {
            // set timer and control motors
            last_cmd_stamp = ros::Time::now();
            cmd_motors(di, cmd_vel, p);
        }
        else if((ros::Time::now()-last_cmd_stamp).toSec() >= p.timeout)
        {
            // stop motors
            cmd_motors(di, zero_vel, p);
        }
        r.sleep();
    }
    return 1;
}