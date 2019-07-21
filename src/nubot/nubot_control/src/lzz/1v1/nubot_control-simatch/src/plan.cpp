#include "nubot/nubot_control/plan.h"
#include "nubot/core/core.hpp"
using namespace  nubot;

/*
public:
        World_Model_Info * world_model_;
        Behaviour  m_behaviour_;
        Subtargets m_subtargets_;

        float kp;
        float kalpha;
        float kbeta;
        bool  inourfield_;
        bool  inoppfield_;
        double lastdirection;

        DPoint robot_pos_;
        Angle  robot_ori_;
        DPoint robot_vec_;
        DPoint ball_pos_;
        DPoint ball_vec_;

        vector<DPoint> target_;

public:
        bool   isinposition_;
*/
Plan::Plan()
{
	isinposition_ = false;

    inourfield_ = world_model_->field_info_.isOurField(robot_pos_);
    inoppfield_ = world_model_->field_info_.isOppField(robot_pos_);
}

/***********catch ball***********/
void Plan::catchBall()  //
{
    if(ball_vec_.norm()<10.0)
    {
        catchMotionlessBall();
    }
    else
    {
        catchMovingBall();
    }
}

void Plan::catchBallForCoop()
{
    catchBallSlowly();
}

void Plan::catchBallSlowly()  //缓慢向球移动，等球接近
{
    positionAvoidObs(ball_pos_);
    if(fabs(robot_ori_.radian_ - (ball_pos_-robot_pos_).angle().radian_) < 5.0/180.0*SINGLEPI_CONSTANT)
    {
        move2Positionwithobs_noball(ball_pos_,10.0);
    }
}

void Plan::catchMovingBall()  //抓住移动的球
{
    double time_robot2ball = robot_pos_.distance(ball_pos_)/max_vel;
    move2Positionwithobs_noball(DPoint(ball_pos_.x_+ball_vec_.x_*time_robot2ball, ball_pos_.y_+ball_vec_.y_*time_robot2ball));
}
void Plan::catchMotionlessBall()  //抓住静止的球
{
    move2Positionwithobs_noball(ball_pos_);
}

/***********postion*************/
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

void Plan::driblleControl(DPoint target,double acc,double sacc,double lvel,double maxvel)
{}

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

/*********round with soccer************/
void Plan::ballRoundTrack()
{
    DPoint ori=ball_pos_-robot_pos_;

    //cout<<"robotori: "<<robot_ori_.radian_<<endl;
    //cout<<"distance: : "<<fabs(ball_pos_.distance(robot_pos_))<<endl;
    if(fabs(ball_pos_.distance(robot_pos_))>=100)
    {
        m_subtargets_.robot_pos_=robot_pos_;
        m_subtargets_.ball_pos_=ball_pos_;
        m_subtargets_.subtarget(ball_pos_, robot_pos_, false);
        m_behaviour_.move2Position(KA_MOVE,KB_MOVE,m_subtargets_.subtargets_pos_,max_vel,robot_pos_,robot_ori_);
    }
    else
    {
        ROS_INFO("roundfoot2");
        m_behaviour_.setAppvx(0.0);
        m_behaviour_.setAppvy(50.0);
        double tmp=ori.angle().radian_-robot_ori_.radian_;
        //cout<<"tmp:"<<tmp<<endl;
        //cout<<"ori pos: "<<ori.x_<<' '<<ori.y_<<endl;
        if(fabs(tmp)>8.0/180.0*SINGLEPI_CONSTANT)
            m_behaviour_.rotate2AbsOrienation(KA_MOVE,KB_MOVE,ori.angle().radian_,max_w,robot_ori_.radian_);
        else
            m_behaviour_.setAppw(0);
    }
}
