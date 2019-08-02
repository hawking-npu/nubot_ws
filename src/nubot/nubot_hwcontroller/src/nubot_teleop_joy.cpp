/*
 * nubot_teleop_key.cpp
 *
 *  Created on: 2012-10-11
 *      Author: hkh
 */
#include <ros/ros.h>
#include <stdio.h>
#include <pthread.h>

#include <math.h>

#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>

#include <nubot_common/VelCmd.h>
#include <nubot_common/OminiVisionInfo.h>
#include <nubot_common/BallHandle.h>
#include <nubot_common/Shoot.h>


class TeleopNubot
{
public:
    TeleopNubot();
    ros::NodeHandle n;
    ros::ServiceClient ballhandle_client_;
    ros::ServiceClient shoot_client_;

    double Vx,Vy,w;

private:
    ros::Subscriber joy_sub;
    ros::Publisher vel_pub;
    ros::Timer vel_publish_timer_;
//    ros::Subscriber ball_sub;

    nubot_common::VelCmd cmd;
    bool CatchEnable;
    double ball_angle;
    double deadzone_;

    void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
//    void ballCallback(const nubot_common::OminiVisionInfo::ConstPtr& ball);
    void publish(const ros::TimerEvent &);
};

TeleopNubot::TeleopNubot()
    :Vx(0),Vy(0),w(0),ball_angle(0),CatchEnable(false)
{
    joy_sub = n.subscribe<sensor_msgs::Joy>("joy", 2, &TeleopNubot::joyCallback, this);
    vel_pub = n.advertise<nubot_common::VelCmd>("/nubotcontrol/velcmd", 10);
    vel_publish_timer_ = n.createTimer(ros::Duration(0.05),&TeleopNubot::publish,this);

//    ball_sub= n.subscribe<nubot_common::OminiVisionInfo>("/omnivision/OminiVisionInfo", 2, &TeleopNubot::ballCallback, this);
    ballhandle_client_ =  n.serviceClient<nubot_common::BallHandle>("BallHandle");
    shoot_client_ = n.serviceClient<nubot_common::Shoot>("Shoot");
}

// 手柄消息回调函数，在ros::spin()里执行
#define linearVel_X 0
#define linearVel_Y 1
#define angularVel 3 // direction <0:clockwise >0:counter-clockwise

#define click    0
#define click_depression 5
#define Button_B    1
#define ballhandle    2
#define Button_Y    3

#define Button_Up   5
#define Button_Down 4

void TeleopNubot::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    // click
    static float strength = 1;
    static int depression = 0;
    if(joy->buttons[Button_Y])
    {
        strength += 0.2;

        if(strength > 40)
           strength = 40;
    }
    else if(joy->buttons[Button_B])
    {
        strength -= 0.2;

        if(strength < 0)
           strength = 0;
    }
    if(joy->buttons[click])
    {
        nubot_common::Shoot s;
        depression = (int) (1 - joy->axes[click_depression])*30; // max_depression : 2*30=60
        s.request.ShootPos= depression;
        s.request.strength=strength;
        ROS_INFO("[CLICK] POWER :  %.1f, POS:  %d", strength, depression);
        shoot_client_.call(s);
    }

    // ballhandle
    static bool BallHandleEnable = false;
    if(joy->buttons[ballhandle]==1)
    {
        BallHandleEnable=!BallHandleEnable;
        nubot_common::BallHandle b;
        b.request.enable=BallHandleEnable;
        ballhandle_client_.call(b);

    }

    // velocity
    cmd.Vx = - joy->axes[linearVel_X]*2000;
    cmd.Vy = joy->axes[linearVel_Y]*2000;
    cmd.w  = - joy->axes[angularVel]*1500;
//    vel_pub.publish(cmd);
}

void TeleopNubot::publish(const ros::TimerEvent &)
{
   vel_pub.publish(cmd);
}

//void TeleopNubot::ballCallback(const nubot_common::OminiVisionInfo::ConstPtr& ball)
//{
//    ball_angle = ball->ballinfo.real_pos.angle;
//    ROS_INFO("Angle:%.2f",ball_angle/M_PI*180.0);

//    if(CatchEnable)
//    {
//        cmd.w  = ball_angle;
//        vel_pub.publish(cmd);
//    }
//}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "nubot_teleop_joy");
	TeleopNubot teleop_nubot;

#if 1
	pid_t pid = vfork();
	if(pid==0)
  {
    if(execlp("rosrun", "rosrun", "joy", "joy_node", "_deadzone:=0.14", (char *)0) < 0)
      ROS_WARN("Process Joy not found!");
    // 正常情况下exec函数不会返回
		return(-1);
	}
#endif
  ros::spin();
	return (0);
}


