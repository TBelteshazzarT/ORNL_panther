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
#include <sdr_control/RoboClaw.h>

const double pi = 3.14159265;
const double two_pi = 6.283185307;

// Cmd vel variables
geometry_msgs::Twist cmd_vel;
ros::Time lastVel;

// Diagnostic
diagnostic_msgs::DiagnosticStatus diagnostic_msg;
ros::Time lastDiag;

template <typename T>
struct ENC_PULSE
{
	T right = 0;
	T left = 0;
};

struct PARAMS
{
	std::string port;
	std::string right_joint_name;
	std::string left_joint_name;
	int max_qpps;
	int addr;
	int baud_rate;
	int ppr;
	int qpps_accel;
	float tire_diameter;
	int counts_per_radian;
	float pose_covariance;
	float twist_covariance;
	float timeout;
	int error_limit;
	float error_timeout;
	int max_current;
	int warn_limit;
};

// ROS Callbacks
void cmdVelCallback(const geometry_msgs::Twist::ConstPtr& input)
{
	cmd_vel.linear = input->linear;
	cmd_vel.angular = input->angular;
	lastVel = ros::Time::now();
}

void diagnosticCallback(const diagnostic_msgs::DiagnosticStatus::ConstPtr& input)
{
	diagnostic_msg = *input;
	lastDiag = ros::Time::now();
}

// ROS Publishers
void publish_odometry(ros::Publisher odom_pub, ENC_PULSE<int> speed, double* pose, float dt, PARAMS p, ros::Time t)
{
	nav_msgs::Odometry odom;
	int i = 0;
	float left_rps = (float)speed.left/(float)p.ppr;
	float right_rps = (float)speed.right/(float)p.ppr;
	float linear_vel = ((left_rps + right_rps) * (pi*p.tire_diameter)) / 2.0;
	float angular_vel = (speed.right - speed.left) / (2.0 * (float)p.counts_per_radian);
	
	pose[2] += angular_vel * dt;
	pose[0] += linear_vel * cos(pose[2]) * dt;
	pose[1] += linear_vel * sin(pose[2]) * dt;
	
	// Set header
	odom.header.stamp = t;
	odom.header.frame_id = "map";
	odom.child_frame_id = "base_link";
	// Set position
	odom.pose.pose.position.x = pose[0];
	odom.pose.pose.position.y = pose[1];
	odom.pose.pose.position.z = 0;
	odom.pose.pose.orientation = tf::createQuaternionMsgFromYaw(pose[2]);
	// Set velocity
	odom.twist.twist.linear.x = linear_vel;
	odom.twist.twist.linear.y = 0;
	odom.twist.twist.linear.z = 0;
	odom.twist.twist.angular.x = 0;
	odom.twist.twist.angular.y = 0;
	odom.twist.twist.angular.z = angular_vel;
	// Set covariance
	for(i=0;i<36;i+=7)
	{
		odom.pose.covariance[i] = p.pose_covariance;
		odom.twist.covariance[i] = p.twist_covariance;
	}
	// Publish
	odom_pub.publish(odom);
}

void publish_joints(ros::Publisher joints_pub, ENC_PULSE<int> qpps, ENC_PULSE<float>& rot, double dt, PARAMS p, ros::Time t)
{
	sensor_msgs::JointState joints;
	// Set header
	joints.header.stamp = t;
	joints.name.push_back(p.right_joint_name);
	joints.name.push_back(p.left_joint_name);
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
}

void publish_battery(ros::Publisher batt_pub, RoboClaw* rc, PARAMS p)
{
	bool valid = false;
	float value = 0;
	sensor_msgs::BatteryState batt;
	
	// Set header
	batt.header.stamp = ros::Time::now();
	// Set voltage
	value = rc->ReadMainBatteryVoltage(p.addr, &valid);
	batt.voltage = value/10.0;
	batt.present = true;
	if(!valid) 
	{ 
		value = rc->ReadMainBatteryVoltage(p.addr, &valid);
		batt.voltage = value/10.0;
		if(valid) { batt_pub.publish(batt); }
	}
	else { batt_pub.publish(batt); }
}

void publish_current(ros::Publisher current_pub, RoboClaw* rc, PARAMS p)
{
	int16_t left_motor = 0;
	int16_t right_motor = 0;
	geometry_msgs::Vector3Stamped current_msg;

	current_msg.header.stamp = ros::Time::now();
	if(rc->ReadCurrents(p.addr, left_motor, right_motor))
	{
		current_msg.vector.x = (float)left_motor/100.0;
		current_msg.vector.y = (float)right_motor/100.0;
		current_pub.publish(current_msg);
	}
}

// Read encoder data and return success or failure
bool poll_encoders(RoboClaw* rc, ENC_PULSE<int>& speed, PARAMS p, ros::Time& t)
{
	bool left_valid = false;
	bool right_valid = false;
	uint8_t left_status = 0;
	uint8_t right_status = 0;
	// Read speed
	speed.left = rc->ReadSpeedM1(p.addr, &left_status, &left_valid);
	speed.right = rc->ReadSpeedM2(p.addr, &right_status, &right_valid);
	if(!left_valid || !right_valid) { return false; }
	// Take time
	t = ros::Time::now();
	
	return true;
}

void check_heat_status(RoboClaw* rc, PARAMS p)
{
	switch(diagnostic_msg.level)
	{
		case 0:
			rc->SetM1MaxCurrent(p.addr, (p.max_current*100));
			rc->SetM2MaxCurrent(p.addr, (p.max_current*100));
			break;
		case 1:
			rc->SetM1MaxCurrent(p.addr, (p.warn_limit*100));
			rc->SetM2MaxCurrent(p.addr, (p.warn_limit*100));
			ROS_WARN_ONCE("Motor temperature is rising. Limiting current.");
			break;
		case 2:
			cmd_vel.linear.x = 0;
			cmd_vel.angular.z = 0;
			ROS_WARN_ONCE("Motor temperature has reached the critical threshold. Stopping robot.");
			break;
		default:
			break;
	}
}

int calc_error(int setpoint, int reading, int max_value)
{
	float error = fabs(setpoint-reading)/max_value;
	return (int)(error*100.0);
}

// Velocity commands to motor driver commands
void cmd_motors(RoboClaw* rc, PARAMS p, ENC_PULSE<int> qpps, float &error_timeout, float dt)
{
	geometry_msgs::Twist vel = cmd_vel;
	float turn_qpps = vel.angular.z * p.counts_per_radian;
	int32_t r_spd = (vel.linear.x * ((float)p.ppr/(p.tire_diameter*pi))) + turn_qpps;
	int32_t l_spd = (vel.linear.x * ((float)p.ppr/(p.tire_diameter*pi))) - turn_qpps;

	int r_error = 0, l_error = 0;
	int16_t pwm1 = 0, pwm2 = 0;
	float fpwm1 = 0, fpwm2 = 0;
	bool qpps_error = false, duty_error = false;
	if(rc->ReadPWMs(p.addr, pwm1, pwm2)) 
	{ 
		fpwm1 = fabs((float)pwm1/327.67);
		fpwm2 = fabs((float)pwm2/327.67);
		r_error = calc_error(r_spd, qpps.right, p.max_qpps);
		l_error = calc_error(l_spd, qpps.left, p.max_qpps);
		// std::cout << fpwm1 << "\t" << fpwm2 << "\n";
		qpps_error = (r_error > p.error_limit) || (l_error > p.error_limit);
		duty_error = (l_spd == 0 && fpwm1 > 80) || (r_spd == 0 && fpwm2 > 80);
		if(qpps_error || duty_error) { error_timeout += dt; }
		else { error_timeout = 0; }
	}
	else 
	{ 
		error_timeout += dt; 
		ROS_WARN("Failed to read MCU duty cycle");
	}

	// std::cout << l_error << "\t" << r_error << "\t" << error_timeout << "\n";
	
	if(error_timeout > p.error_timeout)
	{
		int accel_open = 32767*2;
		rc->DutyAccelM1M2(p.addr, 0, accel_open, 0, accel_open);
	}
	else if((ros::Time::now()-lastVel).toSec() >= p.timeout)
	{
		rc->SpeedAccelM1M2(p.addr, p.qpps_accel, 0, 0);
		ROS_WARN_ONCE("Cmd communication timeout");
	}
	// else if((ros::Time::now()-lastDiag).toSec() >= 5.0)
	// {
	// 	rc->SpeedAccelM1M2(p.addr, p.qpps_accel, 0, 0);
	// 	ROS_WARN("Temperature communication timeout");
	// }
	else
	{
		rc->SpeedAccelM1M2(p.addr, p.qpps_accel, l_spd, r_spd);
	}
}

PARAMS read_parameters(ros::NodeHandle n)
{
	PARAMS out;
	n.param<std::string>("port", out.port, "/dev/ttyACM0");
	n.param<std::string>("right_joint_name", out.right_joint_name, "right_wheel_joint");
	n.param<std::string>("left_joint_name", out.left_joint_name, "left_wheel_joint");
	n.param("max_qpps", out.max_qpps, 100000);
	n.param("address", out.addr, 128);
	n.param("baud_rate", out.baud_rate, 38400);
	n.param("ppr", out.ppr, 20000);
	n.param("qpps_accel", out.qpps_accel, 200000);
	n.param<float>("tire_diameter", out.tire_diameter, 0.3302);
	n.param("counts_per_radian", out.counts_per_radian, 5000);
	n.param<float>("pose_covariance", out.pose_covariance, 0.01);
	n.param<float>("twist_covariance", out.twist_covariance, 0.01);
	n.param<float>("timeout", out.timeout, 1.0);
	n.param("error_limit", out.error_limit, 50);
	n.param<float>("error_timeout", out.error_timeout, 1.0);
	n.param("max_current", out.max_current, 120);
	n.param("warn_limit", out.warn_limit, 60);
	
	return out;
} 

int main(int argc, char** argv)
{
	// ROS init
	ros::init(argc,argv,"roboclaw_driver");
	ros::NodeHandle n("~");
	const float rate = 10;
	ros::Rate r(rate);
	// ROS Subscribers
	ros::Subscriber cmdVelSub, diagnosticSub;
	cmdVelSub = n.subscribe<geometry_msgs::Twist>("/cmd_vel",10,&cmdVelCallback);
	diagnosticSub = n.subscribe<diagnostic_msgs::DiagnosticStatus>("/overheat_status",10,&diagnosticCallback);
	// ROS Publishers
	ros::Publisher odomPub, jointPub, battPub, currentPub;
	odomPub = n.advertise<nav_msgs::Odometry>("wheel_odom", 10);
	jointPub = n.advertise<sensor_msgs::JointState>("/joint_states", 10);
	battPub = n.advertise<sensor_msgs::BatteryState>("/battery", 10);
	currentPub = n.advertise<geometry_msgs::Vector3Stamped>("motor_current", 10);
	// ROS Params
	PARAMS p = read_parameters(n);
	// RoboClaw init
	RoboClaw* rc = new RoboClaw(p.port, p.baud_rate);
	// rc->ResetEncoders(p.addr);
	// Odometry calculation variables
	double pose[3] = { 0 };		// X Y Yaw
	float dt = 0;
	float error_timeout = 0;
	ENC_PULSE<int> qpps_current;
	ENC_PULSE<float> wheel_rot;
	ros::Time prev_stamp = ros::Time::now();
	ros::Time time_current = ros::Time::now();
	
	// Main Loop
	while(ros::ok())
	{		
		ros::spinOnce();
		
		dt = (ros::Time::now() - prev_stamp).toSec();
		prev_stamp = ros::Time::now();

		// check_heat_status(rc, p);
		cmd_motors(rc, p, qpps_current, error_timeout, dt);
		if(poll_encoders(rc, qpps_current, p, time_current))
		{
			publish_joints(jointPub, qpps_current, wheel_rot, dt, p, time_current);
			publish_odometry(odomPub, qpps_current, pose, dt, p, time_current);
		}
		else { ROS_WARN("Invalid encoder read"); }
		
		publish_battery(battPub, rc, p);
		// publish_current(currentPub, rc, p);
		
		r.sleep();
	}
	return 0;
}
