#include <unistd.h>
#include <string>
#include <math.h>
#include <ros/ros.h>
#include <sdr_control/roboteq_interface.hpp>
#include <sdr_control/rosIOHelper.h>
#include <boost/assign/list_of.hpp>

using namespace roboteq_interface;

enum ROBOT_TYPE
{
	DIFF = 0,
	SKID_CAN = 1,
	MEC_CAN = 2
};

struct PARAMS
{
	int32_t serial_mode;
	int32_t counts_per_radian;
	int32_t counts_per_meter;
	int32_t drive_factor;
	int32_t turn_factor;
	int32_t strafe_factor;
	float timeout;
	float low_batt;
	std::string port;
	std::string robot_type_str;
	std::map<std::string, ROBOT_TYPE> robot_map = boost::assign::map_list_of
		("DIFF", DIFF)("SKID_CAN", SKID_CAN)("MEC_CAN", MEC_CAN);
	bool encoders;
	bool odom;
	bool mixed;
	int32_t max_rpm;
	float gear_ratio;
	float accel_lin_x;
	float accel_lin_y;
	float accel_ang_z;
	float decel_lin_x;
	float decel_lin_y;
	float decel_ang_z;
	float wheel_diameter;
	float wheel_separation;
	float wheel_separation_multiplier;
};

struct ENCODERS
{
	uint8_t results = 0;

	int64_t right = 0;
	int64_t left = 0;

	int64_t fr = 0;
	int64_t fl = 0;
	int64_t rr = 0;
	int64_t rl = 0;
};

PARAMS set_params(ros::NodeHandle n)
{
	PARAMS p;
	n.param<std::string>("roboteq_driver/port",p.port,"/dev/ttyACM0");			// Serial port
	n.param("roboteq_driver/serial_mode",p.serial_mode,0);						// RoboteQ defined serial mode
	n.param<float>("roboteq_driver/low_batt",p.low_batt,23.47);					// Low battery level
	n.param("roboteq_driver/counts_per_radian",p.counts_per_radian,100);		// encoder counts per radian
	n.param("roboteq_driver/counts_per_meter",p.counts_per_meter,1000);			// encoder counts per meter
	n.param("roboteq_driver/drive_factor",p.drive_factor,1000);					// multiplier for linear x velocity to roboteq cmd
	n.param("roboteq_driver/turn_factor",p.turn_factor,500);					// multiplier for angular z velocity to roboteq cmd
	n.param("roboteq_driver/strafe_factor",p.strafe_factor,1000);				// multiplier for linear y velocity to roboteq cmd	
	n.param<float>("roboteq_driver/timeout",p.timeout,1.5F);					// Timeout failsafe
	n.param<std::string>("roboteq_driver/robot_type",p.robot_type_str,"DIFF");	// Platform type
	n.param<bool>("roboteq_driver/encoders_enabled",p.encoders,false);			// encoder feedback option
	n.param<bool>("roboteq_driver/publish_odom",p.odom,false);					// publish odometry from encoder feedback
	n.param<bool>("roboteq_driver/ros_mixing",p.mixed,true);					// RoboteQ mixing (false) or ROS mixing (true)
	n.param("roboteq_driver/max_rpm",p.max_rpm,120);							// RPM of the wheels
	n.param<float>("roboteq_driver/gear_ratio",p.gear_ratio,1);
	n.param<float>("roboteq_driver/accel_lin_x",p.accel_lin_x,2.0);				// Acceleration and deceleration limits in m/s^2 and rad/s^2
	n.param<float>("roboteq_driver/accel_lin_y",p.accel_lin_y,2.0);
	n.param<float>("roboteq_driver/accel_ang_z",p.accel_ang_z,2.0);
	n.param<float>("roboteq_driver/decel_lin_x",p.decel_lin_x,2.0);
	n.param<float>("roboteq_driver/decel_lin_y",p.decel_lin_y,2.0);
	n.param<float>("roboteq_driver/decel_ang_z",p.decel_ang_z,2.0);
	n.param<float>("roboteq_driver/wheel_diameter",p.wheel_diameter,0.3302);	// Wheel diameter in meters
	n.param<float>("roboteq_driver/wheel_separation",p.wheel_separation,0.5588); // Wheel separation in meters
	n.param<float>("roboteq_driver/wheel_separation_multiplier",p.wheel_separation_multiplier,1.0);

	return p;
}

void reset_encoders(RoboteqInterface *r, PARAMS p)
{
	switch(p.robot_map[p.robot_type_str])
	{
		case DIFF:
			r->reset_encoders();
			break;

		case SKID_CAN:
			r->can_reset_encoders();
			break;

		case MEC_CAN:
			r->can_reset_encoders();
			break;
	}
}

void set_open(RoboteqInterface *r, PARAMS p)
{
	switch(p.robot_map[p.robot_type_str])
	{
		case DIFF:
			r->set_open();
			break;

		case SKID_CAN:
			r->can_set_open();
			break;

		case MEC_CAN:
			r->can_set_open();
			break;
	}
}

void mix(float drive, float turn, float &ch1, float &ch2, int max_rpm, bool speed)
{
	float max = speed ? max_rpm : 1000;
	ch1 = drive + turn;
	ch1 = ch1 > max ? max : (ch1 < -1*max ? -1*max : ch1);
	ch2 = drive - turn;
	ch2 = ch2 > max ? max : (ch2 < -1*max ? -1*max : ch2);
}

float apply_accel(float cmd, float prev_vel, float cycle_time, float accel)
{
	float cmd_change = cmd - prev_vel;
	float accel_change = accel * cycle_time;
	float min_change = std::min<float>(fabs(cmd_change), accel_change);

	float vel = cmd_change > 0 ? prev_vel + min_change : (cmd_change < 0 ? prev_vel - min_change: prev_vel);

	return vel;
}

float apply_accel_decel(float cmd, float prev_vel, float cycle_time, float accel, float decel)
{
	float cmd_change = cmd - prev_vel;
	float accel_change = accel * cycle_time;
	float decel_change = decel * cycle_time;
	float min_change = 0;
	float vel = prev_vel;

	if((cmd_change > 0) && (prev_vel >= 0))
	{
		min_change = std::min<float>(cmd_change, accel_change);
		vel = prev_vel + min_change;
		//std::cout << "accel : " << min_change << std::endl;
	}
	else if((cmd_change > 0) && (prev_vel < 0))
	{
		min_change = std::min<float>(cmd_change, decel_change);
		vel = prev_vel + min_change;
		//std::cout << "decel : " << min_change << std::endl;
	}
	else if((cmd_change < 0) && (prev_vel > 0))
	{
		min_change = std::min<float>(fabs(cmd_change), decel_change);
		vel = prev_vel - min_change;
		//std::cout << "decel : -" << min_change << std::endl;
	}
	else if((cmd_change < 0) && (prev_vel <= 0))
	{
		min_change = std::min<float>(fabs(cmd_change), accel_change);
		vel = prev_vel - min_change;
		//std::cout << "accel : -" << min_change << std::endl;
	}

	return vel;
}

void cmd_motors(RoboteqInterface *r, geometry_msgs::Twist cmd, geometry_msgs::Twist &prev_cmd, float time, PARAMS p)
{
	float ch1 = 0.0;
	float ch2 = 0.0;
	geometry_msgs::Twist vel;
	vel.linear.x = apply_accel_decel(cmd.linear.x, prev_cmd.linear.x, time, p.accel_lin_x, p.decel_lin_x);
	vel.linear.y = apply_accel_decel(cmd.linear.y, prev_cmd.linear.y, time, p.accel_lin_y, p.decel_lin_y);
	vel.angular.z = apply_accel_decel(cmd.angular.z, prev_cmd.angular.z, time, p.accel_ang_z, p.decel_ang_z);
	float drive = p.encoders ? ((60.0*vel.linear.x)/(p.wheel_diameter*M_PI))/p.gear_ratio : p.drive_factor*cmd.linear.x;
	float turn = p.encoders ? ((60.0*vel.angular.z*p.wheel_separation*p.wheel_separation_multiplier/2)/(p.wheel_diameter*M_PI))/p.gear_ratio : p.turn_factor*cmd.angular.z;
	if(p.mixed) { mix(drive, turn, ch1, ch2, p.max_rpm, p.encoders); }
	else
	{
		ch1 = drive;
		ch2 = turn;
	}
	//std::cout << "ch1: " << ch1 << " ch2: " << ch2 << " lin vel: " << vel.linear.x << " lin ang: " << vel.angular.z << std::endl;
	switch(p.robot_map[p.robot_type_str])
	{
		case DIFF:
			r->diff_cmd(ch1, ch2, p.encoders);
			break;
		
		case SKID_CAN:
			r->skid_can_cmd(ch1, ch2, p.encoders);
			break;

		case MEC_CAN:
			// TODO
			r->mecanum_can_cmd(cmd.linear.x, cmd.angular.z, cmd.linear.y);
			break;
	}
	prev_cmd = vel;
}

void publish_encoders(rosIOHelper io, PARAMS p, ENCODERS enc, const float max_vel)
{
	// Clear results variable
	enc.results = 0;
	// Step through switch to read encoders and publish data
	switch(p.robot_map[p.robot_type_str])
	{
		case DIFF:
			io.publish_encoder_counts(enc.right, enc.left);
			if(p.odom) { io.publish_skid_odom(enc.right, enc.left, p.counts_per_radian, p.counts_per_meter, max_vel); }
			break;

		case SKID_CAN:
			io.publish_encoder_counts(enc.right, enc.left);
			if(p.odom) { io.publish_skid_odom(enc.right, enc.left, p.counts_per_radian, p.counts_per_meter, max_vel); }
			break;

		case MEC_CAN:
			io.publish_encoder_counts(enc.fr, enc.fl, enc.rr, enc.rl);
			if(p.odom) { io.publish_mec_odom(); }		// TODO
			break;
	}
}

int main(int argc, char **argv)
{
	// ROS Init
	ros::init(argc,argv,"roboteq_driver");
	ros::NodeHandle n;
	const float rate = 20.0;
	ros::Rate r(rate);
	// ROS Params
	PARAMS p = set_params(n);
	// Publisher and subscriber helper
	rosIOHelper ros_io(n);
	geometry_msgs::Twist cmd_vel;
	geometry_msgs::Twist prev_vel;
	geometry_msgs::Twist zero_vel = {};
	ros::Time last_cmd_stamp;
	float volts = 0;
	float amps = 0;
	// RoboteQ Init
	ENCODERS enc;
	RoboteqInterface *rbtq = new RoboteqInterface(p.port, 115200);
	if(p.encoders) { reset_encoders(rbtq, p); }
	else { set_open(rbtq, p); }
	// Calculate max wheel speed
	const float max_wheel_vel = p.wheel_diameter*M_PI*p.max_rpm/60.0;
	
	// Main loop
	while(ros::ok())
	{
		ros::spinOnce();
		// Check velocity commands
		if(ros_io.grab_velocity(cmd_vel))
		{
			// set timer and control motors
			last_cmd_stamp = ros::Time::now();
			cmd_motors(rbtq, cmd_vel, prev_vel, 1.0/rate, p);
		}
		else if((ros::Time::now()-last_cmd_stamp).toSec() >= p.timeout)
		{
			// stop motors
			cmd_motors(rbtq, zero_vel, prev_vel, 1.0/rate, p);
		}

		// Query script packet and publish values
		if(rbtq->poll_packet(volts, amps, enc.right, enc.left))
		{
			if(p.encoders) { publish_encoders(ros_io, p, enc, max_wheel_vel); }
			ros_io.publish_batt(volts, p.low_batt);
			ros_io.publish_current(amps);
		}
		else { ROS_WARN("RoboteQ read failed!"); }
		
		r.sleep();
	}
	return 1;
}
