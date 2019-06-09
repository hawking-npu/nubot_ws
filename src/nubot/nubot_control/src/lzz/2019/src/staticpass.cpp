#include "nubot/nubot_control/staticpass.h"

using namespace nubot;

StaticPass::StaticPass()
{
    isPosition_=false;
    m_nCanBeInPenaltyArea=0;
    m_nPassNumber_=-1;
    m_nCatchNumber_=-1;
    ballNumber_=-1;
    ballPos_=DPoint(0,0);
    backFieldPoint_=DPoint(-600,0);
    for(int i=0;i<OUR_TEAM;i++)
    {
        isAllocation_[i]=false;
        targetInit_[i]=DPoint(0,0);
    }
    targetInit_[0]=DPoint(-890,0);                 //静态站位时，守门员基本位置恒定
    target_=DPoint(0,0);                        //为分配的目标点
    m_nPassNumber_=0;
    m_nCatchNumber_=0;
}

StaticPass::~StaticPass()
{}

void StaticPass::staticReady_()                    //判断何种站位
{
    switch (world_model_->CoachInfo_.MatchMode)
    {
    case OUR_KICKOFF:
        //OurkickoffReady_();
        OurDefaultReady_();
        break;
    case OPP_KICKOFF:
        //OppkickoffReady_();
        OurDefaultReady_();
        break;
    case OUR_FREEKICK:
        OurDefaultReady_();
        break;
    case OPP_FREEKICK:
        //OppDefaultReady_();
        OurDefaultReady_();
        break;
    case OUR_GOALKICK:
        OurDefaultReady_();
        break;
    case OPP_GOALKICK:
        //OppDefaultReady_();
        OurDefaultReady_();
        break;
    case OUR_CORNERKICK:
        OurDefaultReady_();
        break;
    case OPP_CORNERKICK:
        //OppDefaultReady_();
        OurDefaultReady_();
        break;
    case OUR_THROWIN:
        OurDefaultReady_();
        break;
    case OPP_THROWIN:
        //OppDefaultReady_();
        OurDefaultReady_();
        break;
    case OUR_PENALTY:
        //OurPenaltyReady_();
        OurDefaultReady_();
        break;
    case OPP_PENALTY:
        //OppPenaltyReady_();
        OurDefaultReady_();
        break;
    case DROPBALL:
        //DropBallReady_();
        OurDefaultReady_();
        break;
    default:
        break;
    }
}

void StaticPass::OurDefaultReady_()                   //我方发球默认的站位
{
    ROS_INFO("nubotcontrol OurDefaultReady_");
    DPoint _robot_pos = m_strategy_->m_plan_->robot_pos_;
    Angle _robot_ori = m_strategy_->m_plan_->robot_ori_;
    switch(world_model_->AgentID_)  // 十分简单的实现，固定的站位，建议动态调整站位，写入staticpass.cpp中
    {                                   // 站位还需要考虑是否犯规，但是现在这个程序没有考虑。
        case 1:
            target_ = DPoint(-850.0,0.0);
            move2target(target_,_robot_pos,_robot_ori);
        break;
        case 2:
            target_ = ballPos_;
            move2target(target_,_robot_pos,_robot_ori,100.0);
        break;
        case 3:
            target_ = ballPos_;
            move2target(target_,_robot_pos,_robot_ori,200.0);
        break;
        case 4:
            target_ = DPoint(-450.0,200.0);
            move2target(target_,_robot_pos,_robot_ori);
        break;
        case 5:
            target_ = DPoint(-450.0,-200.0);
            move2target(target_,_robot_pos,_robot_ori);
        break;
    }
}

void StaticPass::OppDefaultReady_()               //对方发球默认站位
{
    ROS_INFO("nubotcontrol OppDefaultReady_");
    DPoint _robot_pos = m_strategy_->m_plan_->robot_pos_;
    Angle _robot_ori = m_strategy_->m_plan_->robot_ori_;
    switch(world_model_->AgentID_)  // 十分简单的实现，固定的站位，建议动态调整站位，写入staticpass.cpp中
    {                                   // 站位还需要考虑是否犯规，但是现在这个程序没有考虑。
        case 1:
            target_ = DPoint(-850.0,0.0);
            move2target(target_,_robot_pos,_robot_ori);
        break;
        case 2:
            target_ = DPoint(-150.0,100.0);
            move2target(target_,_robot_pos,_robot_ori);
        break;
        case 3:
            target_ = DPoint(-150.0,-100.0);
            move2target(target_,_robot_pos,_robot_ori);
        break;
        case 4:
            target_ = DPoint(-450.0,200.0);
            move2target(target_,_robot_pos,_robot_ori);
        break;
        case 5:
            target_ = DPoint(-450.0,-200.0);
            move2target(target_,_robot_pos,_robot_ori);
        break;
    }
}

void StaticPass::OurPenaltyReady_()
{

}

void StaticPass::OppPenaltyReady_()               //对方penalty发球
{

}

void StaticPass::OurkickoffReady_()                   //我方kickoff发球站位
{

}

void StaticPass::OppkickoffReady_()               //对方kickoff发球站位
{

}

void StaticPass::DropBallReady_()                  //dropball站位
{

}

void StaticPass::targetInitialize()
{
    // 给所有站位点赋值，防止后面也没有赋值导致出错
    targetInit_[0]=DPoint(-890,0);                  //静态站位时，守门员基本位置恒定
    targetInit_[1]=DPoint(-500,200);
    targetInit_[2]=DPoint(-500,-200);
    targetInit_[3]=DPoint(-200,100);
    targetInit_[4]=DPoint(-200,-100);
}

void StaticPass::move2target(DPoint target, DPoint _robot_pos, Angle _robot_ori, double distance_thres)
{
    DPoint br = ballPos_-_robot_pos;
    if(_robot_pos.distance(target) > distance_thres)
    {
        m_strategy_->m_plan_->move2Positionwithobs_noball(target,max_vel,max_acc);
        //m_strategy_->m_plan_->m_behaviour_.move2Position(1.0,0.1,target,max_vel,_robot_pos,_robot_ori);
    }
    else
    {
        m_strategy_->m_plan_->m_behaviour_.setAppvx(0.0);
        m_strategy_->m_plan_->m_behaviour_.setAppvy(0.0);
        if(br.angle().radian_-_robot_ori.radian_ > 8.0/180.0)
            m_strategy_->m_plan_->m_behaviour_.rotate2AbsOrienation(1.0,0.1,br.angle().radian_,max_w,_robot_ori.radian_);
        else
            m_strategy_->m_plan_->m_behaviour_.setAppw(0);
    }
}

