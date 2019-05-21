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
}

/***********catch ball***********/
void catchBall();
void catchBallForCoop();
void catchBallSlowly();
void catchMovingBall();
void catchMotionlessBall();

/***********postion*************/
void positionAvoidObs(DPoint target, float theta, float stopdis, float stoptheta);
void driblleControl(DPoint target,double acc,double sacc,double lvel,double maxvel);
//void move2Positionwithobs(DPoint target);  // move to the target point with obstacles avoidance
void Plan::move2Positionwithobs_noball(DPoint target, float maxvel, float maxacc, bool avoid_ball)
{
    m_subtargets_.world_model_=world_model_;
    Angle ori=target.angle()-robot_ori_;

    if(fabs(target.distance(robot_pos_))>=10)
    {
        m_subtargets_.robot_pos_=robot_pos_;
        m_subtargets_.ball_pos_=ball_pos_;
        m_subtargets_.subtarget(target, robot_pos_, avoid_ball);
        m_behaviour_.move2Position(0.9,0.1,m_subtargets_.subtargets_pos_,max_vel,robot_pos_,robot_ori_);
    }
    else
    {
        m_behaviour_.setAppvx(0.0);
        m_behaviour_.setAppvy(0.0);
        m_behaviour_.setAppw(0);
    }
    ROS_INFO("subtargets");
    cout<<m_subtargets_.subtargets_pos_.x_<<' '<<m_subtargets_.subtargets_pos_.y_<<endl;
    ROS_INFO("robot pos");
    cout<<robot_pos_.x_<<' '<<robot_pos_.y_<<endl;
    /*
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
double PECrossBackMIdlleLine(double direction);
double PEOutField(double direction);
double PEInOurPenaty(double direction);
double PObleDirection4OurField(double direction, double predictlen, double cobledirection, double kobledirection);
double PObleDirection(double direction, double predictlen, double cobledirection,double kobledirection);

/***********check***********/
bool IsNullInTrap(double direction, double swidth, double lwidth, double len);
/*********find and search*********/
double FindBstDirectionForAvoid();
double FindBstDirectionForAvoid2(DPoint target);
int    GetAvoidState();
double SearchDirectionforMinPEPoint(double oridirection,double step,int lefttime,int righttime);
bool   SearchMinPE4PassThroughforOurField(double &direction,double pridictlen,DPoint trap[4],double step,int flg);
bool   SearchMinPE4PassThrough(double &direction, double pridictlen, DPoint trap[4], double step, int flg);

void   update();

/*********round with foot************/
void Plan::roundfoot()
{
    m_subtargets_.world_model_=world_model_;
    DPoint ori=ball_pos_-robot_pos_;

    cout<<"robotori: "<<robot_ori_.radian_<<endl;
    cout<<"distance: : "<<fabs(ball_pos_.distance(robot_pos_))<<endl;
    if(fabs(ball_pos_.distance(robot_pos_))>=100)
    {
        m_subtargets_.robot_pos_=robot_pos_;
        m_subtargets_.ball_pos_=ball_pos_;
        m_subtargets_.subtarget(ball_pos_, robot_pos_, false);
        m_behaviour_.move2Position(0.9,0.1,m_subtargets_.subtargets_pos_,max_vel,robot_pos_,robot_ori_);
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
            m_behaviour_.rotate2AbsOrienation(0.9,0.1,ori.angle().radian_,max_w,robot_ori_.radian_);
        else
            m_behaviour_.setAppw(0);
    }
}
