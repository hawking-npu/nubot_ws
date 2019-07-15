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
void Plan::catchBallForCoop(){}
void Plan::catchBallSlowly()  //缓慢向球移动，等球接近
{
    move2Positionwithobs_noball(world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].getTarget(),10.0);
    world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].setDribbleState(true);
}
void Plan::catchMovingBall()  //抓住移动的球
{
    double distance_robot2ball = robot_pos_.distance(ball_pos_);
    double time_robot2ball = distance_robot2ball/max_vel;

    world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].setTarget(DPoint(ball_pos_.x_+ball_vec_.x_*time_robot2ball, ball_pos_.y_+ball_vec_.y_*time_robot2ball));
    move2Positionwithobs_noball(world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].getTarget());
    world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].setDribbleState(true);
}
void Plan::catchMotionlessBall()  //抓住静止的球
{
    world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].setTarget(ball_pos_);
    move2Positionwithobs_noball(ball_pos_);
    world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].setDribbleState(true);
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

void Plan::positionAvoidObs2(double targetangle, double angle_thres)    //朝向目标位置
{
    m_behaviour_.setAppvx(0.0);
    m_behaviour_.setAppvy(0.0);
    if(fabs(targetangle)>angle_thres)
        m_behaviour_.rotate2AbsOrienation(1.0,0.1,targetangle,max_w,robot_ori_.radian_, angle_thres);
    else
        m_behaviour_.setAppw(0);
}

void Plan::driblleControl(DPoint target,double acc,double sacc,double lvel,double maxvel)
{}

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
        emp=target_[i];
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
bool Plan::IsNullInTrap(double direction, double swidth, double lwidth, double len)//周围是否有障碍物
{
    std::vector<DPoint> pts, obs_pts;
    pts.clear(); //front
    obs_pts.clear(); //back
    std::vector<DPoint> obs_info = world_model_->Opponents_;
/*
    DPoint pnt[4];//pnt转为世界坐标
    pnt[0] = DPoint( front_len,  front_width/2);
    pnt[1] = DPoint( front_len, -front_width/2);
    pnt[2] = DPoint(-front_len,  front_width/2);
    pnt[3] = DPoint(-front_len, -front_width/2);
    DPoint result_pnt[4];
    double theta = direction.radian_;

    for(int i=0; i<4; ++i)
    {
        result_pnt[i].x_ = pnt[i].y_*cos(theta) + pnt[i].x_*sin(theta);
        result_pnt[i].y_ = pnt[i].y_*sin(theta) - pnt[i].x_*cos(theta);

        pnt[i] = result_pnt[i];
    }
*/
    //obs转换为自身坐标 判断前后
    DPoint tmp, now_tmp;
    double theta = -direction;
    for(int i=0; i<obs_info.size(); ++i)
    {
        now_tmp = obs_info[i];
        tmp.x_ = now_tmp.y_*cos(theta) + now_tmp.x_*sin(theta);
        tmp.y_ = now_tmp.y_*sin(theta) - now_tmp.x_*cos(theta);
        if(tmp.y_ > 0)
        {
            if(fabs(tmp.y_) > len || fabs(tmp.x_) > lwidth)
                continue;
            pts.push_back(now_tmp);
        }
        else
        {
            if(fabs(tmp.y_) > len || fabs(tmp.x_) > lwidth)
                continue;
            obs_pts.push_back(now_tmp);
        }
    }
    return pnpoly(robot_pos_, pts, obs_pts);
}

bool Plan::pnpoly(DPoint test, const std::vector<DPoint> & pts, const std::vector<DPoint> & obs_pts)  //判断一个坐标点是否在不规则多边形内部
{
    bool flag = false;
    std::vector<DPoint> pt;
    pt.clear();

    DPoint max_pt;
    DPoint min_pt;
    max_pt.x_ = max_pt.y_ = -INF;
    min_pt.x_ = min_pt.y_ = INF;

    DPoint tmp;
    for(int i=0; i<pts.size(); ++i)
    {
        tmp = pts[i];
        pt.push_back(tmp);
        if(tmp.x_ > max_pt.x_) max_pt.x_ = tmp.x_;
        if(tmp.y_ > max_pt.y_) max_pt.y_ = tmp.y_;
        if(tmp.x_ < min_pt.x_) min_pt.x_ = tmp.x_;
        if(tmp.y_ < min_pt.y_) min_pt.y_ = tmp.y_;
    }
    for(int i=0; i<obs_pts.size(); ++i)
    {
        tmp = obs_pts[i];
        pt.push_back(tmp);
        if(tmp.x_ > max_pt.x_) max_pt.x_ = tmp.x_;
        if(tmp.y_ > max_pt.y_) max_pt.y_ = tmp.y_;
        if(tmp.x_ < min_pt.x_) min_pt.x_ = tmp.x_;
        if(tmp.y_ < min_pt.y_) min_pt.y_ = tmp.y_;
    }
    //横坐标和纵坐标的最大值和最小值，判断目标坐标点是否在这个四边形之内
    if(test.x_<min_pt.x_ || test.x_>max_pt.x_ || test.y_<min_pt.y_ || test.y_>max_pt.y_)
        return false;

    DPoint tmpi, tmpj;
    for(int i=0, j=pt.size()-1; i<pt.size(); j=i, ++i)
    {
        tmpi = pt[i];
        tmpj = pt[j];
        if( ((tmpi.y_>test.y_) != (tmpj.y_>test.y_)) &&
                (test.x_ < (tmpj.x_-tmpi.x_) * (test.y_-tmpi.y_) / (tmpj.y_-tmpi.y_) + tmpi.x_) )
            flag = !flag;
    }
    return flag;
}

/*********find and search*********/
double Plan::FindBstDirectionForAvoid()
{
    DPoint tmp = DPoint(oppGoalPosl.x_, 0.0);
}

double Plan::FindBstDirectionForAvoid2(DPoint target)
{
    m_subtargets_.subtarget(target,robot_pos_,1);
    DPoint tmp = m_subtargets_.subtargets_pos_ - robot_pos_;
    return atan2(tmp.y_, tmp.x_);
}
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

void   Plan::update()
{
    /*
    public:
            World_Model_Info * world_model_;
            Behaviour  m_behaviour_;
            Subtargets m_subtargets_;
    */
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
