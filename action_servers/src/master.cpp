#include <ros/ros.h>
#include <buoy.h>
#include <straight_server.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "master");
    ros::NodeHandle nh;
    buoy buoy_;
    buoy_.setActive(true);
    ros::spin();
    return 0;
}