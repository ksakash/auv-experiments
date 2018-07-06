#include <ros/ros.h>
#include <buoy.h>

double current_angle = 0;

void imuDataCB(const std_msgs::Float64Ptr &_msg) {
    current_angle = _msg->data;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "master");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("/varun/sensors/imu/yaw", 1000, &imuDataCB);
    buoy buoy_(current_angle);
    ros::spin();
    return 0;
}