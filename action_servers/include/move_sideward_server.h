#include <ros/ros.h>
#include <actionlib/server/simple_action_client.h>
#include <angle_PID_server.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <string>
#include <boost/thread.hpp>
#include <actionlib/client/terminal_state.h>

class moveSideward {

protected:

    ros::NodeHandle nh;
    actionlib::SimpleActionClient<action_servers::anglePIDAction> anglePIDClient;
    
    action_servers::anglePIDGoal angle_PID_goal;

    double angle;

public:

    moveSideward(double, int);
    ~moveSideward();

    void setActive(bool);
    void spinThread();
}