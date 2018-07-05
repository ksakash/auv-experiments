#include <move_sideward_server.h>

moveSideward::moveSideward(double angle_, int pwm_): anglePIDClient("anglePID") {
    spin_thread = new boost::thread(boost::bind(&moveSideward::spinThread, this));
    
    nh.setParam("/pwm_sideward_front_straight", pwm_);
    nh.setParam("/pwm_sideward_back_straight", pwm_);

    angle = angle_;
}

moveSideward::~moveSideward() {
    spin_thread->join();
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
        anglePIDClient.cancelGoal();
    }
}   

void moveSideward::spinThread() {
    ros::spin();
}