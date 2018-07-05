#include <move_forward_server.h>

moveForward::moveForward(double angle_, int pwm_): upwardPIDClient("upwardPID"), anglePIDClient("anglePID"), sidewardPIDClient("sidewardPID") {
    spin_thread = new boost::thread(boost::bind(&moveForward::spinThread, this));
    angle = angle_;

    nh.setParam("/pwm_forward_right", pwm_);
    nh.setParam("/pwm_forward_left", pwm_);
}

moveForward::~moveForward() {
    spin_thread->join();
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
        upward_PID_goal.target_depth = 0;
        upwardPIDClient.sendGoal(upward_PID_goal);

        ROS_INFO("Waiting for sidewardPID server to start.");
        anglePIDClient.waitForServer();

        ROS_INFO("sidewardPID server started, sending goal.");
        angle_PID_goal.target_angle = angle;
        anglePIDClient.sendGoal(angle_PID_goal);
    }

    if (status == false) {
        sidewardPIDClient.cancelGoal();
        upwardPIDClient.cancelGoal();
        anglePIDClient.cancelGoal();
    }
}   

void moveForward::spinThread() {
    ros::spin();
}