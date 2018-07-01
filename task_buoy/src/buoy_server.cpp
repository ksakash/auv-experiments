#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <task_buoy/buoyAction.h>
#include <errorToPWM.h>
#include <geometry_msgs/PointStamped.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <string>
#include "std_msgs/Float64MultiArray.h"

class buoyAction
{
protected:

    ros::NodeHandle nh_;
    actionlib::SimpleActionServer<task_buoy::buoyAction> as_; // NodeHandle instance must be created before this line. Otherwise strange error occurs.
    std::string action_name_;
    // create messages that are used to published feedback/result
    task_buoy::buoyFeedback feedback_;
    task_buoy::buoyResult result_;
    double goal_;
    ros::Subscriber sub_;
    ros::Publisher rightForwardPublisher;
    ros::Publisher leftForwardPublisher;
    ErrorDescriptor x_coord("X_COORD"); // x_coord for forward direction

    std_msgs::Int32 pwm_forward_right;
    std_msgs::Int32 pwm_forward_left;


public:

    buoyAction(std::string name) :
        as_(nh_, name, false),
        action_name_(name)
    {
        //register the goal and feeback callbacks
        as_.registerGoalCallback(boost::bind(&buoyAction::goalCB, this));
        as_.registerPreemptCallback(boost::bind(&buoyAction::preemptCB, this));
        goal_ = 0;

        x_coord.setPID(7.5, 0, 2, 10);

        //subscribe to the data topic of interest
        sub_ = nh_.subscribe("/buoy_task/buoy_coordinates", 1, &buoyAction::analysisCB, this);

        rightForwardPublisher = nh.advertise<std_msgs::Int32>("/pwm/forwardRight", 1000);
        leftForwardPublisher = nh.advertise<std_msgs::Int32>("/pwm/forwardLeft", 1000);

        as_.start();
    }

    ~buoyAction(void)
    {
    }

    void goalCB()
    {
        // helper variables
        ros::Rate r(1);
        bool success = true;

        goal_ = as_.acceptNewGoal()->target_distance;

        x_coord.setReference(goal_);

        // publish info to the console for the user
        ROS_INFO("%s: Executing, got a target of %f", action_name_.c_str(), goal_);
    }

    void preemptCB()
    {
        ROS_INFO("%s: Preempted", action_name_.c_str());
        // set the action state to preempted
        as_.setPreempted();
    }

    void analysisCB(const geometry_msgs::PointStampedConstPtr& msg)
    {
        // make sure that the action hasn't been canceled
        if (!as_.isActive())
            return;
        
        x_coord.errorToPWM(msg->point.x);

        feedback_.current_distance = msg->point.x;

        as_.publishFeedback(feedback_);

        if (msg->point.x == goal_) {
            ROS_INFO("%s: Succeeded", action_name_.c_str());
            // set the action state to succeeded
            as_.setSucceeded(result_);
        }

        pwm_forward_left.data = x_coord.getPWM();
        pwm_forward_right.data = x_coord.getPWM();

        rightForwardPublisher.publish(pwm_forward_left);
        leftForwardPublisher.publish(pwm_forward_right);

    }
};


int main(int argc, char** argv)
{
  ros::init(argc, argv, "buoy");

  buoyAction buoy("buoy");
  ros::spin();

  return 0;
}