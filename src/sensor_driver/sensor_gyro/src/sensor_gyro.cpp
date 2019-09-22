#include <stdio.h>
#include <string>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>

#include "sensor_gyro.h"
#include "imu_data_decode.h"
#include "packet.h"

template <class Type>
Type stringToNum(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

HI_Gyro::HI_Gyro()
{
    //设置串口属性，并打开串口
    try
    {
        serial.setPort("/dev/HI_control");
        serial.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        serial.setTimeout(to);
        serial.open();
        imu_data_decode_init();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Gyro: Unable to open port");
    }
    //检测串口是否已经打开，并给出提示信息
    if (serial.isOpen())
    {
        ROS_INFO_STREAM("Gyro: Serial Port initialized");
    }
    else
    {
        ROS_ERROR_STREAM("Gyro: Unable to initialize port");
    }

    GyroInfo_pub = new realtime_tools::RealtimePublisher<nubot_common::GyroInfo>(nh, "/nubotdriver/gyroinfo", 1);
}

HI_Gyro::~HI_Gyro()
{

}

void HI_Gyro::SerialRead()
{
    if(serial.available() && GyroInfo_pub->trylock())
    {
//        ROS_INFO_STREAM("Reading from serial port\n");
        std_msgs::String read_stream;
        read_stream.data = serial.read(serial.available());
        std::istringstream read_stream_(read_stream.data);
        string tmp;
        read_stream_ >> tmp;

        for(int i=0; i<tmp.size(); i++)
        {
            Packet_Decode((uint8_t)tmp[i]);
//            ROS_INFO("tmp[%d]: %d", i, tmp[i]);
        }

        get_raw_acc(Acc);
        get_raw_gyo(Gyo);
        get_raw_mag(Mag);
        get_eular(Eular);
        get_id(&ID);

        GyroInfo_pub->msg_.header.stamp = ros::Time::now();
        GyroInfo_pub->msg_.Eular_P = Eular[0];
        GyroInfo_pub->msg_.Eular_R = Eular[1];
        GyroInfo_pub->msg_.Eular_Y = Eular[2];
        GyroInfo_pub->msg_.Acc_X = Acc[0];
        GyroInfo_pub->msg_.Acc_Y = Acc[1];
        GyroInfo_pub->msg_.Acc_Z = Acc[2];

        GyroInfo_pub->unlockAndPublish();
    }
}

int main(int argc,char** argv)
{
   ros::init(argc,argv, "sensor_gyro");
   HI_Gyro controller;
   ros::Rate loop_rate(30);
   while(ros::ok())
   {
       controller.SerialRead();
       ros::spinOnce();
       loop_rate.sleep();
   }
   return 0;
}
