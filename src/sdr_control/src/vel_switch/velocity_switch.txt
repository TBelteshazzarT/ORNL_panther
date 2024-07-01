#include <sdr_control/velocity_switch.h>
#include <std_msgs/String.h>

namespace velocity_switch
{
    VelocitySwitch::VelocitySwitch(ros::NodeHandle& nh, const std::string& default_topic) 
        : nh_(nh), topic_(default_topic), lock_(false)
    {
        sub_ = nh_.subscribe<geometry_msgs::Twist>(topic_, 1, &VelocitySwitch::twist_callback, this);
        cmd_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);
        topic_pub_ = nh.advertise<std_msgs::String>("velocity_topic", 1);
        service_ = nh_.advertiseService("switch_velocity", &VelocitySwitch::switch_vel, this);
    }

    VelocitySwitch::~VelocitySwitch() { }

    void VelocitySwitch::spin()
    {
        std_msgs::String current_topic;
        ros::Rate r(20);

        while(ros::ok())
        {
            ros::spinOnce();

            current_topic.data = topic_;
            topic_pub_.publish(current_topic);
            
            r.sleep();
        }
    }

    void VelocitySwitch::twist_callback(const geometry_msgs::Twist::ConstPtr& twist)
    {
        if(!lock_) { cmd_pub_.publish(*twist); }
    }

    bool VelocitySwitch::switch_vel(sdr_control::SwitchVelocity::Request& req, sdr_control::SwitchVelocity::Response& res)
    {
        lock_ = true;
        sub_.shutdown();
        sub_ = nh_.subscribe<geometry_msgs::Twist>(req.switch_to, 1, &VelocitySwitch::twist_callback, this);
        lock_ = false;
        ROS_INFO("Switched to: %s", req.switch_to.c_str());
        res.switched = true;
        topic_ = req.switch_to;

        return true;
    }
}