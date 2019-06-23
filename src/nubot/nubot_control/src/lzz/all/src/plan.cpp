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

    inourfield_ = fieldinformation_.isOurField(robot_pos_);
    inoppfield_ = fieldinformation_.isOppField(robot_pos_);
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
void Plan::catchBallForCoop(){}
void Plan::catchBallSlowly(){}
void Plan::catchMovingBall()  //抓住移动的球
{
    double distance_robot2ball = robot_pos_.distance(ball_pos_);
    double time_robot2ball = distance_robot2ball/max_vel;

    Robot robotinfo = world_model_->RobotInfo_[world_model_->AgentID_];
    robotinfo.setTarget(DPoint(ball_pos_.x_+ball_vec_.x_*time_robot2ball, ball_pos_.y_+ball_vec_.y_*time_robot2ball));
    robotinfo.setDribbleState(true);
    world_model_->RobotInfo_[world_model_->AgentID_] = robotinfo;
}
void Plan::catchMotionlessBall()  //抓住静止的球
{
    Robot robotinfo = world_model_->RobotInfo_[world_model_->AgentID_];
    robotinfo.setTarget(ball_pos_);
    robotinfo.setDribbleState(true);
    world_model_->RobotInfo_[world_model_->AgentID_] = robotinfo;
}

/***********postion*************/
void Plan::positionAvoidObs(DPoint target, double angle_thres)    //朝向目标位置
{
    Angle ori=target.angle()-robot_ori_;
    m_behaviour_.setAppvx(0.0);
    m_behaviour_.setAppvy(0.0);
    double tmp=ori.radian_-robot_ori_.radian_;
    if(fabs(tmp)>angle_thres)
        m_behaviour_.rotate2AbsOrienation(1.0,0.1,ori.radian_,max_w,robot_ori_.radian_, angle_thres);
    else
        m_behaviour_.setAppw(0);
}
void Plan::driblleControl(DPoint target,double acc,double sacc,double lvel,double maxvel){}
void Plan::move2Positionwithobs_noball(DPoint target, float maxvel, float maxacc, bool avoid_ball, double distance_thres)  //移动到目标位置
{

    if(fabs(target.distance(robot_pos_))>=distance_thres)
    {
        m_subtargets_.robot_pos_=robot_pos_;
        m_subtargets_.ball_pos_=ball_pos_;
        m_subtargets_.subtarget(target, robot_pos_, avoid_ball);
        m_behaviour_.move2Position(1.0,0.1,m_subtargets_.subtargets_pos_,maxvel,robot_pos_,robot_ori_);
        m_behaviour_.accelerateLimit();
    }
    else
    {
        m_behaviour_.setAppvx(0.0);
        m_behaviour_.setAppvy(0.0);
        m_behaviour_.setAppw(0);
    }
    /*
    ROS_INFO("subtargets");
    cout<<m_subtargets_.subtargets_pos_.x_<<' '<<m_subtargets_.subtargets_pos_.y_<<endl;
    ROS_INFO("robot pos");
    cout<<robot_pos_.x_<<' '<<robot_pos_.y_<<endl;
    target_.clear();
    target_.push_back(m_subtargets_.robot_pos_);
    m_subtargets_.subtargets_pos_=m_subtargets_.robot_pos_;
    while(fabs(target.distance(m_subtargets_.subtargets_pos_))>=10)//distance_thres
    {
        m_subtargets_.subtarget(target, m_subtargets_.subtargets_pos_, avoid_ball);
        target_.push_back(m_subtargets_.subtargets_pos_);
    }
    target_.push_back(target);
    DPoint emp;
    for(int i=0; i<target_.size(); i++)
    {
        emp=target_.at(i);
    }

    target_.pop_back();//pop the robot position now
    while(!target_.empty())
    {
        m_behaviour_.move2Position(0.7,0.3,target_.front(),maxvel,robot_pos_,robot_ori_);
        target_.pop_back();
    }*/
}

/***********PE and PO***********/
double Plan::PECrossBackMIdlleLine(double direction){}
double Plan::PEOutField(double direction){}
double Plan::PEInOurPenaty(double direction){}
double Plan::PObleDirection4OurField(double direction, double predictlen, double cobledirection, double kobledirection){}
double Plan::PObleDirection(double direction, double predictlen, double cobledirection,double kobledirection){}

/***********check***********/
bool Plan::IsNullInTrap(double direction, double swidth, double lwidth, double len){}//周围是否有障碍物
/*********find and search*********/
double Plan::FindBstDirectionForAvoid(){}
double Plan::FindBstDirectionForAvoid2(DPoint target){}
int Plan::GetAvoidState()  //返回AvoidObs Actions
{
    /*
    enum Actions
    {
        Stucked =0,
        Penalty =1, //处罚
        CanNotSeeBall =2,
        SeeNotDribbleBall =3,
        TurnForShoot =4, // Active
        AtShootSituation =5,
        TurnToPass=6,   //Dynamic pass 动态
        StaticPass =7,
        AvoidObs=8,
        Catch_Positioned = 9,   //catch position
        Positioned =10,          //move to position
        Positioned_Static  =11, //static
        No_Action  =12,
    };
    */
    return AvoidObs;
}
double Plan::SearchDirectionforMinPEPoint(double oridirection,double step,int lefttime,int righttime){}
bool   Plan::SearchMinPE4PassThroughforOurField(double &direction,double pridictlen,DPoint trap[4],double step,int flg){}
bool   Plan::SearchMinPE4PassThrough(double &direction, double pridictlen, DPoint trap[4], double step, int flg){}

void   Plan::update(){}

/*********round with foot************/
void Plan::roundfoot()
{
    DPoint ori=ball_pos_-robot_pos_;

    cout<<"robotori: "<<robot_ori_.radian_<<endl;
    cout<<"distance: : "<<fabs(ball_pos_.distance(robot_pos_))<<endl;
    if(fabs(ball_pos_.distance(robot_pos_))>=100)
    {
        m_subtargets_.robot_pos_=robot_pos_;
        m_subtargets_.ball_pos_=ball_pos_;
        m_subtargets_.subtarget(ball_pos_, robot_pos_, false);
        m_behaviour_.move2Position(1.0,0.1,m_subtargets_.subtargets_pos_,max_vel,robot_pos_,robot_ori_);
    }
    else
    {
        ROS_INFO("roundfoot2");
        m_behaviour_.setAppvx(0.0);
        m_behaviour_.setAppvy(50.0);
        double tmp=ori.angle().radian_-robot_ori_.radian_;
        cout<<"tmp:"<<tmp<<endl;
        //cout<<"ori pos: "<<ori.x_<<' '<<ori.y_<<endl;
        if(fabs(tmp)>8.0/180.0)
            m_behaviour_.rotate2AbsOrienation(1.0,0.1,ori.angle().radian_,max_w,robot_ori_.radian_);
        else
            m_behaviour_.setAppw(0);
    }
}
