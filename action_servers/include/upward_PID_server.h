#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <action_servers/upwardPIDAction.h>
#include <errorToPWM.h>
#include <geometry_msgs/PointStamped.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <string>
#include "std_msgs/Float64MultiArray.h"

class upwardPIDAction
{
protected:

    ros::NodeHandle nh_;
    actionlib::SimpleActionServer<action_servers::upwardPIDAction> as_; // NodeHandle instance must be created before this line. Otherwise strange error occurs.
    std::string action_name_;
    // create messages that are used to published feedback/result
    action_servers::upwardPIDFeedback feedback_;
    action_servers::upwardPIDResult result_;
    double goal_;
    ros::Subscriber sub_;
    ros::Publisher upwardFrontPublisher;
    ros::Publisher upwardBackPublisher;
    ErrorDescriptor z_coord; // z_coord for forward direction

    std_msgs::Int32 pwm_upward_front;
    std_msgs::Int32 pwm_upward_back;
    std::string type;

public:

    upwardPIDAction(std::string, std::string);
    ~upwardPIDAction();
    void goalCB();
    void preemptCB();
    void sensorCB(const std_msgs::Float32ConstPtr&);
    void visionCB(const geometry_msgs::PointStampedConstPtr&);

};
