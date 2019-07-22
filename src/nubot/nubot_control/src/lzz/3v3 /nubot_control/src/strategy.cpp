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
    selected_role_ = RoleAssignment_.process();
}

void Strategy::selectAction()
{/*
    enum Actions
    {
        Stucked =0,
        Penalty =1, //惩罚
        CanNotSeeBall =2,
        SeeNotDribbleBall =3,
        TurnForShoot =4, // Active 踢球准备
        AtShootSituation =5,  //踢球
        TurnToPass=6,  //Dynamic pass 走到传球位置
        StaticPass =7, //
        AvoidObs=8,
        Catch_Positioned = 9,   //catch position 走到球的位置并完成抓球
        Positioned =10,          //move to position 移动到目标位置
        Positioned_Static  =11, //static 走到射门位置
        No_Action  =12,
    };*/
    if(world_model_->RobotInfo_[world_model_->AgentID_-1].isStuck())
    {
        selected_action_ = Stucked;
    }
    else
    {
        selected_action_ = Positioned;
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
    ROS_INFO("robot%d Stragety", world_model_->AgentID_);
    if(world_model_->AgentID_ != 1)
    {
        m_plan_->update();
        world_model_->caculatePassPosition();
        selectRole();
        selectAction();
        ROS_INFO("robot%d selected_role_: %d", world_model_->AgentID_, selected_role_);
        switch (selected_role_)
        {
        case ACTIVE :
            ActiveRole_.process();
            break;
        case ASSISTANT :
            AssistRole_.process();
            break;
        case PASSIVE :
            PassiveRole_.process();
            break;
        case MIDFIELD :
            MidfieldRole_.process();
            break;
        default :
            break;
        }
    }
}

bool Strategy::passStrategy(){}
