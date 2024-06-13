#ifndef VELOCITY_SWITCH_H
#define VELOCITY_SWITCH_H

#include <string>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sdr_control/SwitchVelocity.h>

namespace velocity_switch
{
    /**
     * @brief Class that decides which commands are published to /cmd_vel
     * 
     */
    class VelocitySwitch
    {
        public:
            /**
             * @brief Construct a new Velocity Switch object
             * @param nh ROS node handle
             * @param default_topic Initial topic name to subscribe to
             */
            VelocitySwitch(ros::NodeHandle& nh, const std::string& default_topic);
            /**
             * @brief Destroy the Velocity Switch object
             */
            ~VelocitySwitch();
            /**
             * @brief Main loop for the ROS node
             */
            void spin();

        private:
            ros::NodeHandle nh_;
            std::string topic_;
            ros::Publisher cmd_pub_, topic_pub_;
            ros::Subscriber sub_;
            ros::ServiceServer service_;
            bool lock_;
            /**
             * @brief Callback triggered when an incoming Twist message is buffered
             * @param twist Pointer of a Twist message
             */
            void twist_callback(const geometry_msgs::Twist::ConstPtr& twist);
            /**
             * @brief ROS service that maps the subscriber object to a different topic
             * @param req Service request that contains a topic name to switch to
             * @param res Service response that returns true when the switch is successful
             * @return True when the function is complete
             */
            bool switch_vel(sdr_control::SwitchVelocity::Request& req, sdr_control::SwitchVelocity::Response& res);
    };
}

#endif