#include <buoy.h>

buoy::buoy(double angle): move_sideward_(angle, 100), buoy_(angle) {}
buoy::~buoy() {}

void buoy::setActive(bool status) {
    if (status == true) {
        buoy_.setActive(true);
        move_sideward_.setActive(true);
        ros::Duration(10).sleep();
        move_sideward_.setActive(false);
        buoy_.setActive(true);
        move_sideward_.setActive(true);
        ros::Duration(10).sleep();
    }

    else if (status == false) {
        buoy_.setActive(false);
        move_sideward_.setActive(false);
    }
}