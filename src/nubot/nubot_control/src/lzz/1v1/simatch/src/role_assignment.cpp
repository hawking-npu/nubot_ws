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
    isAssigned = false;
}

RoleAssignment::RoleAssignment(World_Model_Info & _world_model)
{
    world_model_ = & _world_model;
    currentRoleTime_    = lastRoleChangeTime_ = ros::Time::now();
    isAssigned = false;
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
    isAssigned = true;

    currentRoleTime_ = ros::Time::now();//当前角色时间
    //ROS_INFO("robot%d RoleAssignment 2", world_model_->AgentID_);
    for(int robot_id = 1; robot_id<OUR_TEAM; ++robot_id)//排除守门员
    {
        robotinfo = world_model_->RobotInfo_[robot_id];
        robot_pos_ = robotinfo.getLocation();
        ball_pos_ = world_model_->BallInfo_[robot_id].getGlobalLocation();
        if(robot_pos_.x_ == 0.0 && robot_pos_.y_ == 0.0 && ball_pos_.x_ == 0.0 && ball_pos_.y_ == 0.0)
        {
            //ROS_INFO("robot%d RoleAssignment false isAssigned, %d, (%d, %d)", world_model_->AgentID_, robot_id, world_model_->RobotInfo_[robot_id].getLocation().x_, world_model_->RobotInfo_[robot_id].getLocation().y_);
            //printf("robot%d RoleAssignment false isAssigned, %d, (%d, %d)\n", world_model_->AgentID_, robot_id, world_model_->RobotInfo_[robot_id].getLocation().x_, world_model_->RobotInfo_[robot_id].getLocation().y_);
            isAssigned = false;
            return;
        }

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
    }
    ROS_INFO("robot%d RoleUtilityMatrix_ :", world_model_->AgentID_);
    for(int i=1; i<OUR_TEAM; ++i)
    {
        cout<<"robot"<<world_model_->AgentID_<<' '<<RoleUtilityMatrix_[i][ACTIVE]<<' '<<RoleUtilityMatrix_[i][PASSIVE]<<' '<<RoleUtilityMatrix_[i][ASSISTANT]<<endl;
    }
}

void RoleAssignment::adjustRole(){}

int RoleAssignment::selectRole()  //选择角色
{
    ROS_INFO("robot%d RoleAssignment 5", world_model_->AgentID_);
    if(isAssigned == true)
        ROS_INFO("robot%d isAssigned: true", world_model_->AgentID_);
    else if(isAssigned == false)
        ROS_INFO("robot%d isAssigned: false", world_model_->AgentID_);
    if(isAssigned == false)
    {
        return NOROLE;
    }
    ROS_INFO("robot%d RoleAssignment 10", world_model_->AgentID_);
    double selected[OUR_TEAM] = {-1};
    int selected_role = NOROLE;
    ROS_INFO("robot%d RoleAssignment 11", world_model_->AgentID_);
    while(1)
    {
        ROS_INFO("robot%d RoleAssignment 12", world_model_->AgentID_);
        double maxn=-1;
        int robot_id=-1;//robot_id +1 = Agent_id
        int role_id=-1;
        for(int i=1; i<OUR_TEAM-1; ++i)
        {
            if(RoleUtilityMatrix_[i][ACTIVE] > maxn)
            {
                maxn=RoleUtilityMatrix_[i][ACTIVE];
                robot_id=i;
                role_id=ACTIVE;
                ROS_INFO("robot%d RoleAssignment 6 role_id: %d", world_model_->AgentID_, role_id);
            }
            if(RoleUtilityMatrix_[i][ASSISTANT] > maxn)
            {
                maxn=RoleUtilityMatrix_[i][ASSISTANT];
                robot_id=i;
                role_id=ASSISTANT;
                ROS_INFO("robot%d RoleAssignment 6 role_id: %d", world_model_->AgentID_, role_id);
            }
            if(RoleUtilityMatrix_[i][PASSIVE] > maxn)
            {
                maxn=RoleUtilityMatrix_[i][PASSIVE];
                robot_id=i;
                role_id=PASSIVE;
                ROS_INFO("robot%d RoleAssignment 6 role_id: %d", world_model_->AgentID_, role_id);
            }
        }
        if(robot_id == -1)
            continue;
        else
        {
            if(selected[robot_id] < maxn)
            {
                if(world_model_->AgentID_-1 == robot_id)
                {
                    selected_role = role_id;
                }
                selected[robot_id] = maxn;
            }
            else
            {
                continue;
            }
        }
        for(int i=1; i<OUR_TEAM-1; ++i) { RoleUtilityMatrix_[i][role_id]  = -1; }
        RoleUtilityMatrix_[robot_id][ACTIVE] = -1;
        RoleUtilityMatrix_[robot_id][PASSIVE] = -1;
        RoleUtilityMatrix_[robot_id][ASSISTANT] = -1;
    }
    return selected_role;
}

void RoleAssignment::fixRole(){}
int  RoleAssignment::process()
{
    ROS_INFO("robot%d RoleAssignment 3", world_model_->AgentID_);
    memset(RoleUtilityMatrix_, -1, (OUR_TEAM-1)*ROLENUM);
    calculateRoleUtility();
    return selectRole();
    //
}
