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
    Robot temp1=world_model_->RobotInfo_[world_model_->AgentID_-1];
    BallObject temp2=world_model_->BallInfo_[world_model_->AgentID_-1];

    m_subtargets_.robot_pos_=temp1.getLocation();
    m_subtargets_.ball_pos_=temp2.getGlobalLocation();

    if(fabs(target.distance(m_subtargets_.robot_pos_))>=10)
    {
        m_subtargets_.subtarget(target, m_subtargets_.robot_pos_, avoid_ball);
        m_behaviour_.move2Position(/*pval*/0.7,/*pdal*/0.3,target_.front(),maxvel,robot_pos_,robot_ori_);
    }

//    target_.clear();
//    target_.push_back(m_subtargets_.robot_pos_);
//    m_subtargets_.subtargets_pos_=m_subtargets_.robot_pos_;
//    while(fabs(target.distance(m_subtargets_.subtargets_pos_))>=10)//distance_thres
//    {
//        m_subtargets_.subtarget(target, m_subtargets_.subtargets_pos_, avoid_ball);
//        target_.push_back(m_subtargets_.subtargets_pos_);
//    }
//    target_.push_back(target);
//    DPoint emp;
//    for(int i=0; i<target_.size(); i++)
//    {
//        emp=target_.at(i);
//    }

//    target_.pop_back();//pop the robot position now
//    while(!target_.empty())
//    {
//        m_behaviour_.move2Position(/*pval*/0.7,/*pdal*/0.3,target_.front(),maxvel,robot_pos_,robot_ori_);
//        target_.pop_back();
//    }
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
