#include <move_forward_server.h>

moveForward::moveForward(double angle_, int pwm_) {
    boost::thread spin_thread(&spinThread);
    angle = angle_;

    pwm_forward_left.data = pwm_;
    pwm_forward_right.data = pwm_;

    forwardRightPublisher = nh.advertise<std_msgs::Int32>("/pwm/forwardRight", 1000);
    forwardLeftPublisher = nh.advertise<std_msgs::Int32>("/pwm/forwardLeft", 1000);
}

moveForward::~moveForward() {
    spin_thread().join();
}

void moveForward::setActive(bool status) {
    if (status == true) {
        ROS_INFO("Waiting for sidewardPID server to start.");
        sidewardPIDClient.waitForServer();

        ROS_INFO("sidewardPID server started, sending goal.");
        sideward_PID_goal.target_distance = 0;
        sidewardPIDClient.sendGoal(sideward_PID_goal);

        ROS_INFO("Waiting for sidewardPID server to start.");
        upwardPIDClient.waitForServer();

        ROS_INFO("upwardPID server started, sending goal.");
        upward_PID_goal.target_height = 0;
        upwardPIDClient.sendGoal(upward_PID_goal);

        ROS_INFO("Waiting for sidewardPID server to start.");
        anglePIDClient.waitForServer();

        ROS_INFO("sidewardPID server started, sending goal.");
        angle_PID_goal.target_angle = angle;
        anglePIDClient.sendGoal(angle_PID_goal);
    }

    if (status == false) {
        sidewardPIDClient.cancel_goal();
        upwardPIDClient.cancel_goal();
        anglePIDClient.cancel_goal();
    }
}   

void moveForward::spinThread() {
    forwardRightPublisher.publish(pwm_forward_right);
    forwardLeftPublisher.publish(pwm_forward_left);
    ros::spin();
}