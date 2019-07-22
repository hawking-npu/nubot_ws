#include "nubot/core/core.hpp"

#include <nubot_common/VelCmd.h>
#include <nubot_common/WorldModelInfo.h>
#include <nubot_common/BallInfo3d.h>
#include <nubot_common/BallHandle.h>
#include <nubot_common/Shoot.h>
#include <nubot_common/StrategyInfo.h>
#include <nubot_common/TargetInfo.h>

#include <nubot_control/nubotcontrolConfig.h>
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

#define RUN 1
#define FLY -1
//shoot在FLY模式下，strength不重要,只要非零就行
const double DEG2RAD = 1.0/180.0*SINGLEPI_CONSTANT;     // 角度到弧度的转换

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
    DPoint  opp_robot_;


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
        motor_cmd_pub_ = nh_->advertise<nubot_common::VelCmd>("nubotcontrol/velcmd",1);
        strategy_info_pub_ =  nh_->advertise<nubot_common::StrategyInfo>("nubotcontrol/strategy",10);
        std::string  service = "BallHandle";
        ballhandle_client_ =  nh_->serviceClient<nubot_common::BallHandle>(service);
        std::string  service1 = "Shoot";
        shoot_client_ = nh_->serviceClient<nubot_common::Shoot>(service1);
        worldmodelinfo_sub_ = nh_->subscribe("worldmodel/worldmodelinfo", 1, &NuBotControl::update_world_model_info,this);
        ballinfo3d_sub1_    = nh_->subscribe("stereovision/StereoBallInfo",1, &NuBotControl::ballInfo3dCallback, this);
        control_timer_      = nh_->createTimer(ros::Duration(update_T/1000),&NuBotControl::loopControl,this);

        dynamic_reconfigure::Server<nubot_control::nubotcontrolConfig> reconfigureServer_;
        reconfigureServer_.setCallback(boost::bind(&NuBotControl::configure, this, _1, _2));
        world_model_info_.AgentID_ = atoi(environment); /** 机器人标号*/
        world_model_info_.CoachInfo_.MatchMode = STOPROBOT;
        m_plan_.world_model_ =  & world_model_info_;
        m_plan_.m_subtargets_.world_model_ =  & world_model_info_;
        m_staticpass_.world_model_= & world_model_info_;
        m_strategy_ = new Strategy(world_model_info_,m_plan_);

        past_ball_vel=boost::circular_buffer<DPoint>(2);
        past_robot_vel=boost::circular_buffer<nubot_common::VelCmd>(2);
    }

    ~NuBotControl()
    {
        m_plan_.m_behaviour_.app_vx_ = 0;
        m_plan_.m_behaviour_.app_vy_ = 0;
        m_plan_.m_behaviour_.app_w_  = 0;
        setEthercatCommond();
    }

    void
    configure(const nubot_control::nubotcontrolConfig & config, uint32_t level)
    {/*
       kp_ = config.kp;
       kalpha_ = config.kalpha;
       kbeta_  = config.kbeta;

       m_strategy_.m_plan_.kp =  kp_;
       m_strategy_.m_plan_.kalpha =  kalpha_;
       m_strategy_.m_plan_.kbeta  =   kbeta_;*/
    }

    void
    update_world_model_info(const nubot_common::WorldModelInfo & _world_msg)
    {
        /** 更新PathPlan自身与队友的信息，自身的策略信息记住最好不要更新，因为本身策略是从此传过去的*/
        for(std::size_t i = 0 ; i < OUR_TEAM ; i++)
        {
            world_model_info_.RobotInfo_[i].setID(_world_msg.robotinfo[i].AgentID);

            world_model_info_.RobotInfo_[i].setTargetNum(1,_world_msg.robotinfo[i].targetNum1);
            world_model_info_.RobotInfo_[i].setTargetNum(2,_world_msg.robotinfo[i].targetNum2);
            world_model_info_.RobotInfo_[i].setTargetNum(3,_world_msg.robotinfo[i].targetNum3);
            world_model_info_.RobotInfo_[i].setTargetNum(4,_world_msg.robotinfo[i].targetNum4);
            world_model_info_.RobotInfo_[i].setpassNum(_world_msg.robotinfo[i].staticpassNum);
            world_model_info_.RobotInfo_[i].setcatchNum(_world_msg.robotinfo[i].staticcatchNum);

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
                world_model_info_.RobotInfo_[i].setDribbleState(_world_msg.robotinfo[i].isdribble);
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
#ifdef THREEPLAYER
        world_model_info_.IsOurDribble_ = false;
        for(int i=0; i<OUR_TEAM; ++i)
        {
            if(world_model_info_.RobotInfo_[i].getDribbleState() == true)
            {
                world_model_info_.IsOurDribble_ = true;
                break;
            }
        }
#endif
        /** 更新的COACH信息*/
        world_model_info_.CoachInfo_.MatchMode =_world_msg.coachinfo.MatchMode;
        world_model_info_.CoachInfo_.MatchType =_world_msg.coachinfo.MatchType;

        past_ball_vel.push_back(ball_vel_);
        m_plan_.update();
    }

    /** 球的三维信息,用于守门员角色*/
    void
    ballInfo3dCallback(const nubot_common::BallInfo3d  &_BallInfo_3d){

        //m_strategy_->goalie_strategy_.setBallInfo3dRel( _BallInfo_3d );
    }
    /** 主要的控制框架位于这里*/
    void
    loopControl(const ros::TimerEvent& event)
    {
        match_mode_ = world_model_info_.CoachInfo_.MatchMode;               //! 当前比赛模式
        pre_match_mode_ = world_model_info_.CoachInfo_.MatchType;           //! 上一个比赛模式
        robot_pos_  = world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].getLocation();
        robot_ori_  = world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].getHead();
        ball_pos_   = world_model_info_.BallInfo_[world_model_info_.AgentID_-1].getGlobalLocation();
        ball_vel_   = world_model_info_.BallInfo_[world_model_info_.AgentID_-1].getVelocity();

        ///....
        match_mode_ = STARTROBOT;
        if(match_mode_ == STOPROBOT )
        {
            ROS_INFO("nubotcontrol loopControl STOPROBT");
            m_plan_.m_behaviour_.app_vx_ = 0;
            m_plan_.m_behaviour_.app_vy_ = 0;
            m_plan_.m_behaviour_.app_w_  = 0;
            setEthercatCommond();
            setShoot();
        }
        /** 机器人在开始之前的跑位. 开始静态传接球的目标点计算*/
        else if(match_mode_ == PARKINGROBOT)
        {
            DPoint br = DPoint(0.0, 0.0) - robot_pos_;
            m_plan_.move2Positionwithobs_noball(parking_point[world_model_info_.AgentID_-1]);
            if(robot_pos_.distance(parking_point[world_model_info_.AgentID_-1]) < 10.0)
            {
                m_plan_.positionAvoidObs2(br.angle().radian_);
            }
        }
        else if(match_mode_ == STARTROBOT)// 机器人正式比赛了，进入start之后的机器人状态
        {
            normalGame();
        } // start部分结束

#ifdef THREEPLAYER
        pubStrategyInfo();  // 发送策略消息让其他机器人看到，这一部分一般用于多机器人之间的协同
#endif

    }


    void normalGame()
    {
        DPoint br, target;
        //ROS_INFO("robot%d normalGame", world_model_info_.AgentID_);
        m_plan_.m_subtargets_.ball_pos_ = ball_pos_;
        m_plan_.m_subtargets_.robot_pos_ = robot_pos_;
        m_plan_.robot_pos_=robot_pos_;
        m_plan_.robot_ori_=robot_ori_;
        m_plan_.ball_pos_=ball_pos_;
        m_plan_.m_behaviour_.past_ball_vel=past_ball_vel;
        m_plan_.m_behaviour_.past_robot_vel=past_robot_vel;
        cout<<"ball :("<<ball_pos_.x_<<","<<ball_pos_.y_<<")"<<endl;
        cout<<"robot :("<<robot_pos_.x_<<","<<robot_pos_.y_<<")"<<endl;
        cout<<"obs :("<<opp_robot_.x_<<","<<opp_robot_.y_<<")"<<endl;

        if(robot_pos_.x_==0.0 && robot_pos_.y_==0.0 && ball_pos_.x_==0.0 && ball_pos_.y_==0.0)
        {
            m_strategy_->selected_action_ = No_Action;
            world_model_info_.Opponents_.push_back(DPoint(0.0,0.0));
#ifdef THREEPLAYER
            world_model_info_.Opponents_.push_back(DPoint(0.0,0.0));
            world_model_info_.Opponents_.push_back(DPoint(0.0,0.0));
#endif
        }

#ifdef THREEPLAYER
        opp_robot_ = world_model_info_.Opponents_[m_plan_.oppneartargetid(ball_pos_)-1];//对方距球最近的机器人位置
        world_model_info_.pass_ID_ = m_plan_.ourneartargetid(ball_pos_);
        world_model_info_.pass_pt_ = world_model_info_.RobotInfo_[world_model_info_.pass_ID_-1].getLocation();
        world_model_info_.catch_ID_ = m_plan_.ourneartargetid(world_model_info_.RobotInfo_[world_model_info_.pass_ID_-1].getLocation());
        world_model_info_.catch_pt_ = world_model_info_.RobotInfo_[world_model_info_.catch_ID_-1].getLocation();
        //////////////world_model_info_.can_pass_
#else
        opp_robot_ = world_model_info_.Opponents_[0];
#endif

#ifdef THREEPLAYER
        if(world_model_info_.pass_ID_ == world_model_info_.AgentID_) //Active
        {
#endif
            if(world_model_info_.BallInfo_[world_model_info_.AgentID_-1].isLocationKnown() == false)
            {
                m_strategy_->selected_action_ = CanNotSeeBall;
            }
            else if(ball_pos_.distance(opp_robot_) < LIMITDRIBLLEDIS && ball_pos_.distance(opp_robot_) < ball_pos_.distance(robot_pos_)) //对方带球
            {
#ifdef THREEPLAYER
                m_strategy_->selected_action_ = CanNotSeeBall;
#else
                m_strategy_->selected_action_ = Positioned_Static;
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
                opp_robot_ = world_model_info_.Opponents_[m_plan_.oppneartargetid(DPoint(-FIELD_LENGTH*LENGTH_FIX/2, 0.0))-1];//对方距我方球门最近的机器人
#endif
                target = DPoint((-FIELD_LENGTH*LENGTH_FIX/2+opp_robot_.x_)/2,opp_robot_.y_/2);
                br = opp_robot_ - robot_pos_;
                m_plan_.move2Positionwithobs_noball(target);
                if(robot_pos_.distance(target) < 10.0)
                {
                    m_plan_.positionAvoidObs2(br.angle().radian_);
                }
            }
            if(m_strategy_->selected_action_ == TurnForShoot) // Active 踢球准备
            {
                ROS_INFO("TurnForShoot");
                if(isBallHandle() != true)
                {
                    m_strategy_->selected_action_ = Catch_Positioned;
                }
                else
                {///////////////////////////////////////////////
                    target = DPoint(FIELD_LENGTH*LENGTH_FIX/2, 0.0);
                    br = target - robot_pos_;
#ifdef THREEPLAYER
                    opp_robot_ = world_model_info_.Opponents_[m_plan_.oppneartargetid(DPoint(FIELD_LENGTH*LENGTH_FIX/2, 0.0))]; //对方守门员
#endif
                    if(robot_pos_.distance(target) < dist_KICKGoal1 &&
                            br.angle().radian_ <= 60*DEG2RAD && br.angle().radian_ >= -60*DEG2RAD &&
                            fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD)
                    {
                        if(opp_robot_.x_-RADIUS>robot_pos_.x_ && opp_robot_.y_<oppGoalPosl.y_ && opp_robot_.y_>oppGoalPosr.y_)
                        {
                            m_strategy_->selected_action_ = Positioned_Static;
                        }
                        else
                        {
                            m_strategy_->selected_action_ = AtShootSituation;
                        }
                    }
                    else
                    {
                        m_strategy_->selected_action_ = Positioned;
                    }
                }
            }
#ifndef THREEPLAYER
            if(m_strategy_->selected_action_ == Positioned_Static) //移动到防守位置
            {
                ROS_INFO("Positioned_Static");
#ifdef THREEPLAYER
                opp_robot_ = world_model_info_.Opponents_[m_plan_.oppneartargetid(ball_pos_)-1];//对方距球最近的机器人位置
#endif
                target = DPoint(ourGoalPosl.x_,opp_robot_.y_);
                if(opp_robot_.y_ > 0.0)
                {
                    target.y_ += opp_robot_.y_*(ourGoalPosl.x_-opp_robot_.x_)/(opp_robot_.x_-FIELD_LENGTH*LENGTH_FIX/2);
                }
                if(robot_pos_.distance(target) < 10.0)
                {
                    m_plan_.positionAvoidObs(ball_pos_);
                }
                else
                {
                    m_plan_.move2Positionwithobs_noball(target);
                }

                if(opp_robot_.distance(ball_pos_) > robot_pos_.distance(ball_pos_) || opp_robot_.distance(ball_pos_) < LIMITDRIBLLEDIS)
                {
                    m_strategy_->selected_action_ = Catch_Positioned;
                }
            }
#endif
            if(m_strategy_->selected_action_ == Catch_Positioned) //走到球的位置并完成抓球
            {
                ROS_INFO("Catch_Positioned");
                br = ball_pos_ - robot_pos_;
                if(fabs(br.angle().radian_ - robot_ori_.radian_) < 15.0*DEG2RAD)
                {
                    m_plan_.move2Positionwithobs_noball(ball_pos_, ConstDribbleDisFirst);
                    if(ball_pos_.distance(robot_pos_) < ConstDribbleDisFirst)
                    {
                        m_plan_.positionAvoidObs2(br.angle().radian_);
                        if(fabs(br.angle().radian_ - robot_ori_.radian_) < 5.0*DEG2RAD)
                        {
                            if(isBallHandle() != true)
                            {
                                m_plan_.move2Positionwithobs_noball(ball_pos_, ConstDribbleDisSecond);
                            }
                            else
                            {
                                m_strategy_->selected_action_ = Positioned;
                            }
                        }
                    }
                }
                else
                {
                    m_plan_.positionAvoidObs2(br.angle().radian_);
                }
            }
            if(m_strategy_->selected_action_ == Positioned) //移动到目标位置 move to position 射门位置
            {
                ROS_INFO("Positioned");

#ifdef THREEPLAYER
                opp_robot_ = world_model_info_.Opponents_[m_plan_.oppneartargetid(ball_pos_)-1];//对方距球最近的机器人位置
#endif
                br = -opp_robot_ - robot_pos_;
                if(robot_pos_.distance(opp_robot_)<100.0 && fabs(fabs(br.angle().radian_)-fabs(robot_ori_.radian_))>90*DEG2RAD)
                {
                    br = -opp_robot_ - robot_pos_;
                    m_plan_.positionAvoidObs2(br.angle().radian_, 20.0*DEG2RAD);
                }
                else
                {
                    m_plan_.m_behaviour_.app_w_ = 0.0;
                    target = DPoint(FIELD_LENGTH*LENGTH_FIX/2, 0.0);
                    br = target - robot_pos_;
                    if(br.angle().radian_ > 60*DEG2RAD)
                    {
                        m_plan_.move2Positionwithobs_noball(DPoint(robot_pos_.x_,robot_pos_.y_+20.0));
                    }
                    else if(br.angle().radian_ < -60*DEG2RAD)
                    {
                        m_plan_.move2Positionwithobs_noball(DPoint(robot_pos_.x_,robot_pos_.y_-20.0));
                    }
                    else
                    {
                        if(robot_pos_.distance(target) < dist_KICKGoal1)
                        {
                            if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD)
                            {
                                m_plan_.move2Positionwithobs_noball(target, dist_KICKGoal2);
                            }
                            else
                            {
                                m_plan_.positionAvoidObs2(br.angle().radian_);
                            }
                        }
                        else
                        {
                            m_plan_.move2Positionwithobs_noball(target, dist_KICKGoal1);
                        }
                    }
                }
            }
            if(m_strategy_->selected_action_ == Positioned_Static) //到射门位置 如果对方机器人堵住门就转向
            {
                ROS_INFO("Positioned_Static");
#ifdef THREEPLAYER
                opp_robot_ = world_model_info_.Opponents_[m_plan_.oppneartargetid(DPoint(FIELD_LENGTH*LENGTH_FIX/2, 0.0))]; //对方守门员
#endif
                double rad_our2oppl = (DPoint(opp_robot_.x_, opp_robot_.y_+OBLE_RADIUS+5.0)-robot_pos_).angle().radian_;
                double rad_our2oppr = (DPoint(opp_robot_.x_, opp_robot_.y_-OBLE_RADIUS-5.0)-robot_pos_).angle().radian_;
                double rad_our2goall = (oppGoalPosl-robot_pos_).angle().radian_;
                double rad_our2goalr = (oppGoalPosr-robot_pos_).angle().radian_;
                if(rad_our2oppr < rad_our2goalr)
                {
                    if(robot_ori_.radian_ > max(rad_our2oppl,rad_our2goalr) && robot_ori_.radian_ < rad_our2goall)
                    {
                        m_strategy_->selected_action_ = AtShootSituation;
                    }
                    else
                    {
                        m_plan_.positionAvoidObs2((max(rad_our2oppl,rad_our2goalr)+rad_our2goall)/2);
                    }
                }
                else if(rad_our2oppl > rad_our2goall)
                {
                    if(robot_ori_.radian_ < min(rad_our2oppr,rad_our2goall) && robot_ori_.radian_ > rad_our2goalr)
                    {
                        m_strategy_->selected_action_ = AtShootSituation;
                    }
                    else
                    {
                        m_plan_.positionAvoidObs2((min(rad_our2oppr,rad_our2goall)+rad_our2goalr)/2);
                    }
                }
                else if(rad_our2oppl>rad_our2goall && rad_our2oppr<rad_our2goalr)
                {
                    m_plan_.move2Positionwithobs_noball(DPoint(robot_pos_.x_-20.0,robot_pos_.y_));
                }
                else
                {
                    if(fabs(opp_robot_.y_+OBLE_RADIUS+5.0-oppGoalPosl.y_) > fabs(opp_robot_.y_-OBLE_RADIUS-5.0-oppGoalPosr.y_))
                    {
                        if(robot_ori_.radian_ < rad_our2goall && robot_ori_.radian_ > rad_our2oppl)
                        {
                            m_strategy_->selected_action_ = AtShootSituation;
                        }
                        else
                        {
                            m_plan_.positionAvoidObs2((rad_our2oppl+rad_our2goall)/2);
                        }
                    }
                    else
                    {
                        if(robot_ori_.radian_ > rad_our2goalr && robot_ori_.radian_ < rad_our2oppr)
                        {
                            m_strategy_->selected_action_ = AtShootSituation;
                        }
                        else
                        {
                            m_plan_.positionAvoidObs2((rad_our2oppr+rad_our2goalr)/2);
                        }
                    }
                }
            }
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
                target = ball_pos_ + opp_robot_;
                target.x_ /= 2.0;
                target.y_ /= 2.0;
                if(ball_pos_.distance(opp_robot_) < 60.0) //对方带球
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
                target = DPoint((-FIELD_LENGTH*LENGTH_FIX/2+opp_robot_.x_)/2,opp_robot_.y_/2);
                br = opp_robot_ - robot_pos_;
                //cout<<"target :("<<target.x_<<","<<target.y_<<")"<<endl;
                m_plan_.move2Positionwithobs_noball(target);
                if(robot_pos_.distance(target) < 10.0)
                {
                    m_plan_.positionAvoidObs2(br.angle().radian_);
                }
            }
            if(m_strategy_->selected_action_ == Catch_Positioned) //传球用  还没改
            {
                ROS_INFO("Catch_Positioned");
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
                    if(fabs(br.angle().radian_ - robot_ori_.radian_) < 5.0*DEG2RAD)
                    {
                        if(world_model_info_.RobotInfo_[world_model_info_.AgentID_-1].getDribbleState() == false)
                        {
                            m_plan_.move2Positionwithobs_noball(ball_pos_, ConstDribbleDisSecond);
                        }
                        else
                        {
                            m_strategy_->selected_action_ = Positioned;
                        }
                    }
                    else
                    {
                        m_plan_.positionAvoidObs2(br.angle().radian_);
                    }
                }
            }
            if(m_strategy_->selected_action_ == Positioned) //我方将要射门
            {
                ROS_INFO("Positioned");

                int opp_id = -1;
                double min_dist = INF;
                double dist_opp2oppgoal;
                double dist_our2oppgoal = DPoint(FIELD_LENGTH*LENGTH_FIX/2, 0.0).distance(world_model_info_.RobotInfo_[world_model_info_.pass_ID_-1].getLocation());
                for(int i=0; i<OPP_TEAM; ++i)
                {
                    dist_opp2oppgoal = world_model_info_.Opponents_[i].distance(DPoint(FIELD_LENGTH*LENGTH_FIX/2, 0.0));
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
                }
                else
                {
                    m_plan_.move2Positionwithobs_noball(target);
                }
            }

        }
        else//defender
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
                target = DPoint(-FIELD_LENGTH*LENGTH_FIX/2+RADIUS, 0.0);
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

                target = DPoint(-FIELD_LENGTH*LENGTH_FIX/2+RADIUS, ball_pos_.y_>0.0?min(ball_pos_.y_,ourGoalPosl.y_):max(ball_pos_.y_,ourGoalPosr.y_));/////////if i can use gauss....
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
        if(m_strategy_->selected_action_ == AtShootSituation) //踢球
        {
            ROS_INFO("AtShootSituation");

            setShoot(robot_pos_.distance(DPoint(FIELD_LENGTH*LENGTH_FIX/2, 0.0)));
        }
        if(m_strategy_->selected_action_ == No_Action)
        {
            ROS_INFO("No_Action");
            m_plan_.m_behaviour_.app_vx_ = 0.0;
            m_plan_.m_behaviour_.app_vy_ = 0.0;
            m_plan_.m_behaviour_.app_w_  = 0.0;
        }


        //运动
        setEthercatCommond();
    }


    void setEthercatCommond()
    {
        nubot_common::VelCmd command;  // 最后是解算出来的结果
        float vx,vy,w;
        vx = m_plan_.m_behaviour_.app_vx_;
        vy = m_plan_.m_behaviour_.app_vy_;
        w  = m_plan_.m_behaviour_.app_w_;
        m_plan_.m_behaviour_.last_app_vx_ = m_plan_.m_behaviour_.app_vx_;
        m_plan_.m_behaviour_.last_app_vy_ = m_plan_.m_behaviour_.app_vy_;
        m_plan_.m_behaviour_.last_app_w_  = m_plan_.m_behaviour_.app_w_;
        m_plan_.m_behaviour_.app_vx_ = 0;
        m_plan_.m_behaviour_.app_vy_ = 0;
        m_plan_.m_behaviour_.app_w_  = 0;
        command.Vx = vx ;
        command.Vy = vy ;
        command.w  = w  ;
        motor_cmd_pub_.publish(command);
    }

    void setShoot(double strength=0.0, int ShootPos=RUN)
    {
        nubot_common::Shoot shoot;
        nubot_common::BallHandle dribble;
        shoot.request.strength = strength;
        shoot.request.ShootPos = ShootPos;
        shoot_client_.call(shoot);
        dribble.request.enable = 0;
        ballhandle_client_.call(dribble);
    }

    bool isBallHandle()
    {
        nubot_common::BallHandle dribble;
        dribble.request.enable = 1;
        ballhandle_client_.call(dribble);
        return dribble.response.BallIsHolding;
    }

#ifdef THREEPLAYER
    void pubStrategyInfo()
    {
        nubot_common::StrategyInfo strategy_info;       // 这个消息的定义可以根据个人需要进行修改
        strategy_info.header.stamp = ros::Time::now();
        strategy_info.AgentID     = world_model_info_.AgentID_;

        strategy_info_pub_.publish(strategy_info);
    }
#endif
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
