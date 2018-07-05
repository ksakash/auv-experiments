#include <straight_server.h>

moveStraight::moveStraight(double angle_, int pwm_) {
    boost::thread spin_thread(&spinThread);
    angle = angle_;

    forwardRightPublisher = nh.advertise<std_msgs::Int32>("/pwm/forwardRight", 1000);
    forwardLeftPublisher = nh.advertise<std_msgs::Int32>("/pwm/forwardLeft", 1000);

    pwm_forward_left.data = pwm_;
    pwm_forward_right.data = pwm_;
}

moveStraight::~moveStraight() {
    spin_thread().join();
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
        anglePIDClient.cancel_goal();
    }
}   

void moveStraight::spinThread() {
    forwardLeftPublisher.publish(pwm_forward_left);
    forwardRightPublisher.publish(pwm_forward_right);
    ros::spin();
}