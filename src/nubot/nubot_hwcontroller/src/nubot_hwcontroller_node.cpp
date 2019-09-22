#include <stdio.h>
#include <string>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>

#include "nubot_hwcontroller_node.h"


template <class Type>
Type stringToNum(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

Nubot_HWController::Nubot_HWController()
{
    //设置串口属性，并打开串口
    try
    {
        //serial.setPort("/dev/ttyUSB0");
        serial.setPort("/dev/Robot_control");
        serial.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        serial.setTimeout(to);
        serial.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port");
    }
    //检测串口是否已经打开，并给出提示信息
    if (serial.isOpen())
    {
        ROS_INFO_STREAM("Serial Port initialized");
    }
    else
    {
        ROS_ERROR_STREAM("Unable to initialize port");
    }

   // 注册里程计以及调试信息topic
//   DebugInfo_pub = new realtime_tools::RealtimePublisher<nubot_hwcontroller::DebugInfo>(nh, "/nubotdriver/debug", 1);
   OdoInfo_pub = new realtime_tools::RealtimePublisher<nubot_common::OdoInfo>(nh, "/nubotdriver/odoinfo", 1);

   // 接受底盘速度指令
   Velcmd_sub_ = nh.subscribe("/nubotcontrol/velcmd",1,&Nubot_HWController::SerialWrite,this);

   // 注册主动带球以及射门服务
   ballhandle_service_ = nh.advertiseService("BallHandle",&Nubot_HWController::BallHandleControlService,this);
   shoot_service_ = nh.advertiseService("Shoot",&Nubot_HWController::ShootControlServive,this);

}

Nubot_HWController::~Nubot_HWController()
{

}

void Nubot_HWController::SerialWrite(const nubot_common::VelCmd::ConstPtr& cmd)
{
//    ROS_INFO("write msgs");
    Cmd_Vx = cmd->Vx;
    Cmd_Vy = cmd->Vy;
    Cmd_w = cmd->w;

    if(!(cmd->stop_ == false && cmd->Vx == 0 && cmd->Vy == 0 && cmd->w == 0))
    {
        std::stringstream write_stream_;
        write_stream_ << 1 << "\t" << Cmd_Vx << "\t" << 2 << "\t" << Cmd_Vy << "\t" << 3 << "\t" << Cmd_w << "\n";
        std_msgs::String write_stream;
        write_stream.data = write_stream_.str();
        serial.write(write_stream.data);
    }
}

void Nubot_HWController::SerialWrite(const int ShootPower)
{
    std::stringstream write_stream_;
    //write_stream_ << "A\t" << ShootPower << "\n";
    write_stream_<<"A";
    std_msgs::String write_stream;
    write_stream.data = write_stream_.str();
    serial.write(write_stream.data);
    ROS_INFO("hw_shoot");
}

void Nubot_HWController::SerialWrite(const bool PowerSwitch)
{
//    ROS_INFO("write msgs");
    std::stringstream write_stream_;
    write_stream_ << "X\t" << PowerSwitch << "\n";
    std_msgs::String write_stream;
    write_stream.data = write_stream_.str();
    serial.write(write_stream.data);
}

void Nubot_HWController::SerialRead()
{
    if(serial.available() && OdoInfo_pub->trylock())
    {
//        ROS_INFO_STREAM("Reading from serial port\n");
        std_msgs::String read_stream;
        read_stream.data = serial.read(serial.available());
        std::istringstream read_stream_(read_stream.data);
        string tmp1,tmp2,tmp3,tmp4,tmp5,tmp6;
//        read_stream_ >> tmp1 >> tmp2 >> tmp3 >> tmp4  >> tmp5 >> tmp6;
        read_stream_ >> tmp5;
//        Real_Vx = stringToNum<double>(tmp1);
//        Real_Vy = stringToNum<double>(tmp2);
//        Real_w = stringToNum<double>(tmp3);
//        Real_angle = stringToNum<double>(tmp4);
        Real_Vx = 0.;
        Real_Vy = 0.;
        Real_w = 0.;
        Real_angle = 0.;
        BallIsHolding = stringToNum<bool>(tmp5);
        PowerState = true;
        RobotStuck = false; // not controlled by the upper computer

        OdoInfo_pub->msg_.header.stamp = ros::Time::now();
        OdoInfo_pub->msg_.Vx=Real_Vx;
        OdoInfo_pub->msg_.Vy=Real_Vy;
        OdoInfo_pub->msg_.w =Real_w;
        OdoInfo_pub->msg_.angle=Real_angle;
        OdoInfo_pub->msg_.BallIsHolding = BallIsHolding;
        OdoInfo_pub->msg_.RobotStuck =RobotStuck;
        OdoInfo_pub->msg_.PowerState =PowerState;

        OdoInfo_pub->unlockAndPublish();
    }
}

bool Nubot_HWController::BallHandleControlService(nubot_common::BallHandle::Request  &req,
                             nubot_common::BallHandle::Response &res)
{
//   if(BallHandleEnable!=req.enable)
//       Ballhandle_Enable(req.enable);
//   BallHandleEnable=req.enable;
//   SerialWrite(BallHandleEnable);
//   res.BallIsHolding = BallIsHolding;

   PowerSwitch=req.enable;
   SerialWrite(PowerSwitch);

   return true;
}

bool Nubot_HWController::ShootControlServive(nubot_common::Shoot::Request  &req,
                        nubot_common::Shoot::Response &res)
{
   // Whether to shoot or not
   if(req.strength==0)
   {
       // Adjust the shooter rod only
       ShootPower = req.strength;
//       ShootDep   = req.ShootPos;
       res.ShootIsDone = 0;
   }
   else
   {
       ShootPower = req.strength;
//       ShootDep   = req.ShootPos;
       SerialWrite(ShootPower);
       res.ShootIsDone = 1;
   }
   return true;
}

int main(int argc,char** argv)
{

   ros::init(argc,argv, "nubot_hwcontroller_node");
   Nubot_HWController controller;
   ros::Rate loop_rate(30);
   while(ros::ok())
   {
     controller.SerialRead();
     ros::spinOnce();
     loop_rate.sleep();
   }
   return 0;
}

