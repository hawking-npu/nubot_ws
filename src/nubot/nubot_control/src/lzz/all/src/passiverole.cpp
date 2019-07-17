#include "nubot/nubot_control/passiverole.h"

using namespace nubot;

PassiveRole::PassiveRole()
{
}

PassiveRole::~PassiveRole()
{
}

/*

World_Model_Info * world_model_;
Plan *plan_;

bool isInOurfeild_;                     //用于站位决策的两个因素
bool IsOurDribble_;

DPoint defence_pos_;
Angle  defence_ori_;
*/
void PassiveRole::process()
{
    isInOurfeild_ = plan_->inourfield_;
    IsOurDribble_ = world_model_->IsOurDribble_;

    passiveCalculate();

    plan_->move2Positionwithobs_noball(defence_pos_);
}

void PassiveRole::passiveCalculate()                                //新的站位点计算
{
    defence_pos_ = world_model_->passive_pt_;
}

void PassiveRole::findPointOut(Angle ang_goal2ball)                  //找到这个方向上出禁区的点
{}

void PassiveRole::awayFromActive()
{}
