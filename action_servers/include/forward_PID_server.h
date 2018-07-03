#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <action_servers/forwardPIDAction.h>
#include <errorToPWM.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <string>
#include "std_msgs/Float64MultiArray.h"

class forwardPIDAction
{
protected:

    ros::NodeHandle nh_;
    actionlib::SimpleActionServer<action_servers::forwardPIDAction> as_; // NodeHandle instance must be created before this line. Otherwise strange error occurs.
    std::string action_name_;
    // create messages that are used to published feedback/result
    action_servers::forwardPIDFeedback feedback_;
    action_servers::forwardPIDResult result_;
    double goal_;
    ros::Subscriber sub_;
    ros::Publisher forwardLeftPublisher;
    ros::Publisher forwardRightPublisher;
    ErrorDescriptor x_coord;

    std_msgs::Int32 pwm_forward_left;
    std_msgs::Int32 pwm_forward_right;
    std::string type;

public:

    forwardPIDAction(std::string, std::string);
    ~forwardPIDAction();
    void goalCB();
    void preemptCB();
    void visionCB(const geometry_msgs::PointStampedConstPtr&);

};
