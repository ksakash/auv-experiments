#ifndef BUOY_H
#define BUOY_H

#include <ros/ros.h>
#include <single_buoy.h>
#include <move_sideward_server.h>

class buoy {
public:
    buoy(double);
    ~buoy();
    void setActive(bool);

private:
    singleBuoy buoy_;
    moveSideward move_sideward_;
};
#endif // BUOY_H
