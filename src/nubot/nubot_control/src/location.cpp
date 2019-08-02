#include <nubot/nubot_control/location.h>

using namespace std;
using namespace nubot;

Location::Location()
{
    init();
}

Location::Location(World_Model_Info & _world_model, Plan & _plan, DribbleState &_dribble)
{
    world_model_ = & _world_model;
    m_plan_ = & _plan;
    m_dribble_ = & _dribble;

    init();
}

Location::~Location()
{
}

void Location::process(int n, int usednum[])
{
    DPoint usedPoints[n];
    for(int i=0; i<n; ++i)
    {
        usedPoints[i] = allPoints[usednum[i]];
    }
    int m=0;
    DPoint addusedPoints[2*n];///
    for(int i=0, j=0; j<n; ++i)
    {
        if(i%2 == 1)
        {
            addusedPoints[i] = DPoint(0.0, 0.0);
        }
        else
        {
            addusedPoints[i] = usedPoints[j];
            j++;
        }
        m++;
    }
    addusedPoints[m] = DPoint(0.0, 0.0);
    m++;

    if(CatchBall())
    {
        if(flag_location < m)
        {
            target = addusedPoints[flag_location];
            ROS_INFO("all_points = %d  flag_location = %d", m, flag_location);
            br = target - robot_pos_;
            //m_plan_->positionAvoidObs2(br.angle().radian_, MAXW, 3.0*DEG2RAD);
            m_plan_->m_behaviour_.app_w_ = MAXW;
//            if(m_plan_->m_behaviour_.app_vx_ < 10.0 && fabs(m_plan_->m_behaviour_.app_w_) > 20*DEG2RAD)
//            {
//                m_plan_->m_behaviour_.app_vx_ = 10.0;
//            }
            if(fabs(br.angle().radian_ - robot_ori_.radian_) < 10.0*DEG2RAD)
            {
                flag_face2target = true;
            }
            if(target.distance(robot_pos_) < 10.0)
            {
                flag_location++;
                flag_face2target = false;
            }
            if(flag_face2target == true)
            {
                m_plan_->positionAvoidObs2(br.angle().radian_, 5.0*DEG2RAD);
                m_plan_->move2Positionwithobs_noball(target);
            }
        }
        else
        {
            m_plan_->m_behaviour_.clearBehaviorState();
        }
        ROS_INFO("target: (%f, %f)", target.x_, target.y_);
    }
}

bool Location::CatchBall()
{
    if(m_dribble_->is_dribble_ != true)
    {
        if(world_model_->BallInfo_[world_model_->AgentID_-1].isLocationKnown() == false)
        {
            ROS_INFO("CanNotSeeBall");
            m_plan_->m_behaviour_.app_vx_ = -10.0;
            m_plan_->m_behaviour_.app_vy_ = 0.0;
            m_plan_->m_behaviour_.app_w_  = 0.0;
        }
        else
        {
            ROS_INFO("location Catch_Ball");
            target = ball_pos_;
            br = ball_pos_ - robot_pos_;

            m_plan_->positionAvoidObs2(br.angle().radian_, MAXW, 3*DEG2RAD);
            if(robot_pos_.distance(ball_pos_) < LIMITDRIBLLEDIS + 20.0)
            {
                if(fabs(br.angle().radian_-robot_ori_.radian_)<3*DEG2RAD)
                {
                    m_plan_->move2Positionwithobs_noball(ball_pos_);
                }
            }
            else
            {
                m_plan_->move2Positionwithobs_noball(ball_pos_);
            }
        }
    }
    return m_dribble_->is_dribble_;
}

void Location::update()
{
    robot_pos_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getLocation();
    robot_ori_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getHead();
    ball_pos_   = world_model_->BallInfo_[world_model_->AgentID_-1].getGlobalLocation();
    ball_vel_   = world_model_->BallInfo_[world_model_->AgentID_-1].getVelocity();
    world_model_->Obstacles_.clear();
    world_model_->Opponents_.clear();
    if(robot_pos_.x_ == 0.0 && robot_pos_.y_ == 0.0)
    {
        flag_face2target = false;
    }
}

void Location::init()
{
    flag_location = 0;
    flag_face2target = false;
    allPoints[0]  = DPoint(   0.0,    0.0);
    allPoints[1]  = DPoint(-280.0,  180.0);
    allPoints[2]  = DPoint(- 72.5,  180.0);
    allPoints[3]  = DPoint(  72.5,  180.0);
    allPoints[4]  = DPoint( 280.0,  180.0);
    allPoints[5]  = DPoint(-225.0,  105.0);
    allPoints[6]  = DPoint(-122.5,   95.0);
    allPoints[7]  = DPoint(   0.0,  100.0);
    allPoints[8]  = DPoint( 122.5,   95.0);
    allPoints[9]  = DPoint( 225.0,  105.0);
    allPoints[10] = DPoint(-260.0,    0.0);
    allPoints[11] = DPoint(-165.0,    0.0);
    allPoints[12] = DPoint( 165.0,    0.0);
    allPoints[13] = DPoint( 260.0,    0.0);
    allPoints[14] = DPoint(-225.0, -105.0);
    allPoints[15] = DPoint(-122.5, - 95.0);
    allPoints[16] = DPoint(   0.0, -100.0);
    allPoints[17] = DPoint( 122.5, - 95.0);
    allPoints[18] = DPoint( 225.0, -105.0);
    allPoints[19] = DPoint(-280.0, -180.0);
    allPoints[20] = DPoint(- 72.5, -180.0);
    allPoints[21] = DPoint(  72.5, -180.0);
    allPoints[22] = DPoint( 280.0, -180.0);
}
