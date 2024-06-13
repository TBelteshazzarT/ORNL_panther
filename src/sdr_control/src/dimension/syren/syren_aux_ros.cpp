#include <string>
#include <math.h>
#include <ros/ros.h>
#include <sdr_control/dimensionInterface.h>
#include <geometry_msgs/Twist.h>

class Listener
{
public:
    geometry_msgs::Twist cmd_aux;
    ros::Time last_cmd_stamp;
    void callback(const geometry_msgs::Twist::ConstPtr &input)
    {
        cmd_aux = *input;
        last_cmd_stamp = ros::Time::now();
    }
};

struct PARAMS
{
    std::string port;
    int32_t cmd_factor;
    float timeout;
    int32_t baud_rate;
    int32_t address;
};

PARAMS set_params(ros::NodeHandle n)
{
    PARAMS p;
    // Serial port
    n.param<std::string>("syren_aux/port", p.port, "/dev/ttyACM0");
    // Multiplier for percent effort to SyRen command
    n.param("syren_aux/cmd_factor", p.cmd_factor, 126);
    // Command timeout
    n.param<float>("syren_aux/timeout", p.timeout, 1.5F);
    // Controller baud rate
    n.param("syren_aux/baud_rate", p.baud_rate, 9600);
    // Controller address
    n.param("syren_aux/address", p.address, 128);

    return p;
}

int main(int argc, char **argv)
{
    // ROS Init
    ros::init(argc, argv, "syren_aux");
    ros::NodeHandle n;
    ros::Rate r(20);        // 20 Hz
    // ROS Params
    PARAMS p = set_params(n);
    // Subscribers
    Listener listener;
    ros::Subscriber sub = n.subscribe<geometry_msgs::Twist>("/cmd_aux", 5, &Listener::callback, &listener);
    // Controller setup
    dimensionInterface di(p.port, p.baud_rate);
    if(!di.initialize()) { return 0; }

    // Main loop
    while(ros::ok())
    {
        ros::spinOnce();
        // Check timeout and send controller commands
        if((ros::Time::now()-listener.last_cmd_stamp).toSec() <= p.timeout)
        {
            di.cmdSyrenChannel(p.address, listener.cmd_aux.linear.x*p.cmd_factor);
        }
        else { di.cmdSyrenChannel(p.address, 0); }

        r.sleep();
    }
    return 1;
}