#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <angle_PID_server.h>
#include <sideward_PID_server.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <string>
#include <limits>
#include <boost/thread.hpp>
#include <actionlib/client/terminal_state.h>

class moveStraight {

protected:

    ros::NodeHandle nh;
    actionlib::SimpleActionClient<action_servers::anglePIDAction> anglePIDClient;
    actionlib::SimpleActionClient<action_servers::sidewardPIDAction> sidewardPIDClient;
    
    action_servers::anglePIDGoal angle_PID_goal;
    action_servers::sidewardPIDGoal sideward_PID_goal;

    double angle;

public:

    moveStraight(double);
    ~moveStraight();

    void setActive(bool);
    void spinThread();
}