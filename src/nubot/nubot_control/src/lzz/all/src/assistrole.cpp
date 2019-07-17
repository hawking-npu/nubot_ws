#include "nubot/nubot_control/assistrole.h"
#include "nubot_common/RobotInfo.h"

using namespace nubot;

AssistRole::AssistRole()
{
}

AssistRole::~AssistRole()
{
}

/*
World_Model_Info * world_model_;
Plan *plan_;

bool isInOurfeild_;                     //用于站位决策的两个因素
bool IsOurDribble_;  //球是否在我方手中

//DPoint best_pass_[4];
*/

void AssistRole::process()     //根据当前状态选择需要执行的动作,现在只考虑了站位
{
    isInOurfeild_ = plan_->inourfield_;
    IsOurDribble_ = world_model_->IsOurDribble_;
    assistCalculate();
}

void AssistRole::assistCalculate()
{
    ///catch_sight 2 ball
    /////world_model_->assist_pt_;
    if(IsOurDribble_)
    {
        if(isInOurfeild_)
        {
            if(isBestPass(plan_->robot_pos_))
            {
                plan_->catchBallSlowly();
            }
            else
            {
                plan_->move2Positionwithobs_noball(world_model_->catch_pt_);
            }
        }
        else if(isObsActiver(plan_->robot_pos_))
        {
            if(plan_->robot_pos_.y_ > 0)
            {
                plan_->move2Positionwithobs_noball(DPoint(plan_->robot_pos_.x_, plan_->robot_pos_.y_-100.0));
            }
            else
            {
                plan_->move2Positionwithobs_noball(DPoint(plan_->robot_pos_.x_, plan_->robot_pos_.y_+100.0));
            }
        }
    }

}

bool AssistRole::isBestPass(DPoint world_pt)       //是否在最佳的接球区域
{
    //+-PI/6
    DPoint active_pos = world_model_->pass_pt_;
    Angle active_ang = world_model_->RobotInfo_[world_model_->pass_ID_-1].getHead();
    DPoint tmp = world_pt - active_pos;
    double active_radian;
    active_radian = atan2(tmp.y_, tmp.x_);
    if(fabs(fabs(active_radian) - fabs(active_ang.radian_)) <= PI/6)
    {
        return true;
    }
    return false;
}

bool AssistRole::isObsActiver(DPoint candidate_point/* 候选*/)     //是否阻碍到主攻射门
{
    DPoint active_pos = world_model_->pass_pt_;
    if(fabs(candidate_point.x_-active_pos.x_) < EPS)
        return false;
    DPoint act2oppgl = DPoint(oppGoalPosl.x_-active_pos.x_, oppGoalPosl.y_-active_pos.y_);
    DPoint act2oppgr = DPoint(oppGoalPosr.x_-active_pos.x_, oppGoalPosr.y_-active_pos.y_);
    DPoint act2candi = DPoint(candidate_point.x_-active_pos.x_, candidate_point.y_-active_pos.y_);
    double radian_oppl = atan2(act2oppgl.y_, act2oppgl.x_);
    double radian_oppr = atan2(act2oppgr.y_, act2oppgr.x_);
    double radian_a2c = atan2(act2candi.y_, act2candi.x_);
    if((radian_a2c < max(radian_oppl, radian_oppr) ) && (radian_a2c > min(radian_oppl, radian_oppr) ))
        return true;
    return false;
}

bool AssistRole::isNullInTrap(const DPoint & robot_pos, const std::vector<DPoint> & obs_pts)          //周围是否有障碍物
{
    std::vector<DPoint> pts;
    pts.clear();
    return plan_->pnpoly(robot_pos, pts, obs_pts);
}
