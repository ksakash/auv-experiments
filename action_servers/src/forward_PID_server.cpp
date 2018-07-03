#include <forward_PID_server.h>

forwardPIDAction::forwardPIDAction(std::string name, std::string type_) :
    as_(nh_, name, false),
    action_name_(name), x_coord("X_COORD")
{
    //register the goal and feeback callbacks
    as_.registerGoalCallback(boost::bind(&forwardPIDAction::goalCB, this));
    as_.registerPreemptCallback(boost::bind(&forwardPIDAction::preemptCB, this));
    goal_ = 0;

    type = type_;
    x_coord.setPID(7.5, 0, 2, 10);
    
    //subscribe to the data topic of interest
    sub_ = nh_.subscribe("/buoy_task/buoy_coordinates", 1, &forwardPIDAction::visionCB, this);

    forwardRightPublisher = nh_.advertise<std_msgs::Int32>("/pwm/forwardRight", 1000);
    forwardLeftPublisher = nh_.advertise<std_msgs::Int32>("/pwm/forwardLeft", 1000);

    as_.start();
}

forwardPIDAction::~forwardPIDAction(void)
{
}

void forwardPIDAction::goalCB()
{
    // helper variables
    ros::Rate r(1);
    bool success = true;

    goal_ = as_.acceptNewGoal()->target_distance;

    x_coord.setReference(goal_);

    // publish info to the console for the user
    ROS_INFO("%s: Executing, got a target of %f", action_name_.c_str(), goal_);
}

void forwardPIDAction::preemptCB()
{
    ROS_INFO("%s: Preempted", action_name_.c_str());
    // set the action state to preempted
    as_.setPreempted();
}

void forwardPIDAction::visionCB(const geometry_msgs::PointStampedConstPtr &msg) {
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

    pwm_forward_right.data = x_coord.getPWM();
    pwm_forward_left.data = x_coord.getPWM();

    forwardLeftPublisher.publish(pwm_forward_left);
    forwardRightPublisher.publish(pwm_forward_right);        
}

