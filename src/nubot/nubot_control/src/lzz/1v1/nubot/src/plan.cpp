#include "nubot/nubot_control/plan.h"
#include "nubot/core/core.hpp"
using namespace  nubot;

Plan::Plan()
{
	isinposition_ = false;
}

void Plan::positionAvoidObs(DPoint target, double angle_thres)    //朝向目标位置
{
    Angle ori=target.angle()-robot_ori_;
    m_behaviour_.setAppvx(0.0);
    m_behaviour_.setAppvy(0.0);
    double tmp=ori.radian_-robot_ori_.radian_;
    if(fabs(tmp)>angle_thres)
        m_behaviour_.rotate2AbsOrienation(KA_MOVE,KB_MOVE,ori.radian_,max_w,robot_ori_.radian_, angle_thres);
    else
        m_behaviour_.setAppw(0);
}

void Plan::positionAvoidObs2(double targetangle, double angle_thres)    //朝向目标位置
{
    m_behaviour_.setAppvx(0.0);
    m_behaviour_.setAppvy(0.0);
    if(fabs(targetangle)>angle_thres)
        m_behaviour_.rotate2AbsOrienation(KA_MOVE,KB_MOVE,targetangle,max_w,robot_ori_.radian_, angle_thres);
    else
        m_behaviour_.setAppw(0);
}

void Plan::move2Positionwithobs_noball(DPoint target, double distance_thres, float maxvel, float maxacc, bool avoid_ball)  //移动到目标位置
{

    if(fabs(target.distance(robot_pos_))>=distance_thres)
    {
        m_subtargets_.robot_pos_=robot_pos_;
        m_subtargets_.ball_pos_=ball_pos_;
        m_subtargets_.subtarget(target, robot_pos_, avoid_ball);
        m_behaviour_.move2Position(KA_MOVE,KB_MOVE,m_subtargets_.subtargets_pos_,maxvel,robot_pos_,robot_ori_);
        m_behaviour_.accelerateLimit();
    }
    else
    {
        m_behaviour_.setAppvx(0.0);
        m_behaviour_.setAppvy(0.0);
        m_behaviour_.setAppw(0);
    }
}

void   Plan::update()
{
    lastdirection = robot_ori_.radian_;
    robot_pos_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getLocation();
    robot_ori_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getHead();
    robot_vec_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getVelocity();
    ball_pos_  = world_model_->BallInfo_[world_model_->AgentID_-1].getGlobalLocation();
    ball_vec_  = world_model_->BallInfo_[world_model_->AgentID_-1].getVelocity();
    inourfield_= world_model_->field_info_.isOurField(robot_pos_);
    inoppfield_= world_model_->field_info_.isOppField(robot_pos_);
    //isinposition_ kp kalpha kbeta target_

    m_subtargets_.robot_pos_ = robot_pos_;
    m_subtargets_.ball_pos_  = ball_pos_;
}
