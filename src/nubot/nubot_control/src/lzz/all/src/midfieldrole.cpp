#include "nubot/nubot_control/midfieldrole.h"
#include "nubot_common/RobotInfo.h"
#include "nubot/nubot_control/mydefine.hpp"

using namespace nubot;

MidfieldRole::MidfieldRole()
{
}

MidfieldRole::~MidfieldRole()
{
}
/*

      World_Model_Info * world_model_;
      Plan *plan_;

     // DPoint best_pass_[4];

      DPoint midfield_pos_;
      Angle  midfield_ori_;
*/

void MidfieldRole::asAssist()
{
    //接球
    plan_->move2Positionwithobs_noball(midfield_pos_);
    plan_->positionAvoidObs2(midfield_ori_.radian_);
}

void MidfieldRole::asPassiver()
{
    //盯人防守
    //if(findOppRobot())
    //{
    //    if(isObsOpp(midfield_pos_))
    //    {
    //        plan_->move2Positionwithobs_noball(midfield_pos_);
    //        plan_->positionAvoidObs2(midfield_ori_.radian_);
    //    }
    //}
    plan_->move2Positionwithobs_noball(midfield_pos_);
    plan_->positionAvoidObs2(midfield_ori_.radian_);
}

void MidfieldRole::process()
{
    world_model_->caculatePassPosition();
    midfield_pos_ = world_model_->middle_pt_;
    midfield_ori_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getHead();
    if(world_model_->RobotInfo_[world_model_->AgentID_-1].getCurrentRole() == ASSISTANT)
    {
        asAssist();
    }
    else if(world_model_->RobotInfo_[world_model_->AgentID_-1].getCurrentRole() == PASSIVE)
    {
        asPassiver();
    }
    else return;
}

bool MidfieldRole::findOppRobot()     //盯人防守，要找到对方机器人的位置
{
    return true;
}

bool MidfieldRole::isNullInTrap(const DPoint & robot_pos, const std::vector<DPoint> & obs_pts)
{
    std::vector<DPoint> pts;
    pts.clear();
    return plan_->pnpoly(robot_pos, pts, obs_pts);
}

bool MidfieldRole::isBestPass(DPoint world_pt)               //是否在最佳的站位区域
{
    //+-PI/6
    DPoint active_pos = world_model_->pass_pt_;
    Angle active_ang = world_model_->RobotInfo_[world_model_->pass_ID_-1].getHead();
    DPoint tmp = world_pt - active_pos;
    double active_radian;
    if(tmp.x_==0)
    {
        active_radian = PI/2;
    }
    else
    {
        active_radian = atan2(tmp.y_, tmp.x_);
    }
    if(fabs(fabs(active_radian) - fabs(active_ang.radian_)) <= PI/6)
    {
        return true;
    }
    return false;
}

bool MidfieldRole::isObsOpp(DPoint candidate_point)          //是否在对方的射门角度内
{
    DPoint active_pos = world_model_->lastBallPosition_;
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

bool MidfieldRole::isObsActiver(DPoint candidate_point)      //是否在我方的射门角度内
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
