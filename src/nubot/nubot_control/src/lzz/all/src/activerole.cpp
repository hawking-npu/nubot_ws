#include "nubot/nubot_control/activerole.h"

using namespace nubot;

ActiveRole::ActiveRole()
{
    stuckflg_ = false;
    pass_lock_ = 0;
    stucktime_ = 0;
    NeedEvaluat = false;
    dynamic_shoot_state_ = false;
    dynamic_shoot_count_ = 0;
    quick_shoot_state_ = false;
    quick_shoot_count_ = 0;
    currentstate_ = CanNotSeeBall;
    catch_in_ourfeild_ = false;
    kick_enable_ = false;
}
ActiveRole:: ~ActiveRole()
{
}
/*
public:
World_Model_Info * world_model_;
Plan  * m_plan_;
bool stuckflg_ ;             // 机器人当前是否堵转
int  stucktime_ ;            // 机器人堵转周期记录
bool NeedEvaluat ;           // 射门的ROS服务，于底层控制节点通信
bool dynamic_shoot_state_ ;  // 动态射门的状态
int  dynamic_shoot_count_ ;  // 动态射门的
bool quick_shoot_state_ ;    // 快速射门状态
int  quick_shoot_count_ ;
int  pass_lock_;
bool catch_in_ourfeild_;
DPoint target_shoot_;
Angle  ldetAng_shoot_;
Angle  rdetAng_shoot_;
unsigned char currentstate_;
bool   kick_enable_;         // 准备踢球
bool   dribble_enable_;      // 带球状态
float kick_force_;           // 踢球的力量
DPoint kick_target_;

FieldInformation fieldinformation_;
*/

void ActiveRole::process()
{
    //有威胁的目标判定
    //阻碍射门的目标判定
    //评估己方所有机器人进球的可能性

    selectCurrentState();
    selectCurrentAction(currentstate_);


    activeDecisionMaking();///
}
bool ActiveRole::checkPass(){}
void ActiveRole::clearActiveState()  //清空状态
{
    stuckflg_ = false;
    pass_lock_ = 0;
    stucktime_ = 0;
    NeedEvaluat = false;
    dynamic_shoot_state_ = false;
    dynamic_shoot_count_ = 0;
    quick_shoot_state_ = false;
    quick_shoot_count_ = 0;
    currentstate_ = CanNotSeeBall;
    catch_in_ourfeild_ =false;
    kick_enable_ =false;
}
bool ActiveRole::isNullInTrap(const std::vector<DPoint> & obs_info,const DPoint & robot_pos, const Angle & direction,//周围是否有障碍物
            double back_width, double front_width, double back_len, double front_len)
{
    std::vector<DPoint> pts, obs_pts;
    pts.clear(); //front
    obs_pts.clear(); //back

    DPoint pnt[4];
    pnt[0] = DPoint(front_len, front_width/2);
    DPoint tmp;
    double theta;
    for(int i=0; i<4; ++i)
    {
        tmp = pnt[i];
        theta = atan(tmp);
    }
    //坐标转换
    //robot_pos_

    DPoint ori;
    double theta_t;
    DPoint tar_now = target;
    tar_now -= robot_pos_;
    if(tar_now.x_ == 0)
    {
        if(tar_now.y_>0) { theta_t = PI/2; }
        else if(tar_now.y_<0) { theta_t = -PI/2; }
        else { theta_t = 0; }
    }
    else { theta_t = atan(tar_now.y_/tar_now.x_); }
    theta_t = -theta_t;
    ori = tar_now;
    tar_now.x_ = ori.x_*cos(theta_t)-ori.y_*sin(theta_t);
    tar_now.y_ = ori.x_*sin(theta_t)+ori.y_*cos(theta_t);

        //!转回世界坐标
        ori = subtargets_pos_;
        theta_t = -theta_t;
        tran_ob.x_ = ori.x_*cos(theta_t)-ori.y_*sin(theta_t);
        tran_ob.y_ = ori.x_*sin(theta_t)+ori.y_*cos(theta_t);
        tran_ob += robot_pos_;
        subtargets_pos_ = tran_ob;
    }


    for(int i=0; i<obs_info.size(); ++i){}
    return pnpoly(robot_pos, pts, obs_pts);
}
bool ActiveRole::pnpoly(DPoint test, const std::vector<DPoint> & pts, const std::vector<DPoint> & obs_pts)  //判断一个坐标点是否在不规则多边形内部
{
    bool flag = false;
    std::vector<DPoint> pt;
    pt.clear();

    DPoint max_pt;
    DPoint min_pt;
    max_pt.x_ = max_pt.y_ = -INF;
    min_pt.x_ = min_pt.y_ = INF;

    DPoint tmp;
    for(int i=0; i<pts.size(); ++i)
    {
        tmp = pts.at(i);
        pt.push_back(tmp);
        if(tmp.x_ > max_pt.x_) max_pt.x_ = tmp.x_;
        if(tmp.y_ > max_pt.y_) max_pt.y_ = tmp.y_;
        if(tmp.x_ < min_pt.x_) min_pt.x_ = tmp.x_;
        if(tmp.y_ < min_pt.y_) min_pt.y_ = tmp.y_;
    }
    for(int i=0; i<obs_pts.size(); ++i)
    {
        tmp = obs_pts.at(i);
        pt.push_back(tmp);
        if(tmp.x_ > max_pt.x_) max_pt.x_ = tmp.x_;
        if(tmp.y_ > max_pt.y_) max_pt.y_ = tmp.y_;
        if(tmp.x_ < min_pt.x_) min_pt.x_ = tmp.x_;
        if(tmp.y_ < min_pt.y_) min_pt.y_ = tmp.y_;
    }
    //横坐标和纵坐标的最大值和最小值，判断目标坐标点是否在这个四边形之内
    if(test.x_<min_pt.x_ || test.x_>max_pt.x_ || test.y_<min_pt.y_ || test.y_>max_pt.y_)
        return false;

    DPoint tmpi, tmpj;
    for(int i=0, j=pt.size()-1; i<pt.size(); j=i, ++i)
    {
        tmpi = pt.at(i);
        tmpj = pt.at(j);
        if( ((tmpi.y_>test.y_) != (tmpj.y_>test.y_)) &&
                (test.x_ < (tmpj.x_-tmpi.x_) * (test.y_-tmpi.y_) / (tmpj.y_-tmpi.y_) + tmpi.x_) )
            flag = !flag;
    }
    return flag;
}
void ActiveRole::activeDecisionMaking()
{
}

void ActiveRole::selectCurrentState()
{
    currentstate_ = CanNotSeeBall;///
    /*
    enum Actions
    {
        Stucked =0,
        Penalty =1,
        CanNotSeeBall =2,
        SeeNotDribbleBall =3,
        TurnForShoot =4, // Active
        AtShootSituation =5,
        TurnToPass=6,   //Dynamic pass
        StaticPass =7,
        AvoidObs=8,
        Catch_Positioned = 9,   //catch position
        Positioned =10,          //move to position
        Positioned_Static  =11, //static
        No_Action  =12,
    };
    */
}
void ActiveRole::selectCurrentAction(unsigned char state)
{
    double avoid_energy,energy;
    bool isNullFrontRobot;
    int label;
    caculateDribblingEnergy(avoid_energy,isNullFrontRobot);
    caculatePassEnergy(energy,label);
    if(avoid_energy > energy)
    {
        currentstate_ = AvoidObs;
    }
    else
    {
        currentstate_ = StaticPass;
    }
}
bool ActiveRole::evaluateKick(DPoint & kick_target,Angle & leftdelta,Angle &rightdelta){}
void ActiveRole::caculatePassEnergy(double & energy, int & label)
{
}
void ActiveRole::caculateDribblingEnergy(double & avoid_enegy,bool isNullFrontRobot)
{
}
void ActiveRole::selectDribblingOrPassing(bool isNullFrontRobot){}
void ActiveRole::findBall(){}
void ActiveRole::turn4Shoot(DPoint kicktarget){}
void ActiveRole::NewAvoidObs(){}
void ActiveRole::NewAvoidObsForPass(){}
void ActiveRole::activeCatchBall()
{
    m_plan_->move2Positionwithobs_noball(m_plan_->ball_pos_,max_vel,max_acc);
}
void ActiveRole::triggerShoot(DPoint target)
{
}
void ActiveRole::stuckProcess()
{
    stuckflg_ = true;
    stucktime_++;
    currentstate_ = Stucked;
}
void ActiveRole::kickball4Coop(DPoint target){}
bool ActiveRole::IsLocationInOppGoalArea(DPoint location) { return fieldinformation_.isOppGoal(location); }   // 是否在对方小禁区
bool ActiveRole::IsLocationInField(DPoint location) { return fieldinformation_.isInInterField2(location); }   // 是否在场地内
bool ActiveRole::IsLocationInOppPenalty(DPoint location) { return fieldinformation_.isOppPenalty(location); } // 是否在对方大禁区
bool ActiveRole::IsLocationInOurPenalty(DPoint location) { return fieldinformation_.isOurPenalty(location); } // 是否在我方大禁区
bool ActiveRole::IsLocationInOppField(DPoint location) { return fieldinformation_.isOppField(location); }     // 是否在对方半场
bool ActiveRole::IsLocationInOurField(DPoint location) { return fieldinformation_.isOurField(location); }     // 是否在己方半场
