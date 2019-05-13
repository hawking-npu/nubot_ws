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
  :BallSensor_IsHolding(0)
{
    //设置串口属性，并打开串口
    try
    {
      serial.setPort("/dev/ttyUSB0");
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
   OdeInfo_pub = new realtime_tools::RealtimePublisher<nubot_common::OdoInfo>(nh, "/nubotdriver/odoinfo", 1);

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
//  ROS_INFO("write msgs");
  Cmd_Vx = cmd->Vx;
  Cmd_Vy = cmd->Vy;
  Cmd_w = cmd->w;

  std::stringstream write_stream_;
  write_stream_ << Cmd_Vx << "\t" << Cmd_Vy << "\t" << Cmd_w << "\n";
  std_msgs::String write_stream;
  write_stream.data = write_stream_.str();
  serial.write(write_stream.data);
}

void Nubot_HWController::SerialWrite(const float ShootPower, const int ShootDep)
{
  std::stringstream write_stream_;
  write_stream_ << "A\t" << ShootPower << "\t" << ShootDep<< "\n";
  std_msgs::String write_stream;
  write_stream.data = write_stream_.str();
  serial.write(write_stream.data);
}

void Nubot_HWController::SerialWrite(const bool BallHandleEnable)
{
//  ROS_INFO("write msgs");
  std::stringstream write_stream_;
  write_stream_ << "X\t" << BallHandleEnable << "\n";
  std_msgs::String write_stream;
  write_stream.data = write_stream_.str();
  serial.write(write_stream.data);
}

void Nubot_HWController::SerialRead()
{
  if(serial.available() && OdeInfo_pub->trylock())
  {
//      ROS_INFO_STREAM("Reading from serial port\n");
      std_msgs::String read_stream;
      read_stream.data = serial.read(serial.available());
      std::istringstream read_stream_(read_stream.data);
      string tmp1,tmp2,tmp3,tmp4,tmp5;
      read_stream_ >> tmp1 >> tmp2 >> tmp3 >> tmp4  >> tmp5;
      Real_Vx = stringToNum<double>(tmp1);
      Real_Vy = stringToNum<double>(tmp2);
      Real_w = stringToNum<double>(tmp3);
      BallSensor_IsHolding = stringToNum<bool>(tmp4);
      RobotStuck = stringToNum<bool>(tmp5);

      OdeInfo_pub->msg_.header.stamp = ros::Time::now();
      OdeInfo_pub->msg_.Vx=Real_Vx;
      OdeInfo_pub->msg_.Vy=Real_Vy;
      OdeInfo_pub->msg_.w =Real_w;
      OdeInfo_pub->msg_.BallSensor_IsHolding = BallSensor_IsHolding;
      OdeInfo_pub->msg_.RobotStuck = RobotStuck;
      OdeInfo_pub->unlockAndPublish();
  }
}

bool Nubot_HWController::BallHandleControlService(nubot_common::BallHandle::Request  &req,
                             nubot_common::BallHandle::Response &res)
{
//   if(BallHandleEnable!=req.enable)
//       Ballhandle_Enable(req.enable);
   BallHandleEnable=req.enable;
   SerialWrite(BallHandleEnable);
   res.BallIsHolding = BallSensor_IsHolding;

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
       ShootDep   = req.ShootPos;
       res.ShootIsDone = 0;
   }
   else
   {
       ShootPower = req.strength;
       ShootDep   = req.ShootPos;
       SerialWrite(ShootPower, ShootDep);
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






