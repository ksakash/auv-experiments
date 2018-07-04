#include <straight_server.h>

moveStraight::moveStraight(double angle_) {
    boost::thread spin_thread(&spinThread);
    angle = angle_;
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

        ROS_INFO("Waiting for sidewardPID server to start.");
        sidewardPIDClient.waitForServer();

        ROS_INFO("sidewardPID server started, sending goal.");
        sideward_PID_goal.target_distance = std::numeric_limits<float>::infinity();
        sidewardPIDClient.sendGoal(sideward_PID_goal);
    }

    if (status == false) {
        anglePIDClient.cancel_goal();
        sidewardPIDClient.cancel_goal();
    }
}   

void moveStraight::spinThread() {
    ros::spin();
}