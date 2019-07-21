#ifndef BALL_HANDLE_H
#define BALL_HANDLE_H

#include <ros/ros.h>
#include <serial/serial.h>
#include <realtime_tools/realtime_publisher.h>

//#include "nubot_hwcontroller/DebugInfo.h"
#include "nubot_common/VelCmd.h"
#include "nubot_common/OdoInfo.h"
#include "nubot_common/Shoot.h"
#include "nubot_common/BallHandle.h"

using namespace std;

class Nubot_HWController
{
public:
    Nubot_HWController();
    ~Nubot_HWController();
    // write info
    int Cmd_Vx,Cmd_Vy,Cmd_w;
    int ShootPower;
    bool PowerSwitch;
//    int ShootDep;


    // read info
    double Real_Vx,Real_Vy,Real_w;
    double Real_angle;
    bool BallIsHolding;
    bool PowerState;
    bool RobotStuck; //const

    ros::ServiceServer ballhandle_service_;
    ros::ServiceServer shoot_service_;

    void SerialWrite(const nubot_common::VelCmd::ConstPtr& cmd);
    void SerialWrite(const int ShootPower);//, const int ShootDep);
    void SerialWrite(const bool PowerSwitch);
    void SerialRead();
    bool BallHandleControlService(nubot_common::BallHandle::Request  &req,
                                  nubot_common::BallHandle::Response &res);
    bool ShootControlServive(nubot_common::Shoot::Request  &req,
                             nubot_common::Shoot::Response &res);

private:
    ros::NodeHandle nh;
    serial::Serial serial;
    ros::Subscriber Velcmd_sub_;
    ros::Timer timer1;
//    realtime_tools::RealtimePublisher<nubot_hwcontroller::DebugInfo> *DebugInfo_pub;
    realtime_tools::RealtimePublisher<nubot_common::OdoInfo> *OdoInfo_pub;

};


#endif
