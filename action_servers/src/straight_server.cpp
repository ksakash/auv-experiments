#include <straight_server.h>

moveStraight::moveStraight(double angle_, int pwm_): anglePIDClient("anglePID") {
    spin_thread = new boost::thread(boost::bind(&moveStraight::spinThread, this));
    angle = angle_;

    nh.setParam("/pwm_forward_right", pwm_);
    nh.setParam("/pwm_forward_left", pwm_);
}

moveStraight::~moveStraight() {
    spin_thread->join();
}

void moveStraight::setActive(bool status) {
    if (status == true) {
        ROS_INFO("Waiting for anglePID server to start.");
        anglePIDClient.waitForServer();

        ROS_INFO("anglePID server started, sending goal.");
        angle_PID_goal.target_angle = angle;
        anglePIDClient.sendGoal(angle_PID_goal);
    }

    if (status == false) {
        anglePIDClient.cancelGoal();
    }
}   

void moveStraight::spinThread() {
    ros::spin();
}