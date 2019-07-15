#include "nubot/nubot_control/strategy.hpp"
#include "nubot/nubot_control/mydefine.hpp"

using namespace nubot;

Strategy::Strategy(World_Model_Info & _world_model, Plan & _plan){
    world_model_ = & _world_model;
    m_plan_ = & _plan;
    RoleAssignment_.world_model_ = world_model_;

    ActiveRole_.m_plan_ = m_plan_;
    ActiveRole_.world_model_ = world_model_;

    AssistRole_.world_model_ = world_model_;
    AssistRole_.plan_ = m_plan_;

    PassiveRole_.world_model_ = world_model_;
    PassiveRole_.plan_ = m_plan_;

    MidfieldRole_.world_model_ = world_model_;
    MidfieldRole_.plan_ = m_plan_;
    selected_role_ = NOROLE;
    selected_action_ = Positioned;
}

Strategy::Strategy(){
    RoleAssignment_.world_model_ = world_model_;
    selected_role_ = NOROLE;
}
Strategy::~Strategy(){
}
void Strategy::selectRole()  //选择角色 调用RoleAssignment_
{
    /*
    enum Roles{GOALIE = 0 ,//守门员
               ACTIVE = 1,//主攻
               PASSIVE = 2,//防守
               MIDFIELD = 3,//中场队员
               ASSISTANT = 4,//助攻
               ACIDPASSIVE = 5,
               GAZER = 6 ,
               BLOCK = 7,//堵塞
               NOROLE = 8,//没有角色
               CATCHOFPASS = 9,//接球
               PASSOFPASS =10//传球
              };
    */
    RoleAssignment_.process();
    selected_role_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getCurrentRole();
}

void Strategy::selectAction()
{/*
    enum Actions
    {
        Stucked =0,
        Penalty =1,
        CanNotSeeBall =2,
        SeeNotDribbleBall =3,
        TurnForShoot =4, // Active
        AtShootSituation =5,
        TurnToPass=6,   //Dynamic pass
        StaticPass =7,
        AvoidObs=8,
        Catch_Positioned = 9,   //catch position
        Positioned =10,          //move to position
        Positioned_Static  =11, //static
        No_Action  =12,
    };*/
    for(int robot_id=1; robot_id<OUR_TEAM; ++robot_id)
    {
        if(world_model_->RobotInfo_[robot_id].isStuck())
        {
            world_model_->RobotInfo_[robot_id].setCurrentAction(Stucked);
            continue;
        }
    }
}

void Strategy::process()
{
    /*
    DPoint target=DPoint(-100,-100);
    m_plan_->move2Positionwithobs_noball(target);
    //m_plan_->ballRoundTrack();*/

    /*
    enum StrategyTyples {  STRATEGY_ATTACK = 0,
                           STRATEGY_DEFEND = 1,
                           STRATEGY_BALANCE =2,
                           STRATEGY_AUTO = 4,
                           NOSTRATEG =5
    };*/
    if(world_model_->AgentID_==1)
    {
        goalie_strategy_.robot_info_ = world_model_->RobotInfo_[0];
        DPoint robot_pos_ = world_model_->RobotInfo_[0].getLocation();
        DPoint ball_pos_  = world_model_->BallInfo_[0].getGlobalLocation();
        Angle  robot_ori_ = world_model_->RobotInfo_[0].getHead();
        goalie_strategy_.strategy();
        if(goalie_strategy_.state_ == goalie_strategy_.Move2Ball)
        {
            m_plan_->move2Positionwithobs_noball(goalie_strategy_.dest_point_);
            m_plan_->positionAvoidObs2(goalie_strategy_.dest_angle_);
        }
        else if(goalie_strategy_.state_ == goalie_strategy_.Move2Origin)
        {
            m_plan_->move2Positionwithobs_noball(DPoint(GOAL_POS_X*1.0, GOAL_POS_Y*1.0));
            m_plan_->positionAvoidObs2(0.0);
        }
        else if(goalie_strategy_.state_ == goalie_strategy_.Turn2Ball)
        {
            DPoint tmp = ball_pos_ - robot_pos_;
            m_plan_->positionAvoidObs2(atan2(tmp.x_, tmp.y_));
        }
        else if(goalie_strategy_.state_ == goalie_strategy_.CatchBall)
        {
            nubot_common::BallHandle    dribble;
            DPoint br = ball_pos_ - robot_pos_;

            if(ball_pos_.distance(robot_pos_) > 300.0)
            {
                goalie_strategy_.state_ = goalie_strategy_.StandBy;
            }
            else
            {
                m_plan_->positionAvoidObs2(br.angle().radian_);
                if(fabs(br.angle().radian_-robot_ori_.radian_) < 8.0/180.0)        // 先往足球靠近
                {
                    m_plan_->move2Positionwithobs_noball(ball_pos_, max_vel, max_acc, false, 50.0);
                    if(ball_pos_.distance(robot_pos_) < 50.0)
                    {
                        dribble.request.enable = 1;
                        ballhandle_client_.call(dribble);
                        if(dribble.response.BallIsHolding != true)
                        {
                            m_plan_->move2Positionwithobs_noball(ball_pos_, max_vel, max_acc, false, 40.0);
                            if(ball_pos_.distance(robot_pos_) < 40.0)
                            {
                                m_plan_->positionAvoidObs2(br.angle().radian_);
                            }
                        }
                        else
                        {
                            goalie_strategy_.state_ = goalie_strategy_.KickBall;
                        }
                    }
                }
            }
        }
        else if(goalie_strategy_.state_ == goalie_strategy_.KickBall)
        {
            nubot_common::BallHandle    dribble;
            DPoint br = ball_pos_ - robot_pos_;

            m_plan_->positionAvoidObs2(br.angle().radian_);
            if(fabs(br.angle().radian_-robot_ori_.radian_) < 8.0/180.0)        // 先往足球靠近
            {
                if(move2target(ball_pos_, robot_pos_, 50.0))
                {
                    dribble.request.enable = 1;
                    ballhandle_client_.call(dribble);
                    if(dribble.response.BallIsHolding != true)
                    {
                        if(move2target(ball_pos_, robot_pos_, 40.0))
                            move2ori(br.angle().radian_, robot_ori_.radian_);
                    }
                    else        // 带上球了
                    {
                        DPoint dirc = DPoint(0.0, 0.0) - robot_pos_;         // 对准 (0.0 ,0.0)
                        double id2goalie = -1;
                        double min_dist = INF;
                        DPoint this_robot_pos;
                        for(int i=1; i<OUR_TEAM; ++i)
                        {
                            this_robot_pos = world_model_info_.RobotInfo_[i].getLocation();
                            if(robot_pos_.distance(this_robot_pos) < min_dist)
                            {
                                id2goalie = i+1;
                            }
                        }
                        if(id2goalie != -1)//find
                        {
                            dirc = world_model_info_.RobotInfo_[id2goalie].getLocation() - robot_pos_;
                        }
                        if(move2ori(dirc.angle().radian_, robot_ori_.radian_, 5.0*DEG2RAD))  // 跑到位以及转到位
                        {
                            nubot_common::Shoot shoot;
                            shoot.request.ShootPos = FLY;
                            shoot.request.strength = 1.0;   // 在FLY模式下，strength不重要,只要非零就行
                            shoot_client_.call(shoot);
                            goalie_strategy_.state_ = goalie_strategy_.Move2Origin;
                        }
                    }
                }
            }
        }
        else
        {
            m_plan_->move2Positionwithobs_noball(robot_pos_);
            m_plan_->positionAvoidObs2(world_model_->RobotInfo_[world_model_->AgentID_-1].getHead().radian_);
        }
    }
    else
    {
        selectRole();
        Robot robotinfo = world_model_->RobotInfo_[world_model_->AgentID_-1];
        if(robotinfo.getCurrentRole() == ACTIVE) { ActiveRole_.process(); }
        else if(robotinfo.getCurrentRole() == ASSISTANT) { AssistRole_.process(); }
        else if(robotinfo.getCurrentRole() == PASSIVE) { PassiveRole_.process(); }
        else if(robotinfo.getCurrentRole() == MIDFIELD) { MidfieldRole_.process(); }
    }

    //m_plan_->update();
}

bool Strategy::passStrategy(){}
