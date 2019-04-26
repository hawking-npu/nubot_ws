#include "nubot/nubot_control/behaviour.hpp"
using namespace nubot;

Behaviour::Behaviour()
{
 app_vx_ = 0;
 app_vy_ = 0;
 app_w_  = 0;
 isTurn_ =false;
 last_app_vx_ = 0;
 last_app_vy_ = 0;
 last_app_w_ = 0;
}
Behaviour::~Behaviour()
{
}

/*
#define NB 0
#define NM 1
#define NS 2
#define ZO 3
#define PS 4
#define PM 5
#define PB 6


namespace nubot{

class Behaviour
{

public:

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
}

#endif // _NUBOT_BEHAVIOUR_H
*/
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
// 采用PD控制运动到目标点
void move2Position(float pval, float dval, DPoint target, float maxvel,
                   const DPoint  & _robot_pos,const Angle  & _robot_ori );
void move2target(float pval, float dval,DPoint target, DPoint realtarvel, float maxvel,
                 const DPoint  & _robot_pos,const Angle  & _robot_ori);
void traceTarget();
void revDecoupleFromVel(float vx,float vy,float &positivemax_rev,float &negativemax_rev);
// rotate to the target orientation by using PD control
void rotate2AbsOrienation(float pval, float dval, float orientation,float maxw,const Angle & _robot_ori);
void rotate2RelOrienation(float pval, float dval, float rel_orientation,float maxw);
void rotatetowardsSetPoint(DPoint point);
void rotatetowardsRelPoint(DPoint rel_point);
void Behaviour::clearBehaviorState()
{
    app_vx_=0;
    app_vy_=0;
    app_w_=0;
    isTurn_=false;
    last_app_vx_=0;
    last_app_vy_=0;
    last_app_w_=0;
    last_speed=0;
}

void Behaviour::setAppvx(double vx) { app_vx_=vx; }
void Behaviour::setAppvy(double vy) { app_vy_=vy; }
void Behaviour::setAppw(double w) { app_w_=w; }
void Behaviour::setTurn(bool isTurn) { isTurn_=isTurn; }

void Behaviour::accelerateLimit(const double &_acc_thresh = 20, const bool & use_convected_acc = true)
{
    DPoint app_v=new DPoint_(app_vx_, app_vy_);
    DPoint last_app_v=new DPoint_(last_app_vx_, last_app_vy_);
    DPoint _acc=new DPoint_(app_vx_-last_app_vx_, app_vy_-last_app_vy_);
    if(_acc.norm()/T>_acc_thresh)//T是每帧的时间间隔
    {//求以前帧的速度矢量末端点为圆心，最大加速度为半径的圆与两速度矢量末端点连线的交点
        DPoint solution, solution1, solution2;
        /*
         * (x-last_app_vx_)^2+(y-last_app_vy_)^2=_acc_thresh^2
         *
         * if(last_app_vx_-app_vx_==0) x=last_app_vx_;
         * if(last_app_vy_-app_vy_==0) y=last_app_vy_;
         * else (x-app_vx_)/(last_app_vx_-app_vx_)=(y-app_vy_)/(last_app_vy_-app_vy_)
         */
        if(last_app_vx_-app_vx_<eps)//最精确的情况是=0
        {
            solution1.x_=solution2.x_=last_app_vx_;
            solution1.y_=last_app_vy_+_acc_thresh;
            solution2.y_=last_app_vy_-_acc_thresh;
        }
        else if(last_app_vy_-app_vy_<eps)
        {
            solution1.y_=solution2.y_=last_app_vy_;
            solution1.x_=last_app_vx_+_acc_thresh;
            solution2.x_=last_app_vx_-_acc_thresh;
        }
        else
        {
            //(x-app_vx_)/(last_app_vx_-app_vx_)=(y-app_vy_)/(last_app_vy_-app_vy_)
            /**
             * 求圆和直线之间的交点
             * 直线方程：y = kx + b
             * 圆的方程：(x - m)² + (y - n)² = r²
             * x1, y1 = 线坐标1, x2, y2 = 线坐标2, m, n = 圆坐标, r = 半径
             */
            double k=(last_app_vy_-app_vy_)/(last_app_vx_-app_vx_);
            double b=(last_app_vx_*app_vy_-app_vx_*last_app_vy_)/(last_app_vx_-app_vx_);

            double a=1+k*k;
            double b=2*k*(b-last_app_vy_)-2*last_app_vx_;
            double c=last_app_vx_*last_app_vx_+(b-last_app_vy_)*(b-last_app_vy_)-_acc_thresh*_acc_thresh;

            double delt=b*b-4*a*c;
            if(delt>0)
            {
                solution1.x_=(-b+sqrt(delt))/(2*a);
                solution2.x_=(-b-sqrt(delt))/(2*a);
            }
            else if(delt==0)
            {
                solution1.x_=solution2.x_=-b/(2*a);
            }
            solution1.y_=k*solution1.x_+b;
            solution2.y_=k*solution2.x_+b;
        }
        if(solution1.distance(app_v)+solution1.distance(last_app_v)-app_v.distance(last_app_v)<eps)//最精确的情况是=0
        {
            solution=solution1;
        }
        else
        {
            solution=solution2;
        }

        app_vx_=solution.x_;
        app_vy_=solution.y_;
    }
}

void Behaviour::clear()
{
    app_vx_=0;
    app_vy_=0;
    app_w_=0;
    isTurn_=false;
    last_app_vx_=0;
    last_app_vy_=0;
    last_app_w_=0;
    last_speed=0;
}
