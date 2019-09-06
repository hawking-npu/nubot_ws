#include "nubot/nubot_control/plan.h"
#include "nubot/core/core.hpp"
using namespace  nubot;

Plan::Plan()
{
    isinposition_ = false;

    inourfield_ = world_model_->field_info_.isOurField(robot_pos_);
    inoppfield_ = world_model_->field_info_.isOppField(robot_pos_);
}

/***********catch ball***********/
void Plan::catchBall()  //
{
    //if(ball_vec_.norm()<10.0)
    {
        catchMotionlessBall(ball_pos_);
    }
//    else
//    {
//        catchMovingBall();
//    }
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

void Plan::catchMovingBall(double maxvel, double maxw)  //抓住移动的球
{
    double time_robot2ball = robot_pos_.distance(ball_pos_)/MAXVEL;
    DPoint ball_pos_fur_ = DPoint(ball_pos_.x_+ball_vec_.x_*time_robot2ball, ball_pos_.y_+ball_vec_.y_*time_robot2ball);

    if(m_dribble_->is_dribble_ != true)
    {
        DPoint br = ball_pos_ - robot_pos_;

        positionAvoidObs2(br.angle().radian_, maxw, 5*DEG2RAD);
        if(robot_pos_.distance(ball_pos_fur_) < LIMITDRIBLLEDIS)
        {
            if(fabs(br.angle().radian_-robot_ori_.radian_)<5*DEG2RAD)
            {
                move2Positionwithobs_noball(ball_pos_fur_,10.0,maxvel);
            }
        }
        else
        {
            move2Positionwithobs_noball(ball_pos_fur_,10.0,maxvel);
        }
    }
}

void Plan::catchMotionlessBall(DPoint target, double maxvel, double maxw)  //抓住静止的球
{
    if(m_dribble_->is_dribble_ != true)
    {
        ROS_INFO("Catch_Motionless_Ball (%f,%f)", target.x_, target.y_);
        DPoint br = target - robot_pos_;

        positionAvoidObs2(br.angle().radian_, maxw, 5*DEG2RAD);
        if(robot_pos_.distance(target) < LIMITDRIBLLEDIS)
        {
            if(fabs(br.angle().radian_-robot_ori_.radian_)<5*DEG2RAD)
            {
                move2Positionwithobs_noball(target,10.0,maxvel);
            }
        }
        else
        {
            move2Positionwithobs_noball(target,10.0,maxvel);
        }
    }
}

/***********postion*************/
void Plan::positionAvoidObs(DPoint target, double maxw, double angle_thres)    //朝向目标位置
{
    DPoint br = target - robot_pos_;
    positionAvoidObs2(br.angle().radian_, maxw, angle_thres);
}

void Plan::positionAvoidObs2(double targetangle, double maxw, double angle_thres)    //朝向目标位置
{
    if(fabs(targetangle-robot_ori_.radian_)>angle_thres)
        m_behaviour_.rotate2AbsOrienation(KA_MOVE,KB_MOVE,targetangle,maxw,robot_ori_.radian_, angle_thres);
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
    }
    else
    {
        m_behaviour_.setAppvx(0.0);
        m_behaviour_.setAppvy(0.0);
    }
}

bool Plan::pnpoly(const std::vector<DPoint> & pts, const DPoint & test_pt)
{
    int nvert=pts.size();
    int minX(100000),maxX((-100000)),maxY(-100000),minY((100000));
    for(std::size_t i = 0; i <nvert ;i++)
    {
        if(pts[i].x_<minX)
            minX=pts[i].x_;
        if(pts[i].x_>maxX)
            maxX=pts[i].x_;
        if(pts[i].y_<minY)
            minY=pts[i].y_;
        if(pts[i].y_>maxY)
            maxY=pts[i].y_;
    }

    if (test_pt.x_ < minX || test_pt.x_ > maxX || test_pt.y_< minY || test_pt.y_ > maxY)
        return false;

    int i, j;
    bool c = false;
    for (i = 0, j = nvert-1; i < nvert; j = i++)
    {
        if ( ( (pts[i].y_>test_pt.y_) != (pts[j].y_>test_pt.y_) ) &&
             (test_pt.x_ < (pts[j].x_-pts[i].x_) * (test_pt.y_-pts[i].y_)/double((pts[j].y_-pts[i].y_))+ pts[i].x_) )
            c = !c;
    }
    return c;
}

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
    DPoint br=ball_pos_-robot_pos_;

    move2Positionwithobs_noball(ball_pos_);
    ROS_INFO("roundfoot1");
    cout<<"robot pos: "<<robot_pos_.x_<<' '<<robot_pos_.y_<<endl;
    cout<<"ball pos: "<<ball_pos_.x_<<' '<<ball_pos_.y_<<endl;
    cout<<"robot vel: "<<m_behaviour_.app_vx_<<' '<<m_behaviour_.app_vy_<<' '<<m_behaviour_.app_w_<<' '<<endl;
    if(ball_pos_.distance(robot_pos_) < 100.0)
    {
        ROS_INFO("roundfoot2");
        positionAvoidObs2(br.angle().radian_);
        if(fabs(br.angle().radian_ - robot_ori_.radian_) < 8.0/180.0*SINGLEPI_CONSTANT)
        {
            ROS_INFO("roundfoot3");
            m_behaviour_.setAppw(0.0);
        }
        m_behaviour_.setAppvx(0.0);
        m_behaviour_.setAppvy(50.0);
    }
}

int Plan::oppneartargetid(DPoint target)
{
    double max_dist = INF;
    int max_id = 1;
    for(int i=0; i<world_model_->Opponents_.size(); ++i)
    {
//        if(target.distance(world_model_->Opponents_[i]) < OBLE_RADIUS/2)
//        {
//            continue;
//        }
        if(target.distance(world_model_->Opponents_[i]) < max_dist)
        {
            max_dist = target.distance(world_model_->Opponents_[i]);
            max_id = i+1;
        }
    }
    return max_id;
}

int Plan::ourneartargetid(DPoint target, int avoidid)
{
    DPoint avoid_pt = DPoint(10000.0, 10000.0);
    if(avoidid != 0)
    {
        avoid_pt = world_model_->RobotInfo_[avoidid-1].getLocation();
    }
    double max_dist = INF;
    int max_id = 1;
    for(int i=0; i<OUR_TEAM; ++i)
    {
        if(target.distance(world_model_->RobotInfo_[i].getLocation()) < RADIUS/2)
        {
            continue;
        }
        if(avoid_pt.distance(world_model_->RobotInfo_[i].getLocation()) < RADIUS/2)
        {
            continue;
        }
        if(target.distance(world_model_->RobotInfo_[i].getLocation()) < max_dist)
        {
            max_dist = target.distance(world_model_->RobotInfo_[i].getLocation());
            max_id = i+1;
        }
    }
    return max_id;
}
