#include <angle_PID_server.h>

anglePIDAction::anglePIDAction(std::string name, std::string type_) :
    as_(nh_, name, false),
    action_name_(name), angle("ANGLE")
{
    //register the goal and feeback callbacks
    as_.registerGoalCallback(boost::bind(&anglePIDAction::goalCB, this));
    as_.registerPreemptCallback(boost::bind(&anglePIDAction::preemptCB, this));
    goal_ = 0;

    type = type_;
    angle.setPID(2.4, 0, 0.5, 1);
    
    //subscribe to the data topic of interest
    if (type == "VISION") {
        sub_ = nh_.subscribe("/line_task/line_coordinates", 1, &anglePIDAction::visionCB, this);
    }
    else if (type == "SENSOR") {
        sub_ = nh_.subscribe("/vision/sensors/depth", 1, &anglePIDAction::sensorCB, this);
    }

    sidewardFrontPublisher = nh_.advertise<std_msgs::Int32>("/pwm/sidewardFront", 1000);
    sidewardBackPublisher = nh_.advertise<std_msgs::Int32>("/pwm/sidewardBack", 1000);

    as_.start();
}

anglePIDAction::~anglePIDAction(void)
{
}

void anglePIDAction::goalCB()
{
    // helper variables
    ros::Rate r(1);
    bool success = true;

    goal_ = as_.acceptNewGoal()->target_angle;

    angle.setReference(goal_);

    // publish info to the console for the user
    ROS_INFO("%s: Executing, got a target of %f", action_name_.c_str(), goal_);
}

void anglePIDAction::preemptCB()
{
    ROS_INFO("%s: Preempted", action_name_.c_str());
    // set the action state to preempted
    as_.setPreempted();
}

void anglePIDAction::sensorCB(const std_msgs::Float32ConstPtr& msg)
{
    // make sure that the action hasn't been canceled
    if (!as_.isActive())
        return;
    
    angle.errorToPWM(msg->data);

    feedback_.current_angle = msg->data;
    as_.publishFeedback(feedback_);

    if (msg->data == goal_) {
        ROS_INFO("%s: Succeeded", action_name_.c_str());
        // set the action state to succeeded
        as_.setSucceeded(result_);
    }

    pwm_sideward_back.data = -1*angle.getPWM();
    pwm_sideward_front.data = angle.getPWM();

    sidewardFrontPublisher.publish(pwm_sideward_back);
    sidewardBackPublisher.publish(pwm_sideward_front);
}

void anglePIDAction::visionCB(const geometry_msgs::Pose2DConstPtr &msg) {
    if (!as_.isActive())
        return;
    
    angle.errorToPWM(msg->theta);

    feedback_.current_angle = msg->theta;

    as_.publishFeedback(feedback_);

    if (msg->theta == goal_) {
        ROS_INFO("%s: Succeeded", action_name_.c_str());
        // set the action state to succeeded
        as_.setSucceeded(result_);
    }

    pwm_sideward_back.data = -1*angle.getPWM();
    pwm_sideward_front.data = angle.getPWM();

    sidewardFrontPublisher.publish(pwm_sideward_back);
    sidewardBackPublisher.publish(pwm_sideward_front);        
}

