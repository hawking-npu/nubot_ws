#include <nubot/nubot_control/avoidance.h>

using namespace std;
using namespace nubot;

Avoidance::Avoidance()
{

}

Avoidance::Avoidance(World_Model_Info &_world_model, Plan &_plan, DribbleState &_dirbble)
{
    world_model_ = & _world_model;
    m_plan_ = & _plan;
    m_dribble_ = & _dirbble;
}

Avoidance::~Avoidance()
{
}

void Avoidance::process()
{

}

void Avoidance::update()
{
    robot_pos_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getLocation();
    robot_ori_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getHead();
    ball_pos_   = world_model_->BallInfo_[world_model_->AgentID_-1].getGlobalLocation();
    ball_vel_   = world_model_->BallInfo_[world_model_->AgentID_-1].getVelocity();
    world_model_->Obstacles_.clear();
    world_model_->Opponents_.clear();
}
