#include <move_sideward_server.h>

moveSideward::moveSideward(double angle_) {
    boost::thread spin_thread(&spinThread);
    angle = angle_;
    ros::init("straight");
}

moveSideward::~moveSideward() {
    ros::shutdown();
    spin_thread().join();
}

void moveSideward::setActive(bool status) {
    if (status == true) {
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
    ros::spin();
}