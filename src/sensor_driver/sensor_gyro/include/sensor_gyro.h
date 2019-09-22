#ifndef SENSOR_GYRO
#define SENSOR_GYRO

#include <ros/ros.h>
#include <serial/serial.h>
#include <realtime_tools/realtime_publisher.h>

#include "nubot_common/GyroInfo.h"

#include "imu_data_decode.h"
#include "packet.h"

using namespace std;

class HI_Gyro
{
public:
    HI_Gyro();
    ~HI_Gyro();

    //from port
    uint8_t ID;
    int16_t Acc[3];
    int16_t Gyo[3];
    int16_t Mag[3];
    float Eular[3];
    float Quat[4];
    int32_t Pressure;
    uint8_t buf[1024];

    //publish
    double Eular_P,Eular_R,Eular_Y;
    double Acc_X,Acc_Y,Acc_Z;

    void SerialRead();

private:
    ros::NodeHandle nh;
    serial::Serial serial;
    realtime_tools::RealtimePublisher<nubot_common::GyroInfo> *GyroInfo_pub;

};

#endif // SENSOR_GYRO
