#include "nubot/nubot_control/role_assignment.h"
#include<algorithm>

#include "nubot/nubot_control/mydefine.hpp"
#include <cmath>
using namespace nubot;

/*

const double PUREACTIVE_C=200;
const double PUREACTIVE_K=-200.0/MAXDIS_FIELD;
const double PUREACTIVE_THETA_C=20;
const double PUREACTIVE_THETA_K=-20/SINGLEPI_CONSTANT;

const double DISOFDEFENSE1 = FIELD_LENGTH /4.0;


    World_Model_Info * world_model_;
    float RoleUtilityMatrix_[OUR_TEAM-1][ROLENUM];
    ros::Time lastRoleChangeTime_;
    ros::Time currentRoleTime_;
*/
RoleAssignment::RoleAssignment()
{
    currentRoleTime_    = lastRoleChangeTime_ = ros::Time::now();
}

RoleAssignment::RoleAssignment(World_Model_Info & _world_model)
{
    world_model_ = & _world_model;
    currentRoleTime_    = lastRoleChangeTime_ = ros::Time::now();
}

RoleAssignment::~RoleAssignment()
{}

void RoleAssignment::calculateRoleUtility()  //计算角色效用值 P203
{
    Robot robotinfo;
    DPoint robot_pos_;
    DPoint ball_pos_;
    DPoint opp_goal = DPoint(FIELD_LENGTH/2,0.0);//敌方球门
    DPoint our_goal = DPoint(-FIELD_LENGTH/2,0.0);//己方球门
    DPoint passive_pos = DPoint(-500.0,0.0);//防守位置
    DPoint assistant_pos = DPoint(-200.0,0.0);//助攻位置

    DPoint temp1, temp2;
    double cos_theta;
    double delta_t;

    currentRoleTime_ = ros::Time::now();//当前角色时间
    for(int robot_id = 1; robot_id<OUR_TEAM; ++robot_id)//排除守门员
    {
        robotinfo = world_model_->RobotInfo_[robot_id];
        robot_pos_ = robotinfo.getLocation();
        ball_pos_ = world_model_->BallInfo_[robot_id].getGlobalLocation();

        //主攻
        temp1 = robot_pos_-ball_pos_;//机器人与球连线
        temp2 = opp_goal - ball_pos_;//敌方球门与球连线
        cos_theta = (temp1.x_*temp2.x_ + temp2.y_*temp2.y_)/(temp1.norm()*temp2.norm());
        delta_t = robotinfo.getRolePreserveTime();
        if(robotinfo.getCurrentRole() == ACTIVE) { delta_t = 0; }
        else { lastRoleChangeTime_ = currentRoleTime_; }
        RoleUtilityMatrix_[robot_id][ACTIVE] = ACTIVE_K * robot_pos_.distance(ball_pos_) + ACTIVE_B +
                ACTIVE_THETA_K * acos(cos_theta) + min(ACTIVE_THETA_B, delta_t);

        //防守
        temp2 = our_goal - ball_pos_;//己方球门与球连线
        cos_theta = (temp1.x_*temp2.x_ + temp2.y_*temp2.y_)/(temp1.norm()*temp2.norm());
        delta_t = robotinfo.getRolePreserveTime();
        if(robotinfo.getCurrentRole() == PASSIVE) { delta_t = 0; }
        else { lastRoleChangeTime_ = currentRoleTime_; }
        RoleUtilityMatrix_[robot_id][PASSIVE] = PASSIVE_K * robot_pos_.distance(passive_pos) + PASSIVE_B +
                PASSIVE_THETA_K * acos(cos_theta) + min(PASSIVE_THETA_B, delta_t);

        //助攻
        delta_t = robotinfo.getRolePreserveTime();
        if(robotinfo.getCurrentRole() == ASSISTANT){ delta_t = 0; }
        else { lastRoleChangeTime_ = currentRoleTime_; }
        RoleUtilityMatrix_[robot_id][ASSISTANT] = ASSISTANT_K * robot_pos_.distance(assistant_pos) + ASSISTANT_B +
                min(PASSIVE_THETA_B, delta_t);

        world_model_->RobotInfo_[robot_id].setRolePreserveTime(currentRoleTime_.sec -lastRoleChangeTime_.sec);
    }
}

void RoleAssignment::adjustRole(){}

int RoleAssignment::selectRole()  //选择角色
{
    double selected[OUR_TEAM-1] = {-1};
    while(1)
    {
        double maxn=-1;
        int robot_id=-1;//robot_id +1 = Agent_id
        int role_id=-1;
        for(int i=0; i<OUR_TEAM-1; ++i)
        {
            for(int j=0; j<ROLENUM; ++i)
            {
                if(RoleUtilityMatrix_[i][j] > maxn)
                {
                    maxn=RoleUtilityMatrix_[i][j];
                    robot_id=i;
                    role_id=j;
                }
            }
        }
        if(robot_id == -1)
            break;
        else
        {
            if(selected[robot_id] < maxn)
            {
                if(world_model_->AgentID_-1 == robot_id)
                {
                    return role_id;
                }
                selected[robot_id] = maxn;
            }
            else
            {
                continue;
            }
        }
        for(int i=0; i<OUR_TEAM-1; ++i) { RoleUtilityMatrix_[i][role_id]  = -1; }
        for(int j=0; j<ROLENUM;    ++j) { RoleUtilityMatrix_[robot_id][j] = -1; }
    }
}

void RoleAssignment::fixRole(){}
int  RoleAssignment::process()
{
    memset(RoleUtilityMatrix_, -1, (OUR_TEAM-1)*ROLENUM);
    calculateRoleUtility();
    return selectRole();
    //
}
