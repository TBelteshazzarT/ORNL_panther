#ifndef ROS_IO_HELPER_H
#define ROS_IO_HELPER_H

#include <math.h>
#include <chrono>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int64MultiArray.h>
#include <std_msgs/Byte.h>
#include <tf/transform_datatypes.h>

class rosIOHelper
{
public:
    
    rosIOHelper(ros::NodeHandle& node_in);
    ~rosIOHelper();
    // Stores velocity command and returns true when the value has been updated
    bool grab_velocity(geometry_msgs::Twist &out);
    // Publish battery voltage polled from the motor controller
    void publish_batt(float voltage, float low_voltage);
    // Publish current polled from the motor controller
    void publish_current(float current);
    // Publish raw encoder counts
    void publish_encoder_counts(int64_t fr, int64_t fl, int64_t rr, int64_t rl);
    void publish_encoder_counts(int64_t right, int64_t left);
    // Computes odometry for skid steer platforms
    void publish_skid_odom(int64_t right_enc, int64_t left_enc, int32_t counts_per_rad, int32_t counts_per_meter, const float max_vel, float cov=0.01);
    // Computes odometry for mecanum platforms
    // TODO
    void publish_mec_odom();

private:

    // Calculated values
    struct ODOM_RESULTS
    {
        float x = 0;
        float y = 0;
        float yaw = 0;

        float x_vel = 0;
        float y_vel = 0;
        float yaw_vel = 0;

        float cov = 0.01;
    };
    // Skid steer odometry calculations
    struct SKID_ODOM_DATA
    {
        std::chrono::steady_clock::time_point prev_clock;
        int64_t prev_right_enc = 0;
        int64_t prev_left_enc = 0;

        ODOM_RESULTS results;
    };
    // Mecanum odometry calculations
    struct MEC_ODOM_DATA
    {
        ros::Time prev_stamp = prev_stamp.fromSec(0);
        int64_t prev_fr_enc = 0;
        int64_t prev_fl_enc = 0;
        int64_t prev_rr_enc = 0;
        int64_t prev_rl_enc = 0;

        ODOM_RESULTS results;
    };
    // ROS node handle instance
    ros::NodeHandle n;
    // ROS subscribers
    ros::Subscriber cmdVelSub, halfSpeedSub, eStopSub;
    geometry_msgs::Twist cmd_vel;
    std_msgs::Byte prev_half_speed;
    bool new_cmd = false;
    bool half_speed_enabled = false;
    bool eStop = false;
    // ROS publishers
    ros::Publisher wheelPub, battPub, ampPub, encPub;
    // Callbacks
    void cmdVel_callback(const geometry_msgs::Twist::ConstPtr& input);
    void halfSpeed_callback(const std_msgs::Byte::ConstPtr& input);
    void eStop_callback(const std_msgs::Byte::ConstPtr& input);
    // Publishes odom calculation results as a ROS message
    void publish_odom(ODOM_RESULTS odom, ros::Time time);
    // Checks for bad incoming encoder data
    bool check_counts(int64_t dc, float dt, int32_t counts_per_meter, const float max_vel);
};

#endif