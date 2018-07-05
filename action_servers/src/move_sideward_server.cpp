#include <move_sideward_server.h>

moveSideward::moveSideward(double angle_, int pwm_) {
    boost::thread spin_thread(&spinThread);
    angle = angle_;

    sidewardFrontPublisher = nh.advertise<std_msgs::Int32>("/pwm/sidewardFront", 1000);
    sidewardBackPublisher = nh.advertise<std_msgs::Int32>("/pwm/sidewardBack", 1000);

    pwm_sideward_back.data = pwm_;
    pwm_sideward_front.data = pwm_;
}

moveSideward::~moveSideward() {
    spin_thread().join();
}

void moveSideward::setActive(bool status_) {
    status = status_;
    if (status == true) {
        spin_thread(&moveSideward::spinThread);
        ROS_INFO("Waiting for sidewardPID server to start.");
        anglePIDClient.waitForServer();

        ROS_INFO("sidewardPID server started, sending goal.");
        angle_PID_goal.target_angle = angle;
        anglePIDClient.sendGoal(angle_PID_goal);
    }

    if (status == false) {
        anglePIDClient.cancel_goal();
    }
}   

void moveSideward::spinThread() {
    sidewardFrontPublisher.publish(pwm_sideward_front);
    sidewardBackPublisher.publish(pwm_forward_left);
    ros::spin();
}