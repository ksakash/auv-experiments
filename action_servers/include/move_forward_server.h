#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <action_servers/anglePIDAction.h>
#include <action_servers/upwardPIDAction.h>
#include <action_servers/sidewardPIDAction.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <string>
#include <boost/thread.hpp>

class moveForward {

protected:

    ros::NodeHandle nh;
    actionlib::SimpleActionClient<action_servers::upwardPIDAction> upwardPIDClient;
    actionlib::SimpleActionClient<action_servers::anglePIDAction> anglePIDClient;
    actionlib::SimpleActionClient<action_servers::sidewardPIDAction> sidewardPIDClient;

    action_servers::sidewardPIDGoal sideward_PID_goal;
    action_servers::upwardPIDGoal upward_PID_goal;
    action_servers::anglePIDGoal angle_PID_goal;

    ros::Publisher forwardRightPublisher;
    ros::Publisher forwardLeftPublisher;

    std_msgs::Int32 pwm_forward_left;
    std_msgs::Int32 pwm_forward_right;

    double angle;
    boost::thread* spin_thread;

public:

    moveForward(double, int pwm_);
    ~moveForward();

    void setActive(bool);
    void spinThread();
};