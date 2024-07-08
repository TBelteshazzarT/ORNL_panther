#include <math.h>
#include <iostream>
#include <string.h>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/BatteryState.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <nav_msgs/Odometry.h>
#include <diagnostic_msgs/DiagnosticStatus.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_broadcaster.h>

float x,y,th,vx,vy,vth;
const double pi = 3.14159265;
const double two_pi = 6.283185307;

void vel_call(const geometry_msgs::Twist& msg)
{
	x = msg.linear.x;
	y = msg.linear.y;
	th = msg.linear.z;
	vx = msg.angular.x;
	vy = msg.angular.y;
	vth = msg.angular.z;
}

/* void publish_joints(ros::Publisher joints_pub, double dt, ros::Time t)
{
	sensor_msgs::JointState joints;
	// Set header
	joints.header.stamp = t;
	joints.name.push_back("right_wheel_joint");
	joints.name.push_back("left_wheel_joint");
	// Calculate wheel velocity in rad/s
	joints.velocity.push_back(-1 * qpps.right * (two_pi/p.ppr));
	joints.velocity.push_back(qpps.left * (two_pi/p.ppr));
	// Calculate wheel rotation in rad
	rot.right += -1.0 * qpps.right * dt * (two_pi/p.ppr);
	rot.left += qpps.left * dt * (two_pi/p.ppr);
	joints.position.push_back(rot.right);
	joints.position.push_back(rot.left);
	// Publish
	joints_pub.publish(joints);
} */


int main(int argc, char** argv){
   ros::init(argc, argv, "robosim_driver");
 
   ros::NodeHandle n;
   ros::Subscriber sub = n.subscribe("cmd_vel", 1000, &vel_call);
   ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
   tf::TransformBroadcaster odom_broadcaster;
 
   ros::Time current_time, last_time;
   current_time = ros::Time::now();
   last_time = ros::Time::now();
 
   ros::Rate r(1.0);
   while(n.ok()){
 
     ros::spinOnce();               // check for incoming messages
     current_time = ros::Time::now();
 
     //compute odometry in a typical way given the velocities of the robot
     double dt = (current_time - last_time).toSec();
     double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
     double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
     double delta_th = vth * dt;
 
     x += delta_x;
     y += delta_y;
     th += delta_th;
 
     //since all odometry is 6DOF we'll need a quaternion created from yaw
     geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);
 
     //first, we'll publish the transform over tf
     geometry_msgs::TransformStamped odom_trans;
     odom_trans.header.stamp = current_time;
     odom_trans.header.frame_id = "odom";
     odom_trans.child_frame_id = "base_link";
 
     odom_trans.transform.translation.x = x;
     odom_trans.transform.translation.y = y;
     odom_trans.transform.translation.z = 0.0;
     odom_trans.transform.rotation = odom_quat;
 
     //send the transform
     odom_broadcaster.sendTransform(odom_trans);
 
     //next, we'll publish the odometry message over ROS
     nav_msgs::Odometry odom;
     odom.header.stamp = current_time;
     odom.header.frame_id = "odom";
 
     //set the position
     odom.pose.pose.position.x = x;
     odom.pose.pose.position.y = y;
     odom.pose.pose.position.z = 0.0;
     odom.pose.pose.orientation = odom_quat;
 
     //set the velocity
     odom.child_frame_id = "base_link";
     odom.twist.twist.linear.x = vx;
     odom.twist.twist.linear.y = vy;
     odom.twist.twist.angular.z = vth;
 
     //publish the message
     odom_pub.publish(odom);
 
     last_time = current_time;
     r.sleep();
   }
 }