#include "nubot/nubot_control/strategy.hpp"
#include "nubot/nubot_control/mydefine.hpp"

using namespace nubot;
/*
public:
    Strategy();
    Strategy(World_Model_Info & _world_model, Plan & _plan);
    ~Strategy();
   void selectRole();
   void selectAction();
   void process();
   bool passStrategy();
public:
    RoleAssignment    RoleAssignment_;
    World_Model_Info * world_model_;
    int selected_role_;
    int selected_action_;
    Plan * m_plan_;
    ActiveRole        ActiveRole_;
    AssistRole        AssistRole_;
    PassiveRole       PassiveRole_;
    MidfieldRole      MidfieldRole_;
    GoalieStrategy    goalie_strategy_;
    bool  auto_competition;
};
    */
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
    Robot robotinfo;
    for(int robot_id=1; robot_id<OUR_TEAM; ++robot_id)
    {
        robotinfo = world_model_->RobotInfo_[robot_id];
        if(robotinfo.isStuck())
        {
            robotinfo.setCurrentAction(Stucked);
            continue;
        }
    }
}

void Strategy::process()
{
    /*
    DPoint target=DPoint(-100,-100);
    m_plan_->move2Positionwithobs_noball(target);
    //m_plan_->roundfoot();*/

    /*
    enum StrategyTyples {  STRATEGY_ATTACK = 0,
                           STRATEGY_DEFEND = 1,
                           STRATEGY_BALANCE =2,
                           STRATEGY_AUTO = 4,
                           NOSTRATEG =5
    };*/
    if(world_model_->AgentID_==1) { goalie_strategy_.strategy(); }
    else
    {
        selectRole();
        Robot robotinfo = world_model_->RobotInfo_[world_model_->AgentID_-1];
        if(robotinfo.getCurrentRole() == ACTIVE) { ActiveRole_.process(); }
        else if(robotinfo.getCurrentRole() == ASSISTANT) { AssistRole_.process(); }
        else if(robotinfo.getCurrentRole() == PASSIVE) { PassiveRole_.process(); }
        else if(robotinfo.getCurrentRole() == MIDFIELD) { MidfieldRole_.process(); }
    }
}

bool Strategy::passStrategy(){}
