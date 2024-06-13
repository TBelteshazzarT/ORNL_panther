#include <sdr_control/rosIOHelper.h>

// Public

rosIOHelper::rosIOHelper(ros::NodeHandle& node_in)
    : n(node_in)
{
    // Setup subscribers
    cmdVelSub = n.subscribe<geometry_msgs::Twist>("/cmd_vel", 5, &rosIOHelper::cmdVel_callback, this);
    halfSpeedSub = n.subscribe<std_msgs::Byte>("/half_speed", 5, &rosIOHelper::halfSpeed_callback, this);
    eStopSub = n.subscribe<std_msgs::Byte>("/eStop", 5, &rosIOHelper::eStop_callback, this);
    // Setup publishers
    encPub = n.advertise<std_msgs::Int64MultiArray>("/encoder_counts", 40);
    wheelPub = n.advertise<nav_msgs::Odometry>("/wheel_odom", 40);
    battPub = n.advertise<std_msgs::Float32>("/battery_state", 40);
    ampPub = n.advertise<std_msgs::Float32>("/motor_amps", 40);
}

rosIOHelper::~rosIOHelper() {}

bool rosIOHelper::grab_velocity(geometry_msgs::Twist &out)
{
    if(new_cmd)
    {
        out = cmd_vel;
        if(eStop)
        {
            out.linear.x = 0.0;
            out.linear.y = 0.0;
            out.linear.z = 0.0;
            out.angular.x = 0.0;
            out.angular.y = 0.0;
            out.angular.z = 0.0;
        }
        else if(half_speed_enabled)
        {
            out.linear.x *= 0.5;
            out.linear.y *= 0.5;
            out.linear.z *= 0.5;
            out.angular.x *= 0.5;
            out.angular.y *= 0.5;
            out.angular.z *= 0.5;
        }
        new_cmd = false;
        return true;
    }
    else { return false; }
}

void rosIOHelper::publish_batt(float voltage, float low_voltage)
{
    std_msgs::Float32 batt_msg;
    batt_msg.data = voltage;
    battPub.publish(batt_msg);
    if((voltage<=low_voltage)&&(voltage>0)) { ROS_WARN("LOW BATTERY: %f", voltage); }
}

void rosIOHelper::publish_current(float current)
{
    std_msgs::Float32 current_msg;
    current_msg.data = current;
    ampPub.publish(current_msg);
}

void rosIOHelper::publish_encoder_counts(int64_t fr, int64_t fl, int64_t rr, int64_t rl)
{
    // Setup array
    std_msgs::Int64MultiArray encoder_msg;
    encoder_msg.layout.dim.push_back(std_msgs::MultiArrayDimension());
    encoder_msg.layout.dim[0].size = 4;
    encoder_msg.layout.dim[0].stride = 1;
    encoder_msg.layout.dim[0].label = "Raw encoder data [fr, fl, rr, fl]";
    encoder_msg.data.resize(4);
    // Store data
    encoder_msg.data[0] = fr;
    encoder_msg.data[1] = fl;
    encoder_msg.data[2] = rr;
    encoder_msg.data[3] = rl;
    // Publish data
    encPub.publish(encoder_msg);
}

void rosIOHelper::publish_encoder_counts(int64_t right, int64_t left)
{
    // Setup array
    std_msgs::Int64MultiArray encoder_msg;
    encoder_msg.layout.dim.push_back(std_msgs::MultiArrayDimension());
    encoder_msg.layout.dim[0].size = 2;
    encoder_msg.layout.dim[0].stride = 1;
    encoder_msg.layout.dim[0].label = "Raw encoder data [right, left]";
    encoder_msg.data.resize(2);
    // Store data
    encoder_msg.data[0] = right;
    encoder_msg.data[1] = left;
    // Publish data
    encPub.publish(encoder_msg);
}

void rosIOHelper::publish_skid_odom(int64_t right_enc, int64_t left_enc, int32_t counts_per_rad, int32_t counts_per_meter, const float max_vel, float cov)
{
    // Grab time stamp
    ros::Time stamp = ros::Time::now();
    std::chrono::steady_clock::time_point clock = std::chrono::steady_clock::now();
    // Set static struct
    static SKID_ODOM_DATA sod;
    // Calculate time delta
    float dt = std::chrono::duration<float>(clock - sod.prev_clock).count();
    // Calculate encoder delta
    int64_t r_delta = right_enc - sod.prev_right_enc;
    int64_t l_delta = left_enc - sod.prev_left_enc;
    // Populate previous vars
    sod.prev_clock = clock;   
    sod.prev_left_enc = left_enc;
    sod.prev_right_enc = right_enc;
    // Check for jumps in encoder data
    bool r_check = check_counts(r_delta, dt, counts_per_meter, max_vel);
    bool l_check = check_counts(l_delta, dt, counts_per_meter, max_vel);
    if(!r_check || !l_check) 
    { 
        ROS_WARN("Encoder jump detected");
        return; 
    }

    // Calculate velocities
    sod.results.x_vel = ((float)(r_delta+l_delta)/2.0F)/(counts_per_meter*dt);
    sod.results.yaw_vel = ((float)(r_delta-l_delta)/2.0F)/(counts_per_rad*dt);
    
    // Calculate pose
    sod.results.yaw += sod.results.yaw_vel*dt;
    sod.results.x += sod.results.x_vel*cos(sod.results.yaw)*dt;
    sod.results.y += sod.results.x_vel*sin(sod.results.yaw)*dt;
    // Populate covariance
    sod.results.cov = cov;

    // Publish
    publish_odom(sod.results, stamp);
}

void rosIOHelper::publish_mec_odom()
{
    // TODO
}

// Private

void rosIOHelper::cmdVel_callback(const geometry_msgs::Twist::ConstPtr& input)
{
    new_cmd = true;
    cmd_vel = *input;
}

void rosIOHelper::halfSpeed_callback(const std_msgs::Byte::ConstPtr& input)
{
    if(input->data && !prev_half_speed.data) { half_speed_enabled = !half_speed_enabled; }
    prev_half_speed.data = input->data;
}

void rosIOHelper::eStop_callback(const std_msgs::Byte::ConstPtr& input)
{
    if(input->data) { eStop = true; }
    else { eStop = false; }
}

void rosIOHelper::publish_odom(ODOM_RESULTS odom, ros::Time time)
{
    int i = 0;      // For loop index
    // Create message object
    nav_msgs::Odometry odom_msg;
    // Header
    odom_msg.header.stamp = time;
    odom_msg.header.frame_id = "map";
    odom_msg.child_frame_id = "base_link";
    // Pose
    odom_msg.pose.pose.position.x = odom.x;
    odom_msg.pose.pose.position.y = odom.y;
    odom_msg.pose.pose.position.z = 0.0;
    odom_msg.pose.pose.orientation = tf::createQuaternionMsgFromYaw(odom.yaw);
    // Velocity
    odom_msg.twist.twist.linear.x = odom.x_vel;
    odom_msg.twist.twist.linear.y = odom.y_vel;
    odom_msg.twist.twist.linear.z = 0.0;
    odom_msg.twist.twist.angular.x = 0.0;
    odom_msg.twist.twist.angular.y = 0.0;
    odom_msg.twist.twist.angular.z = odom.yaw_vel;
    // Covaraiance
    for(i=0;i<36;i+=7)
    {
        odom_msg.pose.covariance[i] = odom.cov;
        odom_msg.twist.covariance[i] = odom.cov;
    }
    // Publish
    wheelPub.publish(odom_msg);
}

bool rosIOHelper::check_counts(int64_t dc, float dt, int32_t counts_per_meter, const float max_vel)
{
    int32_t max_counts = max_vel*dt*abs(counts_per_meter)*1.5;
    if(abs(dc) > max_counts) { return false; }
    else { return true; }
}