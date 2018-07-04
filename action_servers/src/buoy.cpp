#include <buoy.h>

buoy::buoy() {}
buoy::~buoy() {}

void buoy::setActive(bool status) {
    buoy_.setActive(true);
    move_sideward_.setActive(true);
    ros::Duration(10).sleep();
    move_sideward_.setActive(false);
    buoy_.setActive(true);
    move_sideward_.setActive(true);
    ros::Duration(10).sleep();
}