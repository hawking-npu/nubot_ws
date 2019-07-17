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
    if(world_model_->AgentID_ == 1)
    {
        nubot_common::RobotInfo robotinfo = goalie_strategy_.robot_info_;
        Robot robot = world_model_->RobotInfo_[0];
        robotinfo.pos.x = robot.getLocation().x_, robotinfo.pos.y = robot.getLocation().y_;
        robotinfo.AgentID = 1;
        robotinfo.current_role = robot.getCurrentRole();
        //robotinfo.header
        robotinfo.heading.theta = robot.getHead().radian_;
        //robotinfo.isdribble
        robotinfo.iskick = robot.isKickoff();
        robotinfo.isstuck = robot.isStuck();
        robotinfo.isvalid = robot.isValid();
        //robotinfo.Ptr
        robotinfo.role_time = robot.getRolePreserveTime();
        robotinfo.staticcatchNum = robot.getcatchNum();
        robotinfo.staticpassNum = robot.getpassNum();
        robotinfo.target.x = robot.getTarget().x_, robotinfo.target.y = robot.getTarget().y_;
        robotinfo.targetNum1 = robot.getTargetNum(1);
        robotinfo.targetNum2 = robot.getTargetNum(2);
        robotinfo.targetNum3 = robot.getTargetNum(3);
        robotinfo.targetNum4 = robot.getTargetNum(4);
        //robotinfo.Type
        //robotinfo.vrot
        //robotinfo.vtrans
        goalie_strategy_.robot_info_ = robotinfo;

        DPoint robot_pos_ = robot.getLocation();
        goalie_strategy_.strategy();
        if(!world_model_->field_info_.isOurGoal(robot_pos_))
        {
            goalie_strategy_.state_ = goalie_strategy_.Move2Origin;
        }
    }
    else
    {
        world_model_->caculatePassPosition();
        selectRole();
        selectAction();
        m_plan_->update();
        if(world_model_->RobotInfo_[world_model_->AgentID_-1].getCurrentRole() == ACTIVE)
        {
            ActiveRole_.process();
        }
        else if(world_model_->RobotInfo_[world_model_->AgentID_-1].getCurrentRole() == ASSISTANT)
        {
            AssistRole_.process();
        }
        else if(world_model_->RobotInfo_[world_model_->AgentID_-1].getCurrentRole() == PASSIVE)
        {
            PassiveRole_.process();
        }
        else if(world_model_->RobotInfo_[world_model_->AgentID_-1].getCurrentRole() == MIDFIELD)
        {
            MidfieldRole_.process();
        }
    }
}

bool Strategy::passStrategy(){}
