#include <single_buoy.h>

singleBuoy::singleBuoy() {
    sub_ = nh_.subscribe("/buoy_task/buoy_coordinates", 1, &singleBuoy::fowardCB, this);
}
singleBuoy::~singleBuoy() {}

singleBuoy::setActive(bool status) {
    move_forward_.setActive(true);

    while(forward_distance_ >= 60) {
        continue;
    }
    move_forward_.setActive(false);

    move_straight_.setActive(true);
    ros::Duration(6).sleep();
    move_straight_.setActive(false);
    move_straight_.setActive(true);
    ros::Duration(10).sleep();
    move_straight_.setActive(false);

    ROS_INFO("Waiting for action server to start.");
    ac.waitForServer();

    ROS_INFO("Action server started, sending goal.");
    // send a goal to the action
    action_servers::forwardPIDGoal goal;
    goal.forward_distance = 100;
    forwardPIDClient.sendGoal(goal);

    bool finished_before_timeout = forwardPIDClient.waitForResult(ros::Duration(15.0));
    
    if (finished_before_timeout)
    {
        actionlib::SimpleClientGoalState state = forwardPIDClient.getState();
        ROS_INFO("Action finished: %s",state.toString().c_str());
    }
    else
        ROS_INFO("Action did not finish before the time out.");

}

singleBuoy::forwardCB(geometry_msgs::PointStampedConstPtr &_msg) {
    forward_distance_ = _msg->point.x;
}