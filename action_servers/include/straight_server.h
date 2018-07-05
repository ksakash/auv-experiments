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
    ros::Publisher forwardRightPublisher;
    ros::Publisher forwardLeftPublisher;

    std_msgs::Int32 pwm_forward_left;
    std_msgs::Int32 pwm_forward_right;
    
    action_servers::anglePIDGoal angle_PID_goal;

    double angle;

public:

    moveStraight(double, int);
    ~moveStraight();

    void setActive(bool);
    void spinThread();
}