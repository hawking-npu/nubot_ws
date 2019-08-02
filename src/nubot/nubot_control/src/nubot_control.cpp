#include "nubot/core/core.hpp"

#include <nubot_common/VelCmd.h>
#include <nubot_common/WorldModelInfo.h>
#include <nubot_common/BallInfo3d.h>
#include <nubot_common/BallHandle.h>
#include <nubot_common/Shoot.h>
#include <nubot_common/StrategyInfo.h>
#include <nubot_common/TargetInfo.h>
#include <nubot_common/OdoInfo.h>
#ifndef SIMULATION
#include <nubot_control/nubotcontrolConfig.h>
#endif
#include <dynamic_reconfigure/server.h>
#include <std_msgs/Header.h>
#include <std_msgs/String.h>

#include <iostream>
#include <stdio.h>
#include <ros/ros.h>
#include <nubot/nubot_control/world_model_info.h>
#include <nubot/nubot_control/strategy.hpp>
#include <nubot/nubot_control/common.hpp>
#include <nubot/nubot_control/plan.h>
#include <nubot/nubot_control/staticpass.h>
#include <opencv2/opencv.hpp>

#include <boost/circular_buffer.hpp>
#include <nubot/nubot_control/location.h>
#include <nubot/nubot_control/avoidance.h>
#include <nubot/nubot_control/passing.h>
#include <nubot/nubot_control/dribblestate.hpp>

const int ROLEMAXTIME = 500;
const int maxknowtime = 1.0*1000/update_T;
const int MODE = LOCATION;

using namespace std;
namespace nubot{

class NuBotControl
{

public:
    ros::Subscriber  ballinfo3d_sub1_;
    ros::Subscriber  odoinfo_sub_;
    ros::Subscriber  obstaclesinfo_sub_;
    ros::Subscriber  worldmodelinfo_sub_;

    ros::ServiceClient ballhandle_client_;
    ros::ServiceClient shoot_client_;

    ros::Publisher   motor_cmd_pub_;
    ros::Publisher   strategy_info_pub_;
    ros::Timer       control_timer_;

    boost::shared_ptr<ros::NodeHandle> nh_;

    boost::circular_buffer<DPoint> past_ball_vel;
    boost::circular_buffer<nubot_common::VelCmd> past_robot_vel;
public:
    World_Model_Info world_model_info_;  /** 世界模型中的信息赋值，来源于world_model节点的topic*/
    Strategy  * m_strategy_;
    Plan   m_plan_;
    StaticPass m_staticpass_;

    double kp_;
    double kalpha_;
    double kbeta_;
    char    match_mode_;
    char    pre_match_mode_;
    DPoint  robot_pos_;
    Angle  robot_ori_;
    DPoint  ball_pos_;
    DPoint  ball_vel_;
    DPoint  robot_vel_;
    DPoint  past_ball_pos_;
    DPoint  opp_robot_;
    bool isdribble[10];
    int roletime;
    int defender_ID_;
    int flag;
    bool past_ball_known;
    int known_time;

    DribbleState m_dribble_;
    /// Challenge
    Location * m_location_;
    Avoidance * m_avoidance_;
    Passing * m_passing_;


public:
    NuBotControl(int argc, char **argv)
    {
        const char * environment;
        ROS_INFO("initialize control process");

#ifdef SIMULATION
    std::string robot_name = argv[1];
    std::string num = robot_name.substr(robot_name.size()-1);
    //std::string robot_prefix = robot_name.substr(0,robot_name.size()-1);
    environment = num.c_str();
    ROS_FATAL("robot_name:%s",robot_name.c_str());
    nh_ = boost::make_shared<ros::NodeHandle>(robot_name);
#else
        nh_ = boost::make_shared<ros::NodeHandle>();
        // 读取机器人标号，并赋值. 在 .bashrc 中输入export AGENT=1，2，3，4，等等；
        if((environment = getenv("AGENT"))==NULL)
        {
            ROS_ERROR("this agent number is not read by robot");
            return ;
        }
#endif

        motor_cmd_pub_ = nh_->advertise<nubot_common::VelCmd>("nubotcontrol/velcmd",10);//1
        strategy_info_pub_ =  nh_->advertise<nubot_common::StrategyInfo>("nubotcontrol/strategy",10);
        ballhandle_client_ =  nh_->serviceClient<nubot_common::BallHandle>("BallHandle");
        shoot_client_ = nh_->serviceClient<nubot_common::Shoot>("Shoot");
        ballinfo3d_sub1_    = nh_->subscribe("stereovision/StereoBallInfo",1, &NuBotControl::ballInfo3dCallback, this);
#ifndef SIMULATION
        odoinfo_sub_        = nh_->subscribe("nubotdriver/odoinfo", update_T/1000, &NuBotControl::update_my_dribble_info,this);
#endif
        worldmodelinfo_sub_ = nh_->subscribe("worldmodel/worldmodelinfo", 1, &NuBotControl::update_world_model_info,this);
        control_timer_      = nh_->createTimer(ros::Duration(update_T/1000),&NuBotControl::loopControl,this);
#ifndef SIMULATION
        dynamic_reconfigure::Server<nubot_control::nubotcontrolConfig> reconfigureServer_;
        reconfigureServer_.setCallback(boost::bind(&NuBotControl::configure, this, _1, _2));
#endif
        world_model_info_.AgentID_ = atoi(environment); /** 机器人标号*/
        world_model_info_.CoachInfo_.MatchMode = STOPROBOT;
        m_plan_.world_model_ =  & world_model_info_;
        m_plan_.m_subtargets_.world_model_ =  & world_model_info_;
        m_plan_.m_dribble_ = & m_dribble_;
        m_staticpass_.world_model_= & world_model_info_;
        m_strategy_ = new Strategy(world_model_info_,m_plan_);

        past_ball_vel=boost::circular_buffer<DPoint>(2);
        past_robot_vel=boost::circular_buffer<nubot_common::VelCmd>(2);

        past_ball_known = true;
        past_ball_pos_ = ball_pos_;
        known_time = 0;

#ifdef THREEPLAYER
        roletime = 0;
#endif


        /// Challenge
        m_location_ = new Location(world_model_info_,m_plan_,m_dribble_);
        m_avoidance_ = new Avoidance(world_model_info_,m_plan_,m_dribble_);
        m_passing_ = new Passing(world_model_info_,m_plan_,m_dribble_);
    }

    ~NuBotControl()
    {
        m_plan_.m_behaviour_.clearBehaviorState();
        setEthercatCommond();
    }

#ifndef SIMULATION
    void configure(const nubot_control::nubotcontrolConfig & config, uint32_t level)
    {/*
       kp_ = config.kp;
       kalpha_ = config.kalpha;
       kbeta_  = config.kbeta;

       m_strategy_.m_plan_.kp =  kp_;
       m_strategy_.m_plan_.kalpha =  kalpha_;
       m_strategy_.m_plan_.kbeta  =   kbeta_;*/
    }

    void update_my_dribble_info(const nubot_common::OdoInfo & _my_msg)
    {
        isdribble[world_model_info_.AgentID_-1] = _my_msg.BallIsHolding;
        world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].setDribbleState(_my_msg.BallIsHolding);
        if(isBallHandle(world_model_info_.AgentID_))
        {
            world_model_info_.BallInfo_[world_model_info_.AgentID_-1].setLocationKnown(true);
            ball_pos_ = robot_pos_;
        }
        m_dribble_.is_dribble_ = isBallHandle(world_model_info_.AgentID_);
    }
#endif
    void update_world_model_info(const nubot_common::WorldModelInfo & _world_msg)
    {
        /** 更新PathPlan自身与队友的信息，自身的策略信息记住最好不要更新，因为本身策略是从此传过去的*/
        for(std::size_t i = 0 ; i < OUR_TEAM ; i++)
        {
            world_model_info_.RobotInfo_[i].setID(_world_msg.robotinfo[i].AgentID);

            world_model_info_.RobotInfo_[i].setTargetNum(1,_world_msg.robotinfo[i].targetNum1);
            world_model_info_.RobotInfo_[i].setTargetNum(2,_world_msg.robotinfo[i].targetNum2);
            world_model_info_.RobotInfo_[i].setTargetNum(3,_world_msg.robotinfo[i].targetNum3);
            world_model_info_.RobotInfo_[i].setTargetNum(4,_world_msg.robotinfo[i].targetNum4);
            //world_model_info_.RobotInfo_[i].setpassNum(_world_msg.robotinfo[i].staticpassNum);
            //world_model_info_.RobotInfo_[i].setcatchNum(_world_msg.robotinfo[i].staticcatchNum);
            world_model_info_.RobotInfo_[i].setLocation(DPoint(_world_msg.robotinfo[i].pos.x,
                                                               _world_msg.robotinfo[i].pos.y));
            world_model_info_.RobotInfo_[i].setHead(Angle(_world_msg.robotinfo[i].heading.theta));
            world_model_info_.RobotInfo_[i].setVelocity(DPoint( _world_msg.robotinfo[i].vtrans.x,
                                                                _world_msg.robotinfo[i].vtrans.y));
            world_model_info_.RobotInfo_[i].setStuck(_world_msg.robotinfo[i].isstuck);
            world_model_info_.RobotInfo_[i].setKick(_world_msg.robotinfo[i].iskick);
            world_model_info_.RobotInfo_[i].setValid(_world_msg.robotinfo[i].isvalid);
            world_model_info_.RobotInfo_[i].setW(_world_msg.robotinfo[i].vrot);
            /** 信息是来源于队友，则要更新机器人策略信息*/
            //if(world_model_info_.AgentID_ != i+1)
            {
#ifdef SIMULATION
                world_model_info_.RobotInfo_[i].setDribbleState(_world_msg.robotinfo[i].isdribble);
#endif
                world_model_info_.RobotInfo_[i].setRolePreserveTime(_world_msg.robotinfo[i].role_time);
                world_model_info_.RobotInfo_[i].setCurrentRole(_world_msg.robotinfo[i].current_role);
                world_model_info_.RobotInfo_[i].setTarget(DPoint(_world_msg.robotinfo[i].target.x,_world_msg.robotinfo[i].target.y));
            }
        }
        /** 更新障碍物信息*/
        world_model_info_.Obstacles_.clear();
        for(nubot_common::Point2d point : _world_msg.obstacleinfo.pos )
            world_model_info_.Obstacles_.push_back(DPoint(point.x,point.y));
        world_model_info_.Opponents_.clear();
        for(nubot_common::Point2d point : _world_msg.oppinfo.pos )
            world_model_info_.Opponents_.push_back(DPoint(point.x,point.y));
        /** 更新足球物信息*/
        for(std::size_t i = 0 ; i < OUR_TEAM ; i++)
        {
            world_model_info_.BallInfo_[i].setGlobalLocation(DPoint(_world_msg.ballinfo[i].pos.x ,_world_msg.ballinfo[i].pos.y));
            world_model_info_.BallInfo_[i].setRealLocation(PPoint(Angle(_world_msg.ballinfo[i].real_pos.angle),
                                                                  _world_msg.ballinfo[i].real_pos.radius));
            world_model_info_.BallInfo_[i].setVelocity(DPoint(_world_msg.ballinfo[i].velocity.x,_world_msg.ballinfo[i].velocity.y));
            world_model_info_.BallInfo_[i].setVelocityKnown(_world_msg.ballinfo[i].velocity_known);
            world_model_info_.BallInfo_[i].setLocationKnown(_world_msg.ballinfo[i].pos_known);
            world_model_info_.BallInfo_[i].setValid(_world_msg.ballinfo[i].pos_known);
        }
        world_model_info_.BallInfoState_ = _world_msg.ballinfo[world_model_info_.AgentID_-1].ballinfostate;
        /*
#ifdef THREEPLAYER
        world_model_info_.IsOurDribble_ = false;
        for(int i=0; i<OUR_TEAM; ++i)
        {
            if(isBallHandle(i+1) == true)
            {
                roletime = 0;
                world_model_info_.IsOurDribble_ = true;
                break;
            }
        }
#endif
*/
        /** 更新的COACH信息*/
        world_model_info_.CoachInfo_.MatchMode =_world_msg.coachinfo.MatchMode;
        world_model_info_.CoachInfo_.MatchType =_world_msg.coachinfo.MatchType;

        past_ball_vel.push_back(ball_vel_);
        m_plan_.update();
    }

    /** 球的三维信息,用于守门员角色*/
    void ballInfo3dCallback(const nubot_common::BallInfo3d  &_BallInfo_3d){

        //m_strategy_->goalie_strategy_.setBallInfo3dRel( _BallInfo_3d );
    }
    /** 主要的控制框架位于这里*/
    void loopControl(const ros::TimerEvent& event)
    {
        match_mode_ = world_model_info_.CoachInfo_.MatchMode;               //! 当前比赛模式
        pre_match_mode_ = world_model_info_.CoachInfo_.MatchType;           //! 上一个比赛模式
        robot_pos_  = world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].getLocation();
        robot_ori_  = world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].getHead();
        ball_pos_   = world_model_info_.BallInfo_[world_model_info_.AgentID_-1].getGlobalLocation();
        ball_vel_   = world_model_info_.BallInfo_[world_model_info_.AgentID_-1].getVelocity();
        if(world_model_info_.BallInfo_[world_model_info_.AgentID_-1].isLocationKnown() == false)
        {
            if(known_time < maxknowtime)
            {
                ball_pos_ = past_ball_pos_;
                world_model_info_.BallInfo_[world_model_info_.AgentID_-1].setLocationKnown(true);
                known_time++;
            }
        }
        else
        {
            past_ball_pos_ = ball_pos_;
            known_time = 0;
        }

        ///....
        match_mode_ = MODE;
        if(match_mode_ == STOPROBOT)
        {
            ROS_INFO("nubotcontrol loopControl STOPROBT");
            m_plan_.m_behaviour_.clearBehaviorState();
            if(m_dribble_.is_dribble_)
            ROS_INFO("ballhandle = true");
            else
                ROS_INFO("ballhandle = false");
            //setShoot();
        }
        /** 机器人在开始之前的跑位. 开始静态传接球的目标点计算*/
        else if(match_mode_ == PARKINGROBOT)
        {
            DPoint br = DPoint(0.0, 0.0) - robot_pos_;
            m_plan_.move2Positionwithobs_noball(parking_point[world_model_info_.AgentID_-1]);
            m_plan_.positionAvoidObs2(br.angle().radian_);
        }
        else if(match_mode_ == OUR_KICKOFF)
        {
            if(isBallHandle(world_model_info_.AgentID_))
            {
                setShoot(5.0);
            }
        }
        else if(match_mode_ == DROPBALL)
        {
            DPoint target, br;
            if(flag == 0)
            {
                target = DPoint(0.0,0.0);
                br = target-robot_pos_;
                m_plan_.move2Positionwithobs_noball(target);
                m_plan_.positionAvoidObs2(br.angle().radian_);
                if(robot_pos_.distance(target)<10.0 && fabs(robot_ori_.radian_ - br.angle().radian_) < 5.0*DEG2RAD)
                {
                    flag=1;
                }
            }
            if(flag == 1)
            {
                target = DPoint(260.0,60.0);
                br = target-robot_pos_;
                m_plan_.move2Positionwithobs_noball(target);
                m_plan_.positionAvoidObs2(br.angle().radian_);
                if(robot_pos_.distance(target)<10.0 && fabs(robot_ori_.radian_ - br.angle().radian_) < 5.0*DEG2RAD)
                {
                    flag=2;
                }
            }
            if(flag == 2)
            {
                target = DPoint(260.0,-60.0);
                br = target-robot_pos_;
                m_plan_.move2Positionwithobs_noball(target);
                m_plan_.positionAvoidObs2(br.angle().radian_);
                if(robot_pos_.distance(target)<10.0 && fabs(robot_ori_.radian_ - br.angle().radian_) < 5.0*DEG2RAD)
                {
                    flag=0;
                }
            }
        }
        else if(match_mode_ == STARTROBOT)// 机器人正式比赛了，进入start之后的机器人状态
        {
            normalGame();
        } // start部分结束
        /// Challenge
        else if(match_mode_ == LOCATION) //找框
        {
//            int n = 22;
//            int usednum[n] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22};
            int n=5;
            int usednum[n] = {5,8,19,21,12};
            m_location_->update();
            m_location_->process(n, usednum);
            //m_plan_.m_behaviour_.app_w_ = SINGLEPI_CONSTANT*2.0/7.0;
            //m_plan_.m_behaviour_.app_vx_ = -10.0;
        }
        else if(match_mode_ == AVOIDANCE) //避障
        {
            m_avoidance_->update();
            m_avoidance_->process();
        }
        else if(match_mode_ == PASSING) //踢坛子
        {
            m_passing_->level = 1;
            m_passing_->update();
            m_passing_->process();
            if(m_passing_->kick_enable_ == true)
            {
                setShoot();
                m_passing_->kick_enable_ = false;
            }
        }

        setEthercatCommond();
/*
#ifdef THREEPLAYER
        pubStrategyInfo();  // 发送策略消息让其他机器人看到，这一部分一般用于多机器人之间的协同
#endif
*/
    }


    void normalGame()
    {
        DPoint br, target;
//        ROS_INFO("robot%d normalGame", world_model_info_.AgentID_);
        m_plan_.m_subtargets_.ball_pos_ = ball_pos_;
        m_plan_.m_subtargets_.robot_pos_ = robot_pos_;
        m_plan_.robot_pos_=robot_pos_;
        m_plan_.robot_ori_=robot_ori_;
        m_plan_.ball_pos_=ball_pos_;
        m_plan_.m_behaviour_.past_ball_vel=past_ball_vel;
        m_plan_.m_behaviour_.past_robot_vel=past_robot_vel;

        if(robot_pos_.x_==0.0 && robot_pos_.y_==0.0 && ball_pos_.x_==0.0 && ball_pos_.y_==0.0)
        {
            m_strategy_->selected_action_ = No_Action;
#ifdef THREEPLAYER
            roletime = 0;
#endif
        }
        if(world_model_info_.Opponents_.size() == 0)
        {
            world_model_info_.Opponents_.push_back(DPoint(0.0,0.0));
        }
//        ROS_INFO("ball: (%f, %f)", ball_pos_.x_, ball_pos_.y_);
//        ROS_INFO("robot: (%f, %f)", robot_pos_.x_, robot_pos_.y_);
//        ROS_INFO("obs: (%f, %f)", opp_robot_.x_, opp_robot_.y_);
/*
#ifdef THREEPLAYER
        if(roletime == 0)
        {
            roletime = ROLEMAXTIME;
            opp_robot_ = world_model_info_.Opponents_[m_plan_.oppneartargetid(ball_pos_)-1];//对方距球最近的机器人位置
            world_model_info_.pass_ID_ = m_plan_.ourneartargetid(ball_pos_);
            world_model_info_.pass_pt_ = world_model_info_.RobotInfo_[world_model_info_.pass_ID_-1].getLocation();
            world_model_info_.catch_ID_ = m_plan_.ourneartargetid(world_model_info_.RobotInfo_[world_model_info_.pass_ID_-1].getLocation(), world_model_info_.pass_ID_);
            world_model_info_.catch_pt_ = world_model_info_.RobotInfo_[world_model_info_.catch_ID_-1].getLocation();
            cout<<"passid="<<world_model_info_.pass_ID_<<"catchid="<<world_model_info_.catch_ID_<<endl;
        }
        roletime--;
#else
*/
        opp_robot_ = world_model_info_.Opponents_[0];
//#endif

        if(isBallHandle(world_model_info_.AgentID_))
        {
            world_model_info_.BallInfo_[world_model_info_.AgentID_-1].setLocationKnown(true);
            ball_pos_ = robot_pos_;
        }

#ifdef THREEPLAYER
#ifdef SIMULATION
        if(world_model_info_.AgentID_ == 2) m_strategy_->selected_role_ = ACTIVE;
        else if(world_model_info_.AgentID_ == 1) m_strategy_->selected_role_ = GOALIE;//defender
#else
        if(world_model_info_.AgentID_ == 2) m_strategy_->selected_role_ = ACTIVE;
        else if(world_model_info_.AgentID_ == 3) m_strategy_->selected_role_ = GOALIE;
        //m_strategy_->selected_role_ = 3;//defender
#endif
        //if(world_model_info_.pass_ID_ == world_model_info_.AgentID_) //Active
        if(m_strategy_->selected_role_ == ACTIVE)
#endif
        {
            if(world_model_info_.BallInfo_[world_model_info_.AgentID_-1].isLocationKnown() == false)
            {
                m_strategy_->selected_action_ = CanNotSeeBall;//Cannotseeball  No_Action
            }
            else if(ball_pos_.distance(opp_robot_) < LIMITDRIBLLEDIS && ball_pos_.distance(opp_robot_) < ball_pos_.distance(robot_pos_)) //对方带球
            {
#ifdef THREEPLAYER
                m_strategy_->selected_action_ = CanNotSeeBall;
#else
                if(ball_pos_.x_ < 0.0)
                {
                    m_strategy_->selected_action_ = Positioned_Static;
                }
                else
                {
                    m_strategy_->selected_action_ = CanNotSeeBall;
                }
#endif
            }
            else
            {
                m_strategy_->selected_action_ = TurnForShoot;
            }

            if(m_strategy_->selected_action_ == CanNotSeeBall)
            {
                ROS_INFO("CanNotSeeBall");
#ifdef THREEPLAYER
                opp_robot_ = world_model_info_.Opponents_[m_plan_.oppneartargetid(DPoint(-FIELD_LENGTH*1.0/2, 0.0))-1];//对方距我方球门最近的机器人
#endif
                target = DPoint((-FIELD_LENGTH*1.0/2+opp_robot_.x_)/2,opp_robot_.y_/2);
                br = opp_robot_ - robot_pos_;
                m_plan_.move2Positionwithobs_noball(target);
                m_plan_.positionAvoidObs2(br.angle().radian_);
            }
            if(m_strategy_->selected_action_ == TurnForShoot) // Active 踢球准备
            {
                ROS_INFO("TurnForShoot");
                if(isBallHandle(world_model_info_.AgentID_) == false)
                {
                    m_strategy_->selected_action_ = Catch_Positioned;
                }
                else
                {
//#ifdef THREEPLAYER
//                    if(m_plan_.pnpoly(world_model_info_.Opponents_, robot_pos_) == false)
//#endif
                    {
                        target = DPoint(FIELD_LENGTH*1.0/2, 0.0);
                        br = target - robot_pos_;
//#ifdef THREEPLAYER
//                        opp_robot_ = world_model_info_.Opponents_[m_plan_.oppneartargetid(DPoint(FIELD_LENGTH*1.0/2, 0.0))]; //对方守门员
//#endif
                        m_strategy_->selected_action_ = Positioned;
                    }
//#ifdef THREEPLAYER
//                    else //传球
//                    {
//                        br = world_model_info_.RobotInfo_[world_model_info_.catch_ID_-1].getLocation() - robot_pos_;
//                        DPoint br2;
//                        bool flag222 = false;
//                        for(int i=0; i<world_model_info_.Opponents_.size(); ++i)
//                        {
//                            br2 = world_model_info_.Opponents_[i]-robot_pos_;
//                            if(fabs(br.angle().radian_-br2.angle().radian_) < 30*DEG2RAD)
//                            {
//                                if(robot_pos_.distance(world_model_info_.Opponents_[i]) < robot_pos_.distance(world_model_info_.RobotInfo_[world_model_info_.catch_ID_-1].getLocation()))
//                                {
//                                    flag222 = true;
//                                }
//                            }
//                        }
//                        if(flag222)
//                        {
//                            m_plan_.move2Positionwithobs_noball(world_model_info_.RobotInfo_[world_model_info_.catch_ID_-1].getLocation());
//                        }
//                        else
//                        {
//                            m_plan_.positionAvoidObs2(br.angle().radian_);
//                            if(fabs(robot_ori_.radian_-br.angle().radian_) < 5*DEG2RAD)
//                            {
//                                m_strategy_->selected_action_ = AtShootSituation;
//                            }
//                        }
//                    }
//#endif
                }
            }
#ifndef THREEPLAYER
            if(m_strategy_->selected_action_ == Positioned_Static) //移动到防守位置
            {
                ROS_INFO("robot%d Positioned_Static", world_model_info_.AgentID_);

                double time_robot2ball = robot_pos_.distance(ball_pos_)/MAXVEL;
                target = DPoint(ball_pos_.x_+ball_vel_.x_*time_robot2ball, ball_pos_.y_+ball_vel_.y_*time_robot2ball);
                m_plan_.move2Positionwithobs_noball(DPoint(ourGoalPosl.x_-RADIUS, target.y_));
                m_plan_.positionAvoidObs(ball_pos_);

                if(opp_robot_.distance(ball_pos_) > robot_pos_.distance(ball_pos_))
                {
                    m_strategy_->selected_action_ = Catch_Positioned;
                }
            }
#endif
            if(m_strategy_->selected_action_ == Catch_Positioned) //走到球的位置并完成抓球
            {
                ROS_INFO("robot%d Catch_Positioned", world_model_info_.AgentID_);
//                double time_robot2ball = robot_pos_.distance(ball_pos_)/MAXVEL;
//                target = DPoint(ball_pos_.x_+ball_vel_.x_*time_robot2ball, ball_pos_.y_+ball_vel_.y_*time_robot2ball);
                target = ball_pos_;
                br = ball_pos_ - robot_pos_;
                m_plan_.positionAvoidObs2(br.angle().radian_, MAXW, 3*DEG2RAD);
                if(robot_pos_.distance(ball_pos_) < LIMITDRIBLLEDIS + 20.0)
                {
                    if(fabs(br.angle().radian_-robot_ori_.radian_)<3*DEG2RAD)
                    {
                        m_plan_.move2Positionwithobs_noball(ball_pos_);
                        if(isBallHandle(world_model_info_.AgentID_) != true)
                        {
                            m_plan_.move2Positionwithobs_noball(ball_pos_);
                        }
                        else
                        {
                            m_strategy_->selected_action_ = Positioned;
                        }
                    }
                }
                else
                {
                    m_plan_.move2Positionwithobs_noball(target);
                }
            }
            if(m_strategy_->selected_action_ == Positioned) //移动到目标位置 move to position 射门位置
            {
                ROS_INFO("robot%d Positioned", world_model_info_.AgentID_);
                //m_plan_.move2Positionwithobs_noball(target);
//#ifdef THREEPLAYER
//                opp_robot_ = world_model_info_.Opponents_[m_plan_.oppneartargetid(ball_pos_)-1];//对方距球最近的机器人位置
//#endif
                //br = opp_robot_ - robot_pos_;
                //if(robot_pos_.distance(opp_robot_)<100.0 && fabs(br.angle().radian_-robot_ori_.radian_)<90*DEG2RAD)
                //{
                    //br = DPoint(0.0,0.0)-br;
                    //m_plan_.positionAvoidObs2(robot_ori_.radian_*2-br.angle().radian_);
//                    if(fabs(fabs(br.angle().radian_)-fabs(robot_ori_.radian_))>60*DEG2RAD)
//                    {
//                        m_plan_.move2Positionwithobs_noball(DPoint(robot_pos_.x_>0?robot_pos_.x_-20.0:robot_pos_.x_+20.0, robot_pos_.y_>0?robot_pos_.y_-20.0:robot_pos_.y_+20.0));
//                    }
                //}
                target = DPoint(FIELD_LENGTH*1.0/2, 0.0);
                br = target - robot_pos_;
//                m_plan_.move2Positionwithobs_noball(DPoint(target.x_/2.0,0.0));
//                    if(br.angle().radian_ > 60*DEG2RAD)
//                    {
//                        m_plan_.move2Positionwithobs_noball(DPoint(robot_pos_.x_,robot_pos_.y_+20.0));
//                    }
//                    else if(br.angle().radian_ < -60*DEG2RAD)
//                    {
//                        m_plan_.move2Positionwithobs_noball(DPoint(robot_pos_.x_,robot_pos_.y_-20.0));
//                    }
//                    else
                m_plan_.positionAvoidObs2(br.angle().radian_);
                m_plan_.m_behaviour_.app_vx_ = 10.0;
                if(robot_ori_.radian_ < (oppGoalPosl-robot_pos_).angle().radian_ && robot_ori_.radian_ > (oppGoalPosr-robot_pos_).angle().radian_)
                {
                    ROS_INFO("radian: oppr-pos=%f, robot_ori_=%f, oppl-pos=%f", (oppGoalPosr-robot_pos_).angle().radian_, robot_ori_.radian_, (oppGoalPosl-robot_pos_).angle().radian_);
                    m_plan_.move2Positionwithobs_noball(target);
                }
                if(robot_pos_.distance(target) < (dist_KICKGoal1+dist_KICKGoal2)/2)
                {
                    m_strategy_->selected_action_ = Actions::StaticPass;
                }
            }
            if(m_strategy_->selected_action_ == Actions::StaticPass) //到射门位置 如果对方机器人堵住门就转向
            {
                ROS_INFO("robot%d StaticPass", world_model_info_.AgentID_);

//#ifdef THREEPLAYER
//                opp_robot_ = world_model_info_.Opponents_[m_plan_.oppneartargetid(DPoint(FIELD_LENGTH*1.0/2, 0.0))]; //对方守门员
//#endif

                if(opp_robot_.x_<robot_pos_.x_ || opp_robot_.y_>oppGoalPosl.y_ || opp_robot_.y_<oppGoalPosr.y_)
                {
                    br = DPoint(FIELD_LENGTH*1.0/2, 0.0) - robot_pos_;
                    m_plan_.positionAvoidObs2(br.angle().radian_);
                    if(robot_pos_.x_ > FIELD_LENGTH*1.0/2-70.0 && robot_pos_.y_ < fabs(oppGoalPosl.y_))
                    {
                        m_plan_.move2Positionwithobs_noball(robot_pos_);
                    }
                    else
                    {
                        m_plan_.move2Positionwithobs_noball(DPoint(FIELD_LENGTH*1.0/2-100.0, 0.0));
                    }
                    if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD)
                    {
                        m_strategy_->selected_action_ = AtShootSituation;
                    }
                }
                else
                {
                    if(opp_robot_.y_>0.0)
                    {
                        br = DPoint(FIELD_LENGTH*1.0/2, (oppGoalPosr.y_-OBLE_RADIUS)/2);
                        m_plan_.positionAvoidObs2(br.angle().radian_);
                        if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD)
                        {
                            m_strategy_->selected_action_ = AtShootSituation;
                        }
                    }
                    else
                    {
                        br = DPoint(FIELD_LENGTH*1.0/2, (oppGoalPosl.y_+OBLE_RADIUS)/2);
                        m_plan_.positionAvoidObs2(br.angle().radian_);
                        if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD)
                        {
                            m_strategy_->selected_action_ = AtShootSituation;
                        }
                    }
                }
            }
        }
            /*
#ifdef THREEPLAYER
        }
        else if(world_model_info_.AgentID_ == world_model_info_.catch_ID_) //assist
        {
            if(world_model_info_.BallInfo_[world_model_info_.AgentID_-1].isLocationKnown() == false)
            {
                m_strategy_->selected_action_ = CanNotSeeBall;
            }
            else
            {
                double mindist=10000.0;
                for(int i=0; i<world_model_info_.Opponents_.size(); ++i)
                {
                    if(ball_pos_.distance(world_model_info_.Opponents_[i]) < mindist)
                    {
                        mindist = ball_pos_.distance(world_model_info_.Opponents_[i]);
                        opp_robot_ = world_model_info_.Opponents_[i];
                    }
                }
                target = ball_pos_ + opp_robot_;
                target.x_ /= 2.0;
                target.y_ /= 2.0;
                if(ball_pos_.distance(opp_robot_) < LIMITDRIBLLEDIS && ball_pos_.distance(opp_robot_) < ball_pos_.distance(world_model_info_.RobotInfo_[world_model_info_.pass_ID_-1].getLocation())) //对方带球////
                {
                    m_strategy_->selected_action_ = CanNotSeeBall;
                }
                else if(world_model_info_.IsOurDribble_) //我方带球
                {
                    m_strategy_->selected_action_ = Positioned;
                }
                else
                {
                    m_strategy_->selected_action_ = Positioned_Static;
                }
            }

            if(m_strategy_->selected_action_ == CanNotSeeBall) //走到对方带球机器人和我方球门中间
            {
                ROS_INFO("CanNotSeeBall");
                double mindis = 10000.0;
                for(int i=0; i<world_model_info_.Opponents_.size(); ++i)
                {
                    if(world_model_info_.Opponents_[i].distance(DPoint(-FIELD_LENGTH*1.0/2, 0.0)) < mindis)
                    {
                        opp_robot_ = world_model_info_.Opponents_[i];
                        mindis = world_model_info_.Opponents_[i].distance(ball_pos_);
                    }
                }
                target = DPoint((-FIELD_LENGTH*1.0/2+opp_robot_.x_)/2,opp_robot_.y_/2);
                br = opp_robot_ - robot_pos_;
                //cout<<"target :("<<target.x_<<","<<target.y_<<")"<<endl;
                m_plan_.move2Positionwithobs_noball(target);
                if(robot_pos_.distance(target) < 10.0)
                {
                    m_plan_.positionAvoidObs2(br.angle().radian_);
                }
            }
            if(m_strategy_->selected_action_ == Positioned) //我方将要射门
            {
                ROS_INFO("Positioned");

                int opp_id = -1;
                double min_dist = INF;
                double dist_opp2oppgoal;
                double dist_our2oppgoal = DPoint(FIELD_LENGTH*1.0/2, 0.0).distance(world_model_info_.RobotInfo_[world_model_info_.pass_ID_-1].getLocation());
                for(int i=0; i<world_model_info_.Opponents_.size(); ++i)
                {
                    dist_opp2oppgoal = world_model_info_.Opponents_[i].distance(DPoint(FIELD_LENGTH*1.0/2, 0.0));
                    if(dist_opp2oppgoal > dist_our2oppgoal && dist_opp2oppgoal < min_dist)
                    {
                        opp_id = i+1;
                        min_dist = dist_opp2oppgoal;
                    }
                }
                if(opp_id == -1)
                {
                    m_plan_.move2Positionwithobs_noball(DPoint(0.0, 0.0));
                    if(robot_pos_.distance(DPoint(0.0, 0.0)) < 100.0)
                    {
                        m_plan_.positionAvoidObs(ball_pos_);
                    }
                }
                else
                {
                    m_plan_.move2Positionwithobs_noball(world_model_info_.Opponents_[opp_id-1]);
                    if(robot_pos_.distance(world_model_info_.Opponents_[opp_id-1]) < 20.0)
                    {
                        m_plan_.positionAvoidObs(ball_pos_);
                    }
                }
            }
            if(m_strategy_->selected_action_ == Positioned_Static) //移动到对方机器人和球中间
            {
                ROS_INFO("Positioned_Static");

                target = ball_pos_ + world_model_info_.Opponents_[m_plan_.oppneartargetid(ball_pos_)];
                target.x_ /= 2.0;
                target.y_ /= 2.0;
                if(robot_pos_.distance(target) < 10.0)
                {
                    m_plan_.positionAvoidObs(ball_pos_);
                    if(fabs(br.angle().radian_-robot_ori_.radian_) < 5*DEG2RAD)
                    {
                        m_strategy_->selected_action_ = Catch_Positioned;
                    }
                }
                else
                {
                    m_plan_.move2Positionwithobs_noball(target);
                }
            }
            if(m_strategy_->selected_action_ == Catch_Positioned) //传球用
            {
                ROS_INFO("robot%d Catch_Positioned", world_model_info_.AgentID_);
                br = ball_pos_ - robot_pos_;
                if(fabs(br.angle().radian_ - robot_ori_.radian_) < 15.0*DEG2RAD)
                {
                    m_plan_.move2Positionwithobs_noball(ball_pos_, ConstDribbleDisFirst);
                }
                else
                {
                    m_plan_.positionAvoidObs2(br.angle().radian_);
                }
                if(fabs(br.angle().radian_ - robot_ori_.radian_) < 15.0*DEG2RAD && ball_pos_.distance(robot_pos_) < ConstDribbleDisFirst)
                {
                    m_plan_.positionAvoidObs2(br.angle().radian_);
                    if(fabs(br.angle().radian_ - robot_ori_.radian_) < 5.0*DEG2RAD)
                    {
                        if(isBallHandle(world_model_info_.AgentID_) == false)
                        {
                            m_plan_.move2Positionwithobs_noball(ball_pos_, ConstDribbleDisSecond);
                        }
                    }
                    else
                    {
                        m_plan_.positionAvoidObs2(br.angle().radian_);
                    }
                }
            }

        }
        else if(world_model_info_.AgentID_ == defender_ID_)//defender
        {
            if(world_model_info_.BallInfo_[world_model_info_.AgentID_-1].isLocationKnown() == false)
            {
                m_strategy_->selected_action_ = CanNotSeeBall;
            }
            else
            {
                m_strategy_->selected_action_ = Positioned;
            }

            if(m_strategy_->selected_action_ == CanNotSeeBall)
            {
                target = DPoint(-FIELD_LENGTH*1.0/2+RADIUS, 0.0);
                if(robot_pos_.distance(target) < 10.0)
                {
                    m_plan_.positionAvoidObs(DPoint(0.0, 0.0));
                }
                else
                {
                    m_plan_.move2Positionwithobs_noball(target);
                }
            }
            if(m_strategy_->selected_action_ == Positioned) //移动到目标位置 move to position 射门位置
            {
                ROS_INFO("Positioned");

                target = DPoint(-FIELD_LENGTH*1.0/2+RADIUS, ball_pos_.y_>0.0?min(ball_pos_.y_,ourGoalPosl.y_):max(ball_pos_.y_,ourGoalPosr.y_));/////////if i can use gauss....
                if(robot_pos_.distance(target) < 10.0)
                {
                    m_plan_.positionAvoidObs(ball_pos_);
                }
                else
                {
                    m_plan_.move2Positionwithobs_noball(target);
                }
            }
        }
#endif
*/
#ifdef THREEPLAYER
        else if(m_strategy_->selected_role_ == GOALIE)
        {
            if(world_model_info_.BallInfo_[world_model_info_.AgentID_-1].isLocationKnown() == false) //找不到球
            {
                bool flag_oppinourfield = false;
                for(int i=0; i<world_model_info_.Opponents_.size(); ++i)
                {
                    if(world_model_info_.Opponents_[i].x_<0.0) //有一个敌方机器人在我方半场
                    {
                        m_strategy_->selected_action_ = Catch_Positioned; //预测球可能到达的位置，提前等到位置进行防守
                        flag_oppinourfield = true;
                        break;
                    }
                }
                if(flag_oppinourfield == false) //敌方机器人均在对方半场
                {
                    m_strategy_->selected_action_ = CanNotSeeBall;
                }
            }
            else if(isBallHandle(world_model_info_.AgentID_))
            {
                m_strategy_->selected_action_ = Positioned_Static;
            }
            else
            {
                m_strategy_->selected_action_ = Catch_Positioned;
            }

            if(m_strategy_->selected_action_ == CanNotSeeBall) //看不到球，移动到中间
            {
                ROS_INFO("robot%d CanNotSeeBall", world_model_info_.AgentID_);
                target = DPoint(-FIELD_LENGTH*1.0/2+RADIUS, 0.0);
                m_plan_.positionAvoidObs(DPoint(0.0, 0.0));
                m_plan_.move2Positionwithobs_noball(target);
            }
            if(m_strategy_->selected_action_ == Catch_Positioned) //防守，对方将要射门
            {
                ROS_INFO("robot%d Opp will kick", world_model_info_.AgentID_);

                double time_robot2ball = robot_pos_.distance(ball_pos_)/MAXVEL;
                target = DPoint(-FIELD_LENGTH*1.0/2+RADIUS, ball_pos_.y_+ball_vel_.y_*time_robot2ball);
                if(fabs(target.y_) > fabs(ourGoalPosl.y_))
                {
                    if(target.y_ > 0)
                    {
                        target.y_ = fabs(ourGoalPosl.y_);
                    }
                    else
                    {
                        target.y_ = -fabs(ourGoalPosl.y_);
                    }
                }
                m_plan_.move2Positionwithobs_noball(target);
                m_plan_.positionAvoidObs(ball_pos_);
            }
            if(m_strategy_->selected_action_ == Positioned_Static) //自己持球，传给主攻
            {
                ROS_INFO("robot%d Kick to Active", world_model_info_.AgentID_);
                br = DPoint(0.0,0.0)-robot_pos_;
                m_plan_.positionAvoidObs2(br.angle().radian_);
                if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD)
                {
                    m_strategy_->selected_action_ = AtShootSituation;
                }
            }
        }
#endif
        if(m_strategy_->selected_action_ == AtShootSituation) //踢球
        {
            ROS_INFO("AtShootSituation");

            setShoot(robot_pos_.distance(DPoint(FIELD_LENGTH*1.0/2, 0.0)));
        }
        if(m_strategy_->selected_action_ == No_Action)
        {
            ROS_INFO("No_Action");
            m_plan_.move2Positionwithobs_noball(robot_pos_);
        }
    }

    void setEthercatCommond()
    {
        nubot_common::VelCmd command;  // 最后是解算出来的结果
        float vx,vy,w;
//        vx = m_plan_.m_behaviour_.app_vx_;
//        vy = m_plan_.m_behaviour_.app_vy_;
//        w  = m_plan_.m_behaviour_.app_w_;
//        m_plan_.m_behaviour_.last_app_vx_ = m_plan_.m_behaviour_.app_vx_;
//        m_plan_.m_behaviour_.last_app_vy_ = m_plan_.m_behaviour_.app_vy_;
//        m_plan_.m_behaviour_.last_app_w_  = m_plan_.m_behaviour_.app_w_;
        vx = (m_plan_.m_behaviour_.app_vx_+m_plan_.m_behaviour_.last_app_vx_)/2;
        vy = (m_plan_.m_behaviour_.app_vy_+m_plan_.m_behaviour_.last_app_vy_)/2;
        w  = (m_plan_.m_behaviour_.app_w_+m_plan_.m_behaviour_.last_app_w_)/2;

//        if(m_plan_.m_behaviour_.app_w_ > 0 && m_plan_.m_behaviour_.app_w_ < MINW)
//        {
//          m_plan_.m_behaviour_.app_w_ = MINW;
//        }
//        if(m_plan_.m_behaviour_.app_w_ < 0 && m_plan_.m_behaviour_.app_w_ > -MINW)
//        {
//          m_plan_.m_behaviour_.app_w_ = -MINW;
//        }

        double sqare2 = vx*vx + vy*vy;
        if(sqare2 > MAXVEL*MAXVEL)
        {
            vx = vx*MAXVEL/sqrt(sqare2);
            vy = vy*MAXVEL/sqrt(sqare2);
        }
        m_plan_.m_behaviour_.last_app_vx_ = vx;
        m_plan_.m_behaviour_.last_app_vy_ = vy;
        m_plan_.m_behaviour_.last_app_w_  = w;
        m_plan_.m_behaviour_.clearBehaviorState();
#ifdef SIMULATION
        command.Vx = vx;
        command.Vy = vy;
        command.w  = w;
#else
        command.Vx = -vy * 20;
        command.Vy = vx * 20;
        command.w  = w * 1400;
#endif
        if(command.Vx==0 && command.Vy==0 && command.w==0)
        {
            command.stop_ = true;
        }
        ROS_INFO("V: %d, %d, %d", command.Vx, command.Vy, command.w);
        ROS_INFO("pos: %f, %f", robot_pos_.x_, robot_pos_.y_);
        motor_cmd_pub_.publish(command);

        //(60*vx*16)/(12.7*PI);
        //(60*w*16)/(19);
    }

    void setShoot(double strength=0.0, int ShootPos=RUN)
    {
        nubot_common::Shoot shoot;
        nubot_common::BallHandle dribble;
#ifdef SIMULATION
        shoot.request.strength = strength;
#else
        shoot.request.strength = 6.0;
#endif
        shoot.request.ShootPos = ShootPos;
        shoot_client_.call(shoot);
        dribble.request.enable = 0;
        ballhandle_client_.call(dribble);
        if(shoot.response.ShootIsDone == true)
        {
            isdribble[world_model_info_.AgentID_-1] = false;
        }
    }

    bool isBallHandle(int id)
    {
#ifdef SIMULATION
        nubot_common::BallHandle dribble;
        dribble.request.enable = 1;
        ballhandle_client_.call(dribble);
        return dribble.response.BallIsHolding;
#else
        return isdribble[id-1];
#endif
    }

//#ifdef THREEPLAYER
//    void pubStrategyInfo()
//    {
//        nubot_common::StrategyInfo strategy_info;       // 这个消息的定义可以根据个人需要进行修改
//        strategy_info.header.stamp = ros::Time::now();
//        strategy_info.AgentID     = world_model_info_.AgentID_;

//        strategy_info_pub_.publish(strategy_info);
//    }
//#endif

    //! 中场于助攻在机器人动态传球时会出现穿过传球线的现象，在此矫正传球时候，中场与助攻的跑位点，防止传球失败
/*    void coorrect_target(const DPoint & start_pt, const DPoint & end_pt, const DPoint & robot_pos, DPoint & target)
    {
        std::vector < DPoint> pts;
        pts.reserve(10);
        double max_y(start_pt.y_),min_y(end_pt.y_);
        if(start_pt.y_ < end_pt.y_)
        {
            max_y = end_pt.y_;
            min_y = start_pt.y_;
        }
        DPoint upper_pt =  start_pt;
        DPoint down_pt  =  end_pt;
        if( start_pt.x_ < end_pt.x_)
        {
            upper_pt = end_pt;
            down_pt  = start_pt;
        }
        pts.push_back(down_pt);
        pts.push_back(upper_pt);
        pts.push_back(DPoint(900.0,upper_pt.y_));
        pts.push_back(DPoint(900,600));
        pts.push_back(DPoint(-900,600));
        pts.push_back(DPoint(-900.0,down_pt.y_));
        if(pnpoly(pts,robot_pos)) //表示机器人要向y600方向运动；
        {
            target.x_ =robot_pos.x_;
            if(robot_pos.x_ >upper_pt.x_ && robot_pos.x_ < upper_pt.x_ +100)
                target.x_ = upper_pt.x_+100;
            else if(robot_pos.x_ < down_pt.x_ && robot_pos.x_ > down_pt.x_ -100)
                target.x_ = down_pt.x_-100;
            else
            {
                Angle delta_ang = DPoint(upper_pt.x_ -down_pt.x_,upper_pt.y_ -down_pt.y_).angle();
                if(delta_ang.radian_ > 0)
                    target.x_ = down_pt.x_ -100;
                else
                    target.x_ = upper_pt.x_+100;
            }
            if(fabs(target.x_) > 650 *WIDTH_RATIO)
                target.x_ = 650.0*target.x_/fabs(target.x_);

            target.y_ = max_y+200;
            if(fabs(target.y_) > 500 *WIDTH_RATIO)
                target.y_ = 500.0*WIDTH_RATIO;
            if(fabs(target.y_) < 400 *WIDTH_RATIO)
                target.y_ = 400.0*WIDTH_RATIO;
        }
        else
        {
            target.x_ =robot_pos.x_;
            if(robot_pos.x_ >upper_pt.x_ && robot_pos.x_ < upper_pt.x_ +100)
                target.x_ = upper_pt.x_+100;
            else if(robot_pos.x_ < down_pt.x_ && robot_pos.x_ > down_pt.x_ -100)
                target.x_ = down_pt.x_-100;
            else
            {
                Angle delta_ang = DPoint(upper_pt.x_ -down_pt.x_,upper_pt.y_ -down_pt.y_).angle();
                if(delta_ang.radian_ > 0)
                    target.x_ = upper_pt.x_ +100;
                else
                    target.x_ = down_pt.x_-100;
            }
            if(fabs(target.x_) > 650 *WIDTH_RATIO)
                target.x_ = 650.0*target.x_/fabs(target.x_);

            target.y_ = min_y-200;
            if(fabs(target.y_) > 500 *WIDTH_RATIO)
                target.y_ = -500.0*WIDTH_RATIO;
            if(fabs(target.y_) < 400 *WIDTH_RATIO)
                target.y_ = -400.0*WIDTH_RATIO;
        }
    }
*/

};

}
int main(int argc, char **argv)
{
    ros::init(argc,argv,"nubot_control_node");
    // 完成一系列的初始化工作？ 以及相应的报错机制。  只有当所有的传感器信息都已经准备就绪的时候才可以运行
    ros::Time::init();
    ROS_INFO("start control process");
    nubot::NuBotControl nubotcontrol(argc,argv);
    ros::spin();
    return 0;
}
