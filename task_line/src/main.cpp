// Copyright 2018 AUV-IITK
#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <string>
#include <std_msgs/Float64MultiArray.h>
#include <errorToPWM.h>

ErrorDescriptor angle("ANGLE");
ErrorDescriptor x_coord("X_COORD"); // x_coord along forward direction
ErrorDescriptor y_coord("Y_COORD"); // y_coord along sideward direction

int angle_count = 0;
int coord_count = 0;
int depth_count  = 0;
double imu_angle_ = 0;

void lineCenterDataCallback(const std_msgs::Float64MultiArrayPtr &_msg) {
    if (coord_count == 0) {
        coord_count++;
        y_coord.setReference(0);
        x_coord.setReference(0);
    }

    y_coord.errorToPWM(_msg->data[0]);
    x_coord.errorToPWM(_msg->data[1]);
}

void lineAngleDataCallback(const std_msgs::Float64Ptr &_msg) {
    if (angle_count == 1) {
        angle.setReference(imu_angle_+ _msg->data);
        angle_count++;
    }
}

imuDataCallback(const std_msgs::Float64Ptr &_msg) {
    
    if (angle_count == 0) {
        imu_angle_ = _msg->data;
        angle_count++;
    }
    angle.errorToPWM(_msg->data);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "task_line");

    ros::NodeHandle nh;

    ros::Subscriber lineCenterDataListener = nh.subscribe("/varun/ip/line_centralize", 1000, &lineCenterDataCallback);
    ros::Subscriber lineAngleDataListener = nh.subscribe("/varun/ip/line_angle", 1000, &lineAngleDataCallback);
    ros::Subscriber imuDataListener = nh.subscribe("/varun/sensors/imu/yaw", 1000, &imuDataCallback);

    ros::Publisher frontSidewardPublisher = nh.advertise<std_msgs::Int32>("/pwm/sidewardFront", 1000);
    ros::Publisher backSidewardPublisher = nh.advertise<std_msgs::Int32>("/pwm/sidewardBack", 1000);
    ros::Publisher rightForwardPublisher = nh.advertise<std_msgs::Int32>("/pwm/forwardRight", 1000);
    ros::Publisher leftForwardPublisher = nh.advertise<std_msgs::Int32>("/pwm/forwardLeft", 1000);

    std_msgs::Int32 pwm_sideward_front;
    std_msgs::Int32 pwm_sideward_back;
    std_msgs::Int32 pwm_forward_right;
    std_msgs::Int32 pwm_forward_left;

    angle.setPID(2.4, 0, 0.5, 1);
    x_coord.setPID(0, 0, 0, 0);
    y_coord.setPID(-1.6, 0, -0.3, 15);

    ros::Rate loop_rate(50);

    while (ros::ok()) {
        pwm_sideward_front.data = y_coord.getPWM() + angle.getPWM();
        pwm_sideward_back.data = y_coord.getPWM() - angle.getPWM();
        if (angle.getCurrentValue() < 3 && y_coord.getCurrentValue() < 3) {
            pwm_forward_left.data = 100;
            pwm_forward_right.data = 100;
        }
        else {
            pwm_forward_left.data = x_coord.getPWM();
            pwm_forward_right.data = x_coord.getPWM();
        }

        pwm_upward_front.data = z_coord.getPWM();
        pwm_upward_back.data = z_coord.getPWM();

        frontSidewardPublisher.publish(pwm_sideward_front);
        backSidewardPublisher.publish(pwm_sideward_back);

        rightForwardPublisher.publish(pwm_forward_right);
        leftForwardPublisher.publish(pwm_forward_left);

        std::cout << "-------------------------------------------" << std::endl;
        ROS_INFO("PWM forward_right : %d", pwm_forward_right.data);
        ROS_INFO("PWM forward_left : %d", pwm_forward_left.data);
        ROS_INFO("PWM sideward_front : %d", pwm_sideward_front.data);
        ROS_INFO("PWM sideward_back : %d", pwm_sideward_back.data);

        loop_rate.sleep();
        ros::spinOnce();
    }

}
