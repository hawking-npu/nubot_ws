#ifndef _NUBOT_BEHAVIOUR_H
#define _NUBOT_BEHAVIOUR_H

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <string.h>
#include"nubot/core/core.hpp"
#include "common.hpp"

#include <ros/ros.h>
#include <nubot_common/VelCmd.h>

#define NB 0
#define NM 1
#define NS 2
#define ZO 3
#define PS 4
#define PM 5
#define PB 6

using namespace std;

namespace nubot{

/** Behaviour主要是底层的控制函数，控制机器人按照预定的轨迹运动*/

class Behaviour
{

public:
    Behaviour();
    ~ Behaviour();

    float basicPDControl(float pgain,float dgain, float err,float err1, float maxval);
    float basicPIDcontrol(float pgain,
                       float igain,
                       float dgain,
                       float currval,
                       float targetval,
                       float imaxlimiter,
                       float iminlimiter,
                       float& dstate,
                       float& istate );
    void fuzzyPIDcontrol(float &deltakp, float &deltaki,float &deltakd, float err,float err1);

    void move2PositionForDiff(float kp, float kalpha, float kbeta, DPoint target, float maxvel,
                              const DPoint  & _robot_pos,const Angle  & _robot_ori );
    void move2PositionForDiff(float kp, float kalpha, float kbeta, DPoint target0, DPoint target1,
                              float maxvel, const DPoint & _robot_pos,const Angle  & _robot_ori, const DPoint & _robot_vel);
    /** 采用PD控制运动到目标点*/
    void move2Position(float pval, float dval, DPoint target, float maxvel,
                       const DPoint  & _robot_pos,const Angle  & _robot_ori );
    void move2target(float pval, float dval,DPoint target, DPoint realtarvel, float maxvel,
                     const DPoint  & _robot_pos,const Angle  & _robot_ori);
    void traceTarget();
    void revDecoupleFromVel(float vx,float vy,float &positivemax_rev,float &negativemax_rev);
    /** rotate to the target orientation by using PD control*/
    void rotate2AbsOrienation(float pval, float dval, float orientation, float maxw,const Angle & _robot_ori);
    void rotate2RelOrienation(float pval, float dval, float rel_orientation,float maxw);
    void rotatetowardsSetPoint(DPoint point);
    void rotatetowardsRelPoint(DPoint rel_point,const DPoint  & _robot_pos,const Angle & _robot_ori);
    void clearBehaviorState();
    void setAppvx(double vx);
    void setAppvy(double vy);
    void setAppw(double w);
    void setTurn(bool isTurn);
    void accelerateLimit(const double &_acc_thresh = 20, const bool & use_convected_acc = true);
    void clear();
    
public:
    float app_vx_;
    float app_vy_;
    float app_w_;
    bool  isTurn_;
    float last_app_vx_;
    float last_app_vy_;
    float last_app_w_;
    float last_speed;
};

class FuzzyPID
{
public:
    const static int N=7;
private:
    float target;  //系统的控制目标
    float actual;  //采样获得的实际值
    float e;       //误差
    float e_pre_1; //上一次的误差
    float e_pre_2; //上上次的误差
    float de;      //误差的变化率
    float emax;    //误差基本论域上限
    float demax;   //误差辩化率基本论域的上限
    float delta_Kp_max;   //delta_kp输出的上限
    float delta_Ki_max;   //delta_ki输出上限
    float delta_Kd_max;   //delta_kd输出上限
    float Ke;      //Ke=n/emax,量化论域为[-3,-2,-1,0,1,2,3]
    float Kde;     //Kde=n/demax,量化论域为[-3,-2,-1,0,1,2,3]
    float Ku_p;    //Ku_p=Kpmax/n,量化论域为[-3,-2,-1,0,1,2,3]
    float Ku_i;    //Ku_i=Kimax/n,量化论域为[-3,-2,-1,0,1,2,3]
    float Ku_d;    //Ku_d=Kdmax/n,量化论域为[-3,-2,-1,0,1,2,3]
    int Kp_rule_matrix[N][N];//Kp模糊规则矩阵
    int Ki_rule_matrix[N][N];//Ki模糊规则矩阵
    int Kd_rule_matrix[N][N];//Kd模糊规则矩阵
    string mf_t_e;       //e的隶属度函数类型
    string mf_t_de;      //de的隶属度函数类型
    string mf_t_Kp;      //kp的隶属度函数类型
    string mf_t_Ki;      //ki的隶属度函数类型
    string mf_t_Kd;      //kd的隶属度函数类型
    float *e_mf_paras; //误差的隶属度函数的参数
    float *de_mf_paras;//误差的偏差隶属度函数的参数
    float *Kp_mf_paras; //kp的隶属度函数的参数
    float *Ki_mf_paras; //ki的隶属度函数的参数
    float *Kd_mf_paras; //kd的隶属度函数的参数
    float Kp;
    float Ki;
    float Kd;
    float A;
    float B;
    float C;
    void showMf(const string & type,float *mf_paras);      //显示隶属度函数的信息
    void setMf_sub(const string & type,float *paras,int n);//设置模糊隶属度函数的子函数
public:
    FuzzyPID(float e_max,float de_max,float kp_max,float ki_max,float kd_max,float Kp0,float Ki0,float Kd0);
    FuzzyPID(float *fuzzyLimit,float *pidInitVal);
    ~FuzzyPID();
    float trimf(float x,float a,float b,float c);          //三角隶属度函数
    float gaussmf(float x,float ave,float sigma);          //正态隶属度函数
    float trapmf(float x,float a,float b,float c,float d); //梯形隶属度函数
    void setMf(const string & mf_type_e,float *e_mf,
                   const string & mf_type_de,float *de_mf,
                   const string & mf_type_Kp,float *Kp_mf,
                   const string & mf_type_Ki,float *Ki_mf,
                   const string & mf_type_Kd,float *Kd_mf); //设置模糊隶属度函数的参数
    void setRuleMatrix(int kp_m[N][N],int ki_m[N][N],int kd_m[N][N]);  //设置模糊规则
    float realize(float t,float a);  //实现模糊控制
    void showInfo();   //显示该模糊控制器的信息
};
}

#endif // _NUBOT_BEHAVIOUR_H
