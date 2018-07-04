#include <ros/ros.h>
#include <actionlib/server/simple_action_client.h>
#include <sideward_PID_server.h>
#include <upward_PID_server.h>
#include <angle_PID_server.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <string>

class moveForward {

protected:

    ros::NodeHandle nh;
    actionlib::SimpleActionClient<action_servers::sidewardPIDAction> sidewardPIDClient;
    actionlib::SimpleActionClient<action_servers::upwardPIDAction> upwardPIDClient;
    actionlib::SimpleActionClient<action_servers::anglePIDAction> anglePIDClient;
    
    action_servers::sidewardPIDGoal sideward_PID_goal;
    action_servers::upwardPIDGoal upward_PID_goal;
    action_servers::anglePIDGoal angle_PID_goal;

    double angle;

public:

    moveForward(double);
    ~moveForward();

    void setActive(bool);
    void spinThread();
}