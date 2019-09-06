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

void Location::process(int usednum[], int n)
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

    if(m_dribble_->is_dribble_ == false)
    {
        m_plan_->catchMotionlessBall(ball_pos_);
    }
    else
    {
        if(flag_location < m)
        {
            target = addusedPoints[flag_location];
            ROS_INFO("all_points = %d  flag_location = %d", m, flag_location);
            br = target - robot_pos_;
            //m_plan_->positionAvoidObs2(br.angle().radian_, MAXW, 5.0*DEG2RAD);
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
            if(flag_face2target == true)////!!!!!!
            {
                m_plan_->positionAvoidObs2(br.angle().radian_, 5.0*DEG2RAD);
                m_plan_->move2Positionwithobs_noball(target,10.0,2.0*MAXVEL);
                m_plan_->m_behaviour_.app_vx_ *= 2;
                m_plan_->m_behaviour_.app_vy_ *= 2;
                //m_plan_->m_behaviour_.app_w_ = 0.0;////!!!!!!
            }
        }
        else
        {
            m_plan_->m_behaviour_.clearBehaviorState();
        }
        ROS_INFO("target: (%f, %f)", target.x_, target.y_);
    }
//    m_plan_->m_behaviour_.app_vx_ *= 2;
//    m_plan_->m_behaviour_.app_vy_ *= 2;
//    double square2 = m_plan_->m_behaviour_.app_vx_*m_plan_->m_behaviour_.app_vx_ + m_plan_->m_behaviour_.app_vy_*m_plan_->m_behaviour_.app_vy_;
//    if(square2>MAXVEL*MAXVEL)
//    {
//        m_plan_->m_behaviour_.app_vx_ *= MAXVEL*MAXVEL/square2;
//        m_plan_->m_behaviour_.app_vy_ *= MAXVEL*MAXVEL/square2;
//    }
//    double up_speed = 90.0;
//    double down_speed = 20.0;
//    if(square2 < up_speed*up_speed && square2 > down_speed*down_speed)
//    {
//        m_plan_->m_behaviour_.app_vx_ *= up_speed*up_speed/square2;
//        m_plan_->m_behaviour_.app_vy_ *= up_speed*up_speed/square2;
//    }
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
