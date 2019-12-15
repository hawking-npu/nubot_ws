#include "nubot/nubot_control/normalgame.h"

using namespace std;
using namespace nubot;

NormalGame::NormalGame()
{
    init();
}

NormalGame::NormalGame(World_Model_Info & _world_model, Plan & _plan, DribbleState & _dribble)
{
    world_model_ = & _world_model;
    m_plan_ = & _plan;
    m_dribble_ = & _dribble;
    init();
}

NormalGame::~NormalGame()
{

}

void NormalGame::process(int role_)
{
    if(role_ == ACTIVE)
    {
        asActive();
    }
    else if(role_ == ASSISTANT)
    {
        asAssist();
    }
    else if(role_ == GOALIE)
    {
        asGoalie();
    }
    else
    {
        one2one();
    }

    if(role_ != GOALIE)
    {
        m_plan_->m_behaviour_.app_vx_ *= 1.0;
        m_plan_->m_behaviour_.app_vy_ *= 1.0;
    }

    if(selected_action_ == AtShootSituation) //踢球
    {
        ROS_INFO("AtShootSituation");
        kick_enable_ = true;
    }
    else
    {
        kick_enable_ = false;
    }

    if(selected_action_ == No_Action)
    {
        //ROS_INFO("No_Action");
        m_plan_->m_behaviour_.clearBehaviorState();
    }
}

void NormalGame::asActive()
{
    DPoint target, br;

    if(world_model_->BallInfo_[world_model_->AgentID_-1].isLocationKnown() == false)
    {
        selected_action_ = No_Action;//Cannotseeball  No_Action
    }
    else if(m_dribble_->is_dribble_)
	{
		selected_action_ = Positioned;
    }
    else if(ball_pos_.x_ > 0.0)
    {
        selected_action_ = Catch_Positioned;
    }
    else
    {
        selected_action_ = Positioned_Static;
    }

    if(selected_action_ == Positioned_Static)
    {
        m_plan_->move2Positionwithobs_noball(DPoint(0.0, ball_pos_.y_/2.0));
        m_plan_->positionAvoidObs(ball_pos_);
    }
    if(selected_action_ == Catch_Positioned) //走到球的位置并完成抓球
    {
        //ROS_INFO("robot%d Catch_Positioned", world_model_->AgentID_);
        m_plan_->catchBall();
    }
    if(selected_action_ == Positioned) //移动到目标位置 move to position 射门位置
    {
        //ROS_INFO("robot%d Positioned", world_model_->AgentID_);
        target = DPoint(FIELD_LENGTH*1.0/2+50.0, 0.0);
        br = target - robot_pos_;
        m_plan_->positionAvoidObs2(br.angle().radian_);
        if(robot_ori_.radian_<(oppGoalPosl-robot_pos_).angle().radian_ && robot_ori_.radian_ > (oppGoalPosr-robot_pos_).angle().radian_)
        {
            m_plan_->move2Positionwithobs_noball(target);
            selected_action_ = AtShootSituation;
        }
        if(robot_pos_.distance(target) < (dist_KICKGoal1+dist_KICKGoal2)/2)
        {
            selected_action_ = Actions::StaticPass;
        }
    }
    if(selected_action_ == Actions::StaticPass) //到射门位置 如果对方机器人堵住门就转向
    {
        ROS_INFO("robot%d StaticPass", world_model_->AgentID_);
        m_plan_->move2Positionwithobs_noball(DPoint(-FIELD_LENGTH*1.0/2+50.0, 0.0));
        br = DPoint(FIELD_LENGTH*1.0/2, 0.0) - robot_pos_;
        m_plan_->positionAvoidObs2(br.angle().radian_);
        if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD && velzero())
        {
            selected_action_ = AtShootSituation;
        }

        /*
        if(opp_robot_[0].y_>0.0)
        {
            br = DPoint(FIELD_LENGTH*1.0/2, (oppGoalPosr.y_-OBLE_RADIUS)/2);
            m_plan_->positionAvoidObs2(br.angle().radian_);
            if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD)
            {
                selected_action_ = AtShootSituation;
            }
        }
        else
        {
            br = DPoint(FIELD_LENGTH*1.0/2, (oppGoalPosl.y_+OBLE_RADIUS)/2);
            m_plan_->positionAvoidObs2(br.angle().radian_);
            if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD)
            {
                selected_action_ = AtShootSituation;
            }
        }*/
    }
}

void NormalGame::asAssist()
{
    DPoint target, br;
//    if(world_model_->BallInfo_[world_model_->AgentID_-1].isLocationKnown() == false)
//    {
//        selected_action_ = CanNotSeeBall;
//    }
//    else
//    {
//        if(ball_pos_.distance(opp_robot_[1]) < LIMITDRIBLLEDIS) //对方带球////
//        {
//            selected_action_ = CanNotSeeBall;
//        }
//        else
//        {
//            selected_action_ = Positioned_Static;
//        }
//    }

//    if(selected_action_ == CanNotSeeBall) //走到对方带球机器人和我方球门中间
//    {
//        ROS_INFO("CanNotSeeBall");

    if(world_model_->BallInfo_[world_model_->AgentID_-1].isLocationKnown() == false)
    {
        selected_action_ = No_Action;//Cannotseeball  No_Action
    }
    else if(m_dribble_->is_dribble_)
    {
        selected_action_ = Actions::StaticPass;
    }
    else if(ball_pos_.x_ < 0.0)
    {
        selected_action_ = Catch_Positioned;
    }
    else
    {
        selected_action_ = Positioned_Static;
    }

    if(selected_action_ == Positioned_Static)
    {
        m_plan_->move2Positionwithobs_noball(DPoint(-100.0, ball_pos_.y_/2.0));
        m_plan_->positionAvoidObs(ball_pos_);
    }
    if(selected_action_ == Catch_Positioned) //走到球的位置并完成抓球
    {
        //ROS_INFO("robot%d Catch_Positioned", world_model_->AgentID_);
        if(!(ball_pos_.x_<=-210.0 && fabs(ball_pos_.y_)<=80.0))
        {
            m_plan_->catchBall();
        }
    }
    if(selected_action_ == Actions::StaticPass) //到射门位置 如果对方机器人堵住门就转向
    {
        ROS_INFO("robot%d StaticPass", world_model_->AgentID_);
        br = DPoint(FIELD_LENGTH*1.0/2, 0.0);
        m_plan_->positionAvoidObs2(br.angle().radian_);
        if(fabs(br.angle().radian_-robot_ori_.radian_) < 30.0*DEG2RAD && velzero())
        {
            selected_action_ = AtShootSituation;
        }
    }
/*    }
    if(selected_action_ == Positioned) //我方将要射门
    {
        ROS_INFO("Positioned");

        int opp_id = -1;
        double min_dist = INF;
        double dist_opp2oppgoal;
        double dist_our2oppgoal = DPoint(FIELD_LENGTH*1.0/2, 0.0).distance(world_model_->RobotInfo_[world_model_->pass_ID_-1].getLocation());
        for(int i=0; i<world_model_->Opponents_.size(); ++i)
        {
            dist_opp2oppgoal = world_model_->Opponents_[i].distance(DPoint(FIELD_LENGTH*1.0/2, 0.0));
            if(dist_opp2oppgoal > dist_our2oppgoal && dist_opp2oppgoal < min_dist)
            {
                opp_id = i+1;
                min_dist = dist_opp2oppgoal;
            }
        }
        if(opp_id == -1)
        {
            m_plan_->move2Positionwithobs_noball(DPoint(0.0, 0.0));
            if(robot_pos_.distance(DPoint(0.0, 0.0)) < 100.0)
            {
                m_plan_->positionAvoidObs(ball_pos_);
            }
        }
        else
        {
            m_plan_->move2Positionwithobs_noball(world_model_->Opponents_[opp_id-1]);
            if(robot_pos_.distance(world_model_->Opponents_[opp_id-1]) < 20.0)
            {
                m_plan_->positionAvoidObs(ball_pos_);
            }
        }
    }
    if(selected_action_ == Positioned_Static) //移动到对方机器人和球中间
    {
        ROS_INFO("Positioned_Static");

        target = ball_pos_ + world_model_->Opponents_[m_plan_->oppneartargetid(ball_pos_)];
        target.x_ /= 2.0;
        target.y_ /= 2.0;
        if(robot_pos_.distance(target) < 10.0)
        {
            m_plan_->positionAvoidObs(ball_pos_);
            if(fabs(br.angle().radian_-robot_ori_.radian_) < 5*DEG2RAD)
            {
                selected_action_ = Catch_Positioned;
            }
        }
        else
        {
            m_plan_->move2Positionwithobs_noball(target);
        }
    }
    if(selected_action_ == Catch_Positioned) //传球用
    {
        ROS_INFO("robot%d Catch_Positioned", world_model_->AgentID_);
        br = ball_pos_ - robot_pos_;
        if(fabs(br.angle().radian_ - robot_ori_.radian_) < 15.0*DEG2RAD)
        {
            m_plan_->move2Positionwithobs_noball(ball_pos_, ConstDribbleDisFirst);
        }
        else
        {
            m_plan_->positionAvoidObs2(br.angle().radian_);
        }
        if(fabs(br.angle().radian_ - robot_ori_.radian_) < 15.0*DEG2RAD && ball_pos_.distance(robot_pos_) < ConstDribbleDisFirst)
        {
            m_plan_->positionAvoidObs2(br.angle().radian_);
            if(fabs(br.angle().radian_ - robot_ori_.radian_) < 5.0*DEG2RAD)
            {
                if(isBallHandle(world_model_->AgentID_) == false)
                {
                    m_plan_->move2Positionwithobs_noball(ball_pos_, ConstDribbleDisSecond);
                }
            }
            else
            {
                m_plan_->positionAvoidObs2(br.angle().radian_);
            }
        }
    }
*/
}

void NormalGame::asGoalie()
{
    DPoint target, br;
//    if(world_model_->BallInfo_[world_model_->AgentID_-1].isLocationKnown() == false) //找不到球
//    {
//        bool flag_oppinourfield = false;
//        for(int i=0; i<world_model_->Opponents_.size(); ++i)
//        {
//            if(world_model_->Opponents_[i].x_<0.0) //有一个敌方机器人在我方半场
//            {
//                selected_action_ = Catch_Positioned; //预测球可能到达的位置，提前等到位置进行防守
//                flag_oppinourfield = true;
//                break;
//            }
//        }
//        if(flag_oppinourfield == false) //敌方机器人均在对方半场
//        {
//            selected_action_ = CanNotSeeBall;
//        }
//    }
//    else if(isBallHandle(world_model_->AgentID_))
//    {
//        selected_action_ = Positioned_Static;
//    }
//    else
//    {
//        selected_action_ = Catch_Positioned;
//    }
    if(m_dribble_->is_dribble_)
    {
        selected_action_ = AtShootSituation;
    }
    else
    {
        if(world_model_->BallInfo_[world_model_->AgentID_-1].isLocationKnown() == false)
        {
            selected_action_ = CanNotSeeBall;
        }
//        else if(ball_pos_.x_<=-210.0 && fabs(ball_pos_.y_)<=80.0)
//        {
//            m_plan_->catchBall();
//        }
        else
        {
            selected_action_ = Positioned;
        }
    }

    if(selected_action_ == CanNotSeeBall) //看不到球，移动到中间
    {
        ROS_INFO("robot%d CanNotSeeBall", world_model_->AgentID_);
        target = DPoint(-FIELD_LENGTH*1.0/2+RADIUS-10.0, 0.0);
        m_plan_->positionAvoidObs(DPoint(0.0, 0.0));
        m_plan_->move2Positionwithobs_noball(target);
    }
    if(selected_action_ == Positioned) //防守，对方将要射门
    {
        ROS_INFO("robot%d Opp will kick", world_model_->AgentID_);

        double time_robot2ball = robot_pos_.distance(ball_pos_)/MAXVEL;
        target = DPoint(-FIELD_LENGTH*1.0/2+RADIUS-10.0, ball_pos_.y_+ball_vel_.y_*time_robot2ball);
        if(fabs(target.y_) > fabs(ourGoalPosl.y_)-RADIUS)
        {
            if(target.y_ > 0)
            {
                target.y_ = fabs(ourGoalPosl.y_)-RADIUS;
            }
            else
            {
                target.y_ = -fabs(ourGoalPosl.y_)+RADIUS;
            }
        }
        m_plan_->move2Positionwithobs_noball(target);
        m_plan_->positionAvoidObs(ball_pos_);
        ROS_INFO("target = (%f,%f)", target.x_, target.y_);
        ROS_INFO("ball_pos = (%f,%f)", ball_pos_.x_, ball_pos_.y_);
        if(m_plan_->m_behaviour_.app_vy_ <= 90.0 && m_plan_->m_behaviour_.app_vy_ >= 10.0)
        {
            m_plan_->m_behaviour_.app_vy_ = 90.0;
        }
    }
//    if(selected_action_ == Positioned_Static) //自己持球，传给主攻
//    {
//        ROS_INFO("robot%d Kick to Active", world_model_->AgentID_);
//        br = DPoint(0.0,0.0)-robot_pos_;
//        m_plan_->positionAvoidObs2(br.angle().radian_);
//        if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD)
//        {
//            selected_action_ = AtShootSituation;
//        }
//    }
}

void NormalGame::one2one()
{
    DPoint target, br;

    if(world_model_->BallInfo_[world_model_->AgentID_-1].isLocationKnown() == false)
    {
        selected_action_ = No_Action;//Cannotseeball  No_Action
    }
    else if(m_dribble_->is_dribble_)
    {
        selected_action_ = Positioned;
    }
    else
    {
        selected_action_ = Catch_Positioned;
    }

    if(selected_action_ == Catch_Positioned) //走到球的位置并完成抓球
    {
        //ROS_INFO("robot%d Catch_Positioned", world_model_->AgentID_);
        m_plan_->catchBall();
    }
    if(selected_action_ == Positioned) //移动到目标位置 move to position 射门位置
    {
        ROS_INFO("robot%d Positioned", world_model_->AgentID_);
        target = DPoint(FIELD_LENGTH*1.0/2+50.0, 0.0);
        br = target - robot_pos_;
        m_plan_->positionAvoidObs2(br.angle().radian_);
        if(robot_ori_.radian_<(oppGoalPosl-robot_pos_).angle().radian_ && robot_ori_.radian_ > (oppGoalPosr-robot_pos_).angle().radian_)
        {
            m_plan_->move2Positionwithobs_noball(target);
            selected_action_ = AtShootSituation;
        }
        if(robot_pos_.distance(target) < (dist_KICKGoal1+dist_KICKGoal2)/2)
        {
            selected_action_ = Actions::StaticPass;
        }
    }
    if(selected_action_ == Actions::StaticPass) //到射门位置 如果对方机器人堵住门就转向
    {
        ROS_INFO("robot%d StaticPass", world_model_->AgentID_);
        m_plan_->move2Positionwithobs_noball(DPoint(-FIELD_LENGTH*1.0/2+50.0, 0.0));
        br = DPoint(FIELD_LENGTH*1.0/2, 0.0) - robot_pos_;
        m_plan_->positionAvoidObs2(br.angle().radian_);
        if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD && velzero())
        {
            selected_action_ = AtShootSituation;
        }
    }
}

void NormalGame::update()
{
    //ROS_INFO("update");
    robot_pos_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getLocation();
    robot_ori_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getHead();
    ball_pos_  = world_model_->BallInfo_[world_model_->AgentID_-1].getGlobalLocation();
    ball_vel_   = world_model_->BallInfo_[world_model_->AgentID_-1].getVelocity();
    //ROS_INFO("opp=%d, obs=%d", world_model_->Opponents_.size(), world_model_->Obstacles_.size());
    if(robot_pos_.x_==0.0 && robot_pos_.y_==0.0 && ball_pos_.x_==0.0 && ball_pos_.y_==0.0)
    {
        selected_action_ = No_Action;
    }
//    if(world_model_->Opponents_.size() == 0)
//    {
//        world_model_->Opponents_.push_back(DPoint(0.0,0.0));
//    }
    if(m_dribble_->is_dribble_)
    {
        world_model_->BallInfo_[world_model_->AgentID_-1].setLocationKnown(true);
        ball_pos_ = robot_pos_;
    }

    //goalie active me
//    opp_robot_[0] = world_model_->Opponents_[m_plan_->oppneartargetid(DPoint(FIELD_LENGTH*1.0/2, 0.0))-1];
//    opp_robot_[1] = world_model_->Opponents_[m_plan_->oppneartargetid(ball_pos_)-1];
//    opp_robot_[2] = world_model_->Opponents_[m_plan_->oppneartargetid(robot_pos_)-1];

    //ROS_INFO("kick_enable=%d", kick_enable_);
    //ROS_INFO("dribble=%d", m_dribble_->is_dribble_);
    //ROS_INFO("obs_number=%d", world_model_->Obstacles_.size());
}

bool NormalGame::velzero()
{
    if((int)(m_plan_->m_behaviour_.app_w_*1400) == 0)
    {
        return true;
    }
    return false;
}

void NormalGame::init()
{
    robot_pos_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getLocation();
    robot_ori_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getHead();
    ball_pos_  = world_model_->BallInfo_[world_model_->AgentID_-1].getGlobalLocation();
    ball_vel_   = world_model_->BallInfo_[world_model_->AgentID_-1].getVelocity();
    selected_action_ = No_Action;
    kick_enable_ = false;
}
