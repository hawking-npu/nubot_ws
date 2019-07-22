#ifndef __NUBOT_CORE_HPP__
#define __NUBOT_CORE_HPP__


#include "Circle.hpp"
#include "Angle.hpp"
#include "DPoint.hpp"
#include "PPoint.hpp"
#include "Line.hpp"

#define  SIMULATION
//#define THREEPLAYER
#define  NET_TYPE "wlp3s0"

//#define using_openmp


#ifdef  SIMULATION
const double ConstDribbleDisFirst  = 50;
const double ConstDribbleDisSecond = 40;
#else
const double ConstDribbleDisFirst  = 40;
const double ConstDribbleDisSecond = 35;
#endif

const double WIDTHRATIO4FIELD = 1.0;
const double WIDTH_RATIO= 1.0;
//using namespace nubot;
const int NOT_DATAUPDATE = 300; //数据没有更新的阈值，比如通信过程中时间大于300ms为更新数据，默认为失效
#ifdef THREEPLAYER
const int OUR_TEAM = 3 ;        //自己机器人个数 5
const int OPP_TEAM = 3 ;        //对方的机器人个数 7
#else
const int OUR_TEAM = 1 ;        //自己机器人个数 5
const int OPP_TEAM = 1 ;        //对方的机器人个数 7
#endif

const int ROLENUM = 7;

/** 一些常用的场地参数 */
#ifdef SIMULATION
const int FIELD_LENGTH= 1800;
const int FIELD_WIDTH = 400;
#else
const int FIELD_LENGTH= 600;//1800;    //场地参数，长度
const int FIELD_WIDTH = 400;//1200;     //场地参度，长度
#endif
const double MAXDIS_FIELD = sqrt(FIELD_LENGTH*FIELD_LENGTH+FIELD_WIDTH*FIELD_WIDTH); //对角线长度

/** 场地上的几条水平和垂直线,，其他的信息都可以通过场地信息得到*/
#ifdef SIMULATION
const int FIELD_XLINE1 = 900;
const int FIELD_XLINE2 = 825;
const int FIELD_XLINE3 = 675;
const int FIELD_XLINE4 = 0;
const int FIELD_XLINE5 = -675;
const int FIELD_XLINE6 = -825;
const int FIELD_XLINE7 = -900;
const int FIELD_YLINE1 =  600;
const int FIELD_YLINE2 =  325;
const int FIELD_YLINE3 =  175;
const int FIELD_YLINE4 =  -175;
const int FIELD_YLINE5 =  -325;
const int FIELD_YLINE6 =  -600;
#else
const int FIELD_XLINE1 = 300;//900;
const int FIELD_XLINE2 = 260;//825;
const int FIELD_XLINE3 = 235;//675;
const int FIELD_XLINE4 = 0;
const int FIELD_XLINE5 = -235;//-675;
const int FIELD_XLINE6 = -260;//825;
const int FIELD_XLINE7 = -300;//-900;

const int FIELD_YLINE1 = 200;  // 600
const int FIELD_YLINE2 = 100;  // 325
const int FIELD_YLINE3 = 80;  // 175
const int FIELD_YLINE4 = -80; // -175
const int FIELD_YLINE5 = -100; // -325
const int FIELD_YLINE6 = -200; // -600
#endif
const int FIELD_CENTER_RADIUS = 60; // 150;
const int FIELD_POST_RADIUS = 0;
const int LOCATIONERROR = 10; // 30

///========================================================
const double EPS = 0.1;
const double INF = 0x3f3f3f;

////可能需要修改
const double update_T = 30;//30ms更新一次
const double LENGTH_FIX = 0.3;
const nubot::DPoint oppGoalPosr = nubot::DPoint( 890*LENGTH_FIX, -100*LENGTH_FIX);
const nubot::DPoint oppGoalPosl = nubot::DPoint( 890*LENGTH_FIX,  100*LENGTH_FIX);
const nubot::DPoint ourGoalPosr = nubot::DPoint(-890*LENGTH_FIX, -100*LENGTH_FIX);
const nubot::DPoint ourGoalPosl = nubot::DPoint(-890*LENGTH_FIX,  100*LENGTH_FIX);
const nubot::DPoint parking_point[3] = {nubot::DPoint(-800*LENGTH_FIX, 0.0), nubot::DPoint(-200*LENGTH_FIX, -100*LENGTH_FIX), nubot::DPoint(-200*LENGTH_FIX, 100*LENGTH_FIX)};

////需要修改
const double max_wacc = 3.0;//rad/s
const double max_acc = 50.0;//cm2/s
const double dist_KICKGoal1 = 300.0; //踢球最远距离
const double dist_KICKGoal2 = 70.0;
const double KA_MOVE = 1.0;
const double KB_MOVE = 0.01;
///========================================================

/** 比赛模式的一些定义，*/
enum Roles{GOALIE = 0 ,
           ACTIVE = 1,
           PASSIVE = 2,
           MIDFIELD = 3,
           ASSISTANT = 4,
           ACIDPASSIVE = 5,
           GAZER = 6 ,
           BLOCK = 7,
           NOROLE = 8,
           CATCHOFPASS = 9,
           PASSOFPASS =10};

enum MatchMode {
                 STOPROBOT  =  0,
                 OUR_KICKOFF = 1,
                 OPP_KICKOFF = 2,
                 OUR_THROWIN = 3,
                 OPP_THROWIN = 4,
                 OUR_PENALTY = 5,
                 OPP_PENALTY = 6,
                 OUR_GOALKICK = 7 ,
                 OPP_GOALKICK = 8,
                 OUR_CORNERKICK = 9,
                 OPP_CORNERKICK = 10,
                 OUR_FREEKICK = 11,
                 OPP_FREEKICK = 12,
                 DROPBALL     = 13,
                 STARTROBOT   = 15,
                 PARKINGROBOT = 25
               };

enum StrategyTyples {  STRATEGY_ATTACK = 0,
                       STRATEGY_DEFEND = 1,
                       STRATEGY_BALANCE =2,
                       STRATEGY_AUTO = 4,
                       NOSTRATEG =5
};

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

struct obs_info
{
    nubot::PPoint polar_pt;
    nubot::DPoint world_pt;
    double HRZ[4];
    double HRH[4*4];
};



#endif 

