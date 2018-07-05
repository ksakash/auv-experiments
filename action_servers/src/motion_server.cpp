#include <ros/ros.h>
#include <angle_PID_server.h>
#include <forward_PID_server.h>
#include <sideward_PID_server.h>
#include <upward_PID_server.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "motion");

    forwardPIDAction forward_motion("motion/forward");
    sidewardPIDAction sideward_motion("motion/sideward");
    upwardPIDAction upward_motion_vision("motion/upward/vision", "VISION");
    upwardPIDAction upward_motion_sensor("motioin/upward/sensor", "SENSOR");
    anglePIDAction turn_motion_vision("motion/turn/vision", "VISION");
    anglePIDAction turn_motion_sensor("motion/turn/sensor", "SENSOR");

    ros::spin();
    return 0;
}