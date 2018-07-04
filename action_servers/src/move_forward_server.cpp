#include <move_forward_server.h>

moveForward::moveForward(double angle_) {
    boost::thread spin_thread(&spinThread);
    angle = angle_;
    ros::init("moveForward");
}

moveForward::~moveForward() {
    ros::shutdown();
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
    ros::spin();
}