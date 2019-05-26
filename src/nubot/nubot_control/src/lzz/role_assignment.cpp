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

void RoleAssignment::calculateRoleUtility()//P203
{
    Robot robotinfo;
    BallObject ballinfo;
    DPoint robot_pos_;
    DPoint ball_pos_;
    DPoint opp_goal = DPoint(FIELD_LENGTH/2,0);//敌方球门
    DPoint our_goal = DPoint(-FIELD_LENGTH/2,0);//己方球门
    DPoint passive_pos = DPoint(-500,0);//防守位置
    DPoint assistant_pos = DPoint(-200,0);//助攻位置

    DPoint temp1, temp2;
    double cos_theta;
    double Uakd,Uakt,Uakb,delta_tak;//主攻
    double Udfd,Udft,delta_tdf,Udfb;//防守
    double Uad,delta_ta,Uab;//助攻

    for(int robot_id = 1; robot_id<OUR_TEAM; ++i)//排除守门员
    {
        robotinfo = world_model_->RobotInfo_[robot_id];
        ballinfo = world_model_->BallInfo_[robot_id];
        robot_pos_ = robotinfo.getLocation();
        ball_pos_ = ballinfo.getGlobalLocation();

        //主攻
        Uakd = ACTIVE_K * robot_pos_.distance(ball_pos_) + ACTIVE_B;
        temp1 = robot_pos_-ball_pos_;//机器人与球连线
        temp2 = opp_goal - ball_pos_;//机器人与敌方球门连线
        cos_theta = (temp1.x_*temp2.x_ + temp2.y_*temp2.y_)/(temp1.norm()*temp2.norm());
        Uakt = ACTIVE_THETA_K * acos(cos_theta);
        delta_tak = robotinfo.getRolePreserveTime();
        if(robotinfo.getCurrentRole() == ACTIVE)
            delta_tak = 0;
        Uakb = min(ACTIVE_THETA_B, delta_tak);
        RoleUtilityMatrix_[robot_id][ACTIVE] = Uakd + Uakt + Uakb;

        //防守
        Udfd = PASSIVE_K * robot_pos_.distance(passive_pos) + PASSIVE_B;
        temp2 = our_goal - ball_pos_;
        cos_theta = (temp1.x_*temp2.x_ + temp2.y_*temp2.y_)/(temp1.norm()*temp2.norm());
        Udft = PASSIVE_THETA_K * acos(cos_theta);
        delta_tdf = robotinfo.getRolePreserveTime();
        if(robotinfo.getCurrentRole() == PASSIVE)
            delta_tdf = 0;
        Udfb = min(PASSIVE_THETA_B, delta_tdf);
        RoleUtilityMatrix_[robot_id][PASSIVE] = Udfd + Udft + Udfb;

        //助攻
        Uad = ASSISTANT_K * robot_pos_.distance(assistant_pos) + ASSISTANT_B;
        delta_ta = robotinfo.getRolePreserveTime();
        if(robotinfo.getCurrentRole() == ASSISTANT)
            delta_ta = 0;
        Uab = min(PASSIVE_THETA_B, delta_ta);
        RoleUtilityMatrix_[robot_id][ASSISTANT] = Uad + Uab;
    }
}

void adjustRole();

void RoleAssignment::selectRole()
{
    while(1)
    {
        double maxn=-1;
        int robot_id=-1;//result +1 = agent id
        int role_id=-1;
        for(int i=0; i<OUR_TEAM-1; ++i)
        {
            for(int j=0; j<ROLENUM; ++i)
            {
                if(RoleUtilityMatrix_[i][j]>maxn)
                {
                    maxn=RoleUtilityMatrix_[i][j];
                    robot_id=i;
                    role_id=j;
                }
            }
        }
        if(robot_id==-1)
            break;
        else
        {
            Robot robotinfo=world_model_->RobotInfo_[robot_id];
            robotinfo.setCurrentRole((char)role_id);///需根据core.hpp中的role进行修改
            world_model_->RobotInfo_[robot_id]=robotinfo;
        }
        for(int i=0; i<OUR_TEAM-1; ++i)
        {
            RoleUtilityMatrix_[i][role_id]=-1;
        }
        for(int j=0; j<ROLENUM; ++j)
        {
            RoleUtilityMatrix_[robot_id][j]=-1;
        }
    }
}

void fixRole();
int  process();
