#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <string>
#include <boost/thread.hpp>
#include <move_forward_server.h>
#include <move_sideward_server.h>
#include <straight_server.h>
#include <forward_PID_server.h>

class singleBuoy {
public:
    singleBuoy(double);
    ~singleBuoy();
    void setActive(bool);
    void forwardCB(geometry_msgs::PointStampedConstPtr &_msg);

private:
    actionlib::SimpleActionClient<action_servers::forwardPIDAction> forwardPIDClient;
    moveForward move_forward_;
    moveSideward move_sideward_;
    moveStraight move_straight_;
    ros::Subscriber sub_;
    ros::NodeHandle nh_;
    double forward_distance_;
}