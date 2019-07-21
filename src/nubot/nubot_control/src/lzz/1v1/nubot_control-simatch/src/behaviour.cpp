#include "nubot/nubot_control/behaviour.hpp"
#include "nubot/nubot_control/mydefine.hpp"
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


float Behaviour::basicPDControl(float pgain,float dgain, float err,//targetval
                     float err1,//now vel
                     float maxval)
{
    float temp;
    temp = pgain*err + dgain*(err-err1);
    if(temp>maxval)
    {
        temp=maxval;
    }
    return temp;
}

float Behaviour::basicPDControl2(float pgain, float dgain, float err, float err1, float err2, float maxval)
{
    float temp;
    temp = pgain*err + dgain*(err+err2-2*err1);
    if(temp>maxval)
    {
        temp=maxval;
    }
    return temp;
}

void Behaviour::move2PositionForDiff(float kp, float kalpha, float kbeta, DPoint target, float maxvel,
                          const DPoint  & _robot_pos,const Angle  & _robot_ori ){}
void Behaviour::move2PositionForDiff(float kp, float kalpha, float kbeta, DPoint target0, DPoint target1,
                          float maxvel, const DPoint & _robot_pos,const Angle  & _robot_ori, const DPoint & _robot_vel){}
// 采用PD控制运动到目标点
void Behaviour::move2Position(float pval, float dval, DPoint target, float maxvel,
                   const DPoint  & _robot_pos,const Angle & _robot_ori )//世界坐标系
{
    DPoint temp = target-_robot_pos;
    double tar_theta = temp.angle().radian_;

    temp.x_=temp.norm(); temp.y_=0;
    DPoint tmp;

    //前进
    //double delta_val;
    if(temp.norm()>10)
    {
        tmp.x_=temp.norm()*0.7*cos(tar_theta - _robot_ori.radian_);
        tmp.y_=temp.norm()*0.7*sin(tar_theta - _robot_ori.radian_);
        //cout<<"want vel: "<<tmp.x_<<' '<<tmp.y_<<endl;
        //accelerateLimit();
        double a=tmp.x_*tmp.x_+tmp.y_*tmp.y_;
        double b=maxvel*maxvel;
        if(a > b)
        {
            tmp.x_=sqrt(b/a)*tmp.x_;
            tmp.y_=sqrt(b/a)*tmp.y_;
        }
        if(past_robot_vel.size() == 0)
        {
            app_vx_ = tmp.x_;
            app_vy_ = tmp.y_;
            accelerateLimit();
        }
        else if(past_robot_vel.size() == 1)
        {
            vel1=past_robot_vel[0];
            app_vx_ = basicPDControl(pval,dval,tmp.x_,vel1.Vx,maxvel);
            app_vy_ = basicPDControl(pval,dval,tmp.y_,vel1.Vy,maxvel);
            //app_vx_ = pval*tmp.x_ + dval*(tmp.x_-vel1.Vx)/update_T;
            //app_vy_ = pval*tmp.y_ + dval*(tmp.y_-vel1.Vy)/update_T;
            accelerateLimit();
            //cout<<"past vel0: "<<vel1.Vx<<' '<<vel1.Vy<<endl;
        }
        else
        {
            vel1=past_robot_vel[0];
            vel2=past_robot_vel[1];
            app_vx_ = basicPDControl2(pval,dval,tmp.x_,vel1.Vx,vel2.Vx,maxvel);
            app_vy_ = basicPDControl2(pval,dval,tmp.y_,vel1.Vy,vel2.Vy,maxvel);
            //app_vx_ = pval*tmp.x_ + dval*(tmp.x_-vel2.Vx*vel2.Vx+vel1.Vx)/(update_T*update_T);
            //app_vy_ = pval*tmp.y_ + dval*(tmp.y_-vel2.Vy*vel2.Vy+vel1.Vy)/(update_T*update_T);
            accelerateLimit();
            //cout<<"past vel0: "<<vel1.Vx<<' '<<vel1.Vy<<endl;
            //cout<<"past vel1: "<<vel2.Vx<<' '<<vel2.Vy<<endl;
        }
    }
    else
    {
        app_vx_=0.0;
        app_vy_=0.0;
    }
}

void Behaviour::move2target(float pval, float dval,DPoint target, DPoint realtarvel/*目标速度*/, float maxvel,
                 const DPoint  & _robot_pos,const Angle  & _robot_ori)
{
    DPoint after_target;
    double time=target.norm()*2/maxvel;
    after_target.x_=realtarvel.x_*time, after_target.y_=realtarvel.y_*time;
    after_target+=target;
    move2Position(pval,dval,after_target,maxvel,_robot_pos,_robot_ori);
}

void Behaviour::traceTarget(){}
void Behaviour::revDecoupleFromVel(float vx,float vy,float &positivemax_rev,float &negativemax_rev){}




/////////////////
// rotate to the target orientation by using PD control
void Behaviour::rotate2AbsOrienation(float pval, float dval, float orientation,float maxw,const Angle & _robot_ori, double angle_thres)//自身坐标系
{
    double tmp = orientation - _robot_ori.radian_;
    if(fabs(tmp) > angle_thres)        // 容许误差为5度
    {
        if(past_robot_vel.size()==0)
            app_w_ = tmp;
        else if(past_robot_vel.size()==1)
        {
            vel1=past_robot_vel[0];
            app_w_ = basicPDControl(pval,dval,tmp,vel1.w,maxw);
            //app_w_ = pval*tmp + dval*(tmp-vel1.w)/update_T;
        }
        else
        {
            vel1=past_robot_vel[0];
            vel2=past_robot_vel[1];
            app_w_ = basicPDControl2(pval,dval,tmp,vel1.w,vel2.w,maxw);
            //app_w_ = pval*tmp + dval*(tmp-vel2.w*vel2.w+vel1.w)/(update_T*update_T);
        }
    }
    else
    {
        app_w_ = 0;
    }
    /*
    double temp;
    temp=orientation; temp-=_robot_ori.radian_;
    double delta_w;
    if(fabs(temp)>8.0/180.0*SINGLEPI_CONSTANT)
    {
        temp-=app_w_*update_T;
        delta_w=maxw-app_w_;
        app_w_=pval*delta_w+dval*(app_w_-last_app_w_)/update_T;
        last_app_vx_=app_vx_;
        last_app_vy_=app_vy_;
        last_app_w_ =app_w_ ;
    }
    else
    {
        app_vx_=0.0;
        app_vy_=0.0;
        app_w_=0;
    }*/
}
void rotate2RelOrienation(float pval, float dval, float rel_orientation,float maxw);
void rotatetowardsSetPoint(DPoint point)
{
}

void Behaviour::rotatetowardsRelPoint(DPoint rel_point, const DPoint  & _robot_pos,const Angle & _robot_ori)
{
    DPoint temp;
    temp=rel_point-_robot_pos;

    rotate2AbsOrienation(0.7,0.3,temp.angle().radian_,max_w,_robot_ori);
}

/////////////////



void Behaviour::setAppvx(double vx) { app_vx_=vx;}
void Behaviour::setAppvy(double vy) { app_vy_=vy;}
void Behaviour::setAppw(double w) { app_w_=w;}
void Behaviour::setTurn(bool isTurn) { isTurn_=isTurn;}

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

void Behaviour::accelerateLimit(const double &_acc_thresh/* = 20*/, const bool & use_convected_acc/* 是否使用传送过来的加速度*/)
{
    DPoint app_v, last_app_v, _acc;
    app_v.x_=app_vx_, app_v.y_=app_vy_;
    last_app_v.x_=last_app_vx_, last_app_v.y_=last_app_vy_;
    _acc.x_=app_vx_-last_app_vx_, _acc.y_=app_vy_-last_app_vy_;
    if(_acc.norm()/update_T>_acc_thresh)
    {//求以前帧的速度矢量末端点为圆心，最大加速度为半径的圆与两速度矢量末端点连线的交点
        DPoint solution, solution1, solution2;
        /*
         * (x-last_app_vx_)^2+(y-last_app_vy_)^2=_acc_thresh^2
         *
         * if(last_app_vx_-app_vx_==0) x=last_app_vx_;
         * if(last_app_vy_-app_vy_==0) y=last_app_vy_;
         * else (x-app_vx_)/(last_app_vx_-app_vx_)=(y-app_vy_)/(last_app_vy_-app_vy_)
         */
       if(last_app_vx_-app_vx_<EPS)//最精确的情况是=0
        {
            solution1.x_=solution2.x_=last_app_vx_;
            solution1.y_=last_app_vy_+_acc_thresh;
            solution2.y_=last_app_vy_-_acc_thresh;
        }
        else if(last_app_vy_-app_vy_<EPS)
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
            double m=(last_app_vx_*app_vy_-app_vx_*last_app_vy_)/(last_app_vx_-app_vx_);

            double a=1+k*k;
            double b=2*k*(m-last_app_vy_)-2*last_app_vx_;
            double c=last_app_vx_*last_app_vx_+(m-last_app_vy_)*(m-last_app_vy_)-_acc_thresh*_acc_thresh;

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
        if(solution1.distance(app_v)+solution1.distance(last_app_v)-app_v.distance(last_app_v)<EPS)//最精确的情况是=0
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


