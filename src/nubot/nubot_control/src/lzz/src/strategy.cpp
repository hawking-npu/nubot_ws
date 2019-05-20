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

void selectRole();
void selectAction();
void Strategy::process()
{
    DPoint target=DPoint(0,-200);
    m_plan_->move2Positionwithobs_noball(target,max_vel,max_acc);
    //m_plan_->roundfoot();
}

bool passStrategy();
