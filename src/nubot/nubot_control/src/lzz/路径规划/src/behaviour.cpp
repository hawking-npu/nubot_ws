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


void move2PositionForDiff(float kp, float kalpha, float kbeta, DPoint target, float maxvel,
                          const DPoint  & _robot_pos,const Angle  & _robot_ori );
void move2PositionForDiff(float kp, float kalpha, float kbeta, DPoint target0, DPoint target1,
                          float maxvel, const DPoint & _robot_pos,const Angle  & _robot_ori, const DPoint & _robot_vel);
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
        cout<<"want vel: "<<tmp.x_<<' '<<tmp.y_<<endl;
        //accelerateLimit();
        double a=tmp.x_*tmp.x_+tmp.y_*tmp.y_;
        double b=maxvel*maxvel;
        if(a > b)
        {
            tmp.x_=sqrt(b/a)*tmp.x_;
            tmp.y_=sqrt(b/a)*tmp.y_;
        }
        if(past_robot_vel.size()==0)
        {
            app_vx_ = tmp.x_;
            app_vy_ = tmp.y_;
        }
        else if(past_robot_vel.size()==1)
        {
            vel1=past_robot_vel.at(0);
            app_vx_ = pval*tmp.x_ + dval*(tmp.x_-vel1.Vx)/update_T;
            app_vy_ = pval*tmp.y_ + dval*(tmp.y_-vel1.Vy)/update_T;
            cout<<"past vel0: "<<vel1.Vx<<' '<<vel1.Vy<<endl;
        }
        else
        {
            vel1=past_robot_vel.at(0);
            vel2=past_robot_vel.at(1);
            app_vx_ = pval*tmp.x_ + dval*(tmp.x_-vel2.Vx*vel2.Vx+vel1.Vx)/(update_T*update_T);
            app_vy_ = pval*tmp.y_ + dval*(tmp.y_-vel2.Vy*vel2.Vy+vel1.Vy)/(update_T*update_T);
            cout<<"past vel0: "<<vel1.Vx<<' '<<vel1.Vy<<endl;
            cout<<"past vel1: "<<vel2.Vx<<' '<<vel2.Vy<<endl;
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
    ROS_INFO("Behaviour move2target");
    DPoint after_target;
    double time=target.norm()*2/maxvel;
    after_target.x_=realtarvel.x_*time, after_target.y_=realtarvel.y_*time;
    after_target+=target;
    move2Position(pval,dval,after_target,maxvel,_robot_pos,_robot_ori);
}

void traceTarget();
void revDecoupleFromVel(float vx,float vy,float &positivemax_rev,float &negativemax_rev);




/////////////////
// rotate to the target orientation by using PD control
void Behaviour::rotate2AbsOrienation(float pval, float dval, float orientation,float maxw,const Angle & _robot_ori)//自身坐标系
{
    ROS_INFO("Behaviour rotate2AbsOrienation");
    cout<<orientation<<endl;
    double tmp = orientation - _robot_ori.radian_;
    if(fabs(tmp) > 8.0/180.0)        // 容许误差为5度
    {
        if(past_robot_vel.size()==0)
            app_w_ = tmp;
        else if(past_robot_vel.size()==1)
        {
            vel1=past_robot_vel.at(0);
            app_w_ = pval*tmp + dval*(tmp-vel1.w)/update_T;
        }
        else
        {
            vel1=past_robot_vel.at(0);
            vel2=past_robot_vel.at(1);
            app_w_ = pval*tmp + dval*(tmp-vel2.w*vel2.w+vel1.w)/(update_T*update_T);
        }
    }
    else
    {
        app_w_ = 0;
    }
    ROS_INFO("app_w");
    cout<<app_w_<<endl;
    cout<<tmp<<endl;
    /*
    double temp;
    temp=orientation; temp-=_robot_ori.radian_;
    double delta_w;
    if(fabs(temp)>8.0/180.0)
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
    ROS_INFO("rotate relpoint");
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

void Behaviour::accelerateLimit(const double &_acc_thresh/* = 20*/, const bool & use_convected_acc/* = true*/)
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
void fuzzyPIDcontrol(float &deltakp, float &deltaki,float &deltakd, float err,float err1)
{
        float target=600;
        float actual=0;
        float u=0;
        int deltaKpMatrix[7][7]={{PB,PB,PM,PM,PS,ZO,ZO},
                                 {PB,PB,PM,PS,PS,ZO,NS},
                                 {PM,PM,PM,PS,ZO,NS,NS},
                                 {PM,PM,PS,ZO,NS,NM,NM},
                                 {PS,PS,ZO,NS,NS,NM,NM},
                                 {PS,ZO,NS,NM,NM,NM,NB},
                                 {ZO,ZO,NM,NM,NM,NB,NB}};
        int deltaKiMatrix[7][7]={{NB,NB,NM,NM,NS,ZO,ZO},
                                 {NB,NB,NM,NS,NS,ZO,ZO},
                                 {NB,NM,NS,NS,ZO,PS,PS},
                                 {NM,NM,NS,ZO,PS,PM,PM},
                                 {NM,NS,ZO,PS,PS,PM,PB},
                                 {ZO,ZO,PS,PS,PM,PB,PB},
                                 {ZO,ZO,PS,PM,PM,PB,PB}};
        int deltaKdMatrix[7][7]={{PS,NS,NB,NB,NB,NM,PS},
                                 {PS,NS,NB,NM,NM,NS,ZO},
                                 {ZO,NS,NM,NM,NS,NS,ZO},
                                 {ZO,NS,NS,NS,NS,NS,ZO},
                                 {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
                                 {PB,NS,PS,PS,PS,PS,PB},
                                 {PB,PM,PM,PM,PS,PS,PB}};
        float e_mf_paras[] ={NB,NB,NM,NB,NM,NS,NM,NS,ZO,NS,ZO,PS,ZO,PS,PM,PS,PM,PB,PM,PB,PB};
        float de_mf_paras[]={NB,NB,NM,NB,NM,NS,NM,NS,ZO,NS,ZO,PS,ZO,PS,PM,PS,PM,PB,PM,PB,PB};
        float Kp_mf_paras[]={NB,NB,NM,NB,NM,NS,NM,NS,ZO,NS,ZO,PS,ZO,PS,PM,PS,PM,PB,PM,PB,PB};
        float Ki_mf_paras[]={NB,NB,NM,NB,NM,NS,NM,NS,ZO,NS,ZO,PS,ZO,PS,PM,PS,PM,PB,PM,PB,PB};
        float Kd_mf_paras[]={NB,NB,NM,NB,NM,NS,NM,NS,ZO,NS,ZO,PS,ZO,PS,PM,PS,PM,PB,PM,PB,PB};
        FuzzyPID fuzzypid(1500,650,0.3,0.9,0.6,0.01,0.04,0.01);//float e_max,float de_max,float kp_max,float ki_max,float kd_max,float Kp0,float Ki0,float Kd0
        fuzzypid.setMf("trimf",e_mf_paras,"trimf",de_mf_paras,"trimf",Kp_mf_paras,"trimf",Ki_mf_paras,"trimf",Kd_mf_paras);
        fuzzypid.setRuleMatrix(deltaKpMatrix,deltaKiMatrix,deltaKdMatrix);
        cout<<"num target    actual"<<endl;
        for(int i=0;i<50;i++)
        {
            u=fuzzypid.realize(target,actual);
            actual+=u;
            ///cout<<i<<"   "<<target<<"    "<<actual<<endl;
        }
        ///fuzzypid.showInfo();
        ///system("pause");
}



FuzzyPID::FuzzyPID(float e_max,float de_max,float kp_max,float ki_max,float kd_max,float Kp0,float Ki0,float Kd0):
target(0),actual(0),emax(e_max),demax(de_max),delta_Kp_max(kp_max),delta_Ki_max(ki_max),delta_Kd_max(kd_max),e_mf_paras(NULL),de_mf_paras(NULL),
Kp_mf_paras(NULL),Ki_mf_paras(NULL),Kd_mf_paras(NULL)
{
   e=target-actual;
   e_pre_1=0;
   e_pre_2=0;
   de=e-e_pre_1;
   Ke=(N/2)/emax;
   Kde=(N/2)/demax;
   Ku_p=delta_Kp_max/(N/2);
   Ku_i=delta_Ki_max/(N/2);
   Ku_d=delta_Kd_max/(N/2);
   mf_t_e="No type";
   mf_t_de="No type";
   mf_t_Kp="No type";
   mf_t_Ki="No type";
   mf_t_Kd="No type";
   Kp=Kp0;
   Ki=Ki0;
   Kd=Kd0;
   A=Kp+Ki+Kd;
   B=-2*Kd-Kp;
   C=Kd;
}

FuzzyPID::FuzzyPID(float *fuzzyLimit,float *pidInitVal)
{
    target=0;
    actual=0;
    e=0;
    e_pre_1=0;
    e_pre_2=0;
    de=e-e_pre_1;
    emax=fuzzyLimit[0];
    demax=fuzzyLimit[1];
    delta_Kp_max=fuzzyLimit[2];
    delta_Ki_max=fuzzyLimit[3];
    delta_Kd_max=fuzzyLimit[4];
    Ke=(N/2)/emax;
    Kde=(N/2)/demax;
    Ku_p=delta_Kp_max/(N/2);
    Ku_i=delta_Ki_max/(N/2);
    Ku_d=delta_Kd_max/(N/2);
    mf_t_e="No type";
    mf_t_de="No type";
    mf_t_Kp="No type";
    mf_t_Ki="No type";
    mf_t_Kd="No type";
    e_mf_paras=NULL;
    de_mf_paras=NULL;
    Kp_mf_paras=NULL;
    Ki_mf_paras=NULL;
    Kd_mf_paras=NULL;

    Kp=pidInitVal[0];
    Ki=pidInitVal[1];
    Kd=pidInitVal[2];
    A=Kp+Ki+Kd;
    B=-2*Kd-Kp;
    C=Kd;
}

FuzzyPID::~FuzzyPID()
{
  delete [] e_mf_paras;
  delete [] de_mf_paras;
  delete [] Kp_mf_paras;
  delete [] Ki_mf_paras;
  delete [] Kd_mf_paras;
}
//三角隶属度函数
float FuzzyPID::trimf(float x,float a,float b,float c)
{
   float u;
   if(x>=a && x<=b)
       u=(x-a)/(b-a);
   else if(x>b && x<=c)
       u=(c-x)/(c-b);
   else
       u=0.0;
   return u;

}
//正态隶属度函数
float FuzzyPID::gaussmf(float x,float ave,float sigma)
{
    float u;
    if(sigma<0)
    {
       cout<<"In gaussmf, sigma must larger than 0"<<endl;
    }
    u=exp(-pow(((x-ave)/sigma),2));
    return u;
}
//梯形隶属度函数
float FuzzyPID::trapmf(float x,float a,float b,float c,float d)
{
    float u;
    if(x>=a && x<b)
        u=(x-a)/(b-a);
    else if(x>=b && x<c)
        u=1;
    else if(x>=c && x<=d)
        u=(d-x)/(d-c);
    else
        u=0;
    return u;
}
//设置模糊规则Matrix
void FuzzyPID::setRuleMatrix(int kp_m[N][N],int ki_m[N][N],int kd_m[N][N])
{
    for(int i=0; i<N; i++)
       for(int j=0; j<N; j++)
       {
           Kp_rule_matrix[i][j]=kp_m[i][j];
           Ki_rule_matrix[i][j]=ki_m[i][j];
           Kd_rule_matrix[i][j]=kd_m[i][j];
       }
}
//设置模糊隶属度函数的子函数(setMf中调用)
void FuzzyPID::setMf_sub(const string & type,float *paras,int n)//mf_t_e-0; mf_t_de-1; mf_t_Kp-2; mf_t_Ki-3; mf_t_Kd-4
{
    int N_mf_e,N_mf_de,N_mf_Kp,N_mf_Ki,N_mf_Kd;
    switch(n)
    {
    case 0:
        if(type=="trimf"||type=="gaussmf"||type=="trapmf")
            mf_t_e=type;
        else
            cout<<"Type of membership function must be \"trimf\" or \"gaussmf\" or \"trapmf\""<<endl;
        if(mf_t_e=="trimf")
            N_mf_e=3;
        else if(mf_t_e=="gaussmf")
            N_mf_e=2;
        else if(mf_t_e=="trapmf")
            N_mf_e=4;

        e_mf_paras=new float [N*N_mf_e];
        for(int i=0;i<N*N_mf_e;i++)
            e_mf_paras[i]=paras[i];
        break;

    case 1:
        if(type=="trimf"||type=="gaussmf"||type=="trapmf")
            mf_t_de=type;
        else
            cout<<"Type of membership function must be \"trimf\" or \"gaussmf\" or \"trapmf\""<<endl;
        if(mf_t_de=="trimf")
            N_mf_de=3;
        else if(mf_t_de=="gaussmf")
            N_mf_de=2;
        else if(mf_t_de=="trapmf")
            N_mf_de=4;
        de_mf_paras=new float [N*N_mf_de];
        for(int i=0;i<N*N_mf_de;i++)
            de_mf_paras[i]=paras[i];
        break;

    case 2:
        if(type=="trimf"||type=="gaussmf"||type=="trapmf")
            mf_t_Kp=type;
        else
            cout<<"Type of membership function must be \"trimf\" or \"gaussmf\" or \"trapmf\""<<endl;
        if(mf_t_Kp=="trimf")
            N_mf_Kp=3;
        else if(mf_t_Kp=="gaussmf")
            N_mf_Kp=2;
        else if(mf_t_Kp=="trapmf")
            N_mf_Kp=4;
        Kp_mf_paras=new float [N*N_mf_Kp];
        for(int i=0;i<N*N_mf_Kp;i++)
            Kp_mf_paras[i]=paras[i];
        break;

    case 3:
        if(type=="trimf"||type=="gaussmf"||type=="trapmf")
            mf_t_Ki=type;
        else
            cout<<"Type of membership function must be \"trimf\" or \"gaussmf\" or \"trapmf\""<<endl;
        if(mf_t_Ki=="trimf")
            N_mf_Ki=3;
        else if(mf_t_Ki=="gaussmf")
            N_mf_Ki=2;
        else if(mf_t_Ki=="trapmf")
            N_mf_Ki=4;
        Ki_mf_paras=new float [N*N_mf_Ki];
        for(int i=0;i<N*N_mf_Ki;i++)
            Ki_mf_paras[i]=paras[i];
        break;

    case 4:
        if(type=="trimf"||type=="gaussmf"||type=="trapmf")
            mf_t_Kd=type;
        else
            cout<<"Type of membership function must be \"trimf\" or \"gaussmf\" or \"trapmf\""<<endl;
        if(mf_t_Kd=="trimf")
            N_mf_Kd=3;
        else if(mf_t_Kd=="gaussmf")
            N_mf_Kd=2;
        else if(mf_t_Kd=="trapmf")
            N_mf_Kd=4;
        Kd_mf_paras=new float [N*N_mf_Kd];
        for(int i=0;i<N*N_mf_Kd;i++)
            Kd_mf_paras[i]=paras[i];
        break;

   default: break;
  }
}
//设置模糊隶属度函数的类型和参数
void FuzzyPID::setMf(const string & mf_type_e,float *e_mf,
            const string & mf_type_de,float *de_mf,
            const string & mf_type_Kp,float *Kp_mf,
            const string & mf_type_Ki,float *Ki_mf,
            const string & mf_type_Kd,float *Kd_mf)
{
    setMf_sub(mf_type_e,e_mf,0);
    setMf_sub(mf_type_de,de_mf,1);
    setMf_sub(mf_type_Kp,Kp_mf,2);
    setMf_sub(mf_type_Ki,Ki_mf,3);
    setMf_sub(mf_type_Kd,Kd_mf,4);
}
//实现模糊控制
float FuzzyPID::realize(float t,float a)
{
    float u_e[N],u_de[N],u_u[N];
    int u_e_index[3],u_de_index[3];//假设一个输入最多激活3个模糊子集
    float delta_Kp,delta_Ki,delta_Kd;
    float delta_u;
    target=t;
    actual=a;
    e=target-actual;
    de=e-e_pre_1;
    e=Ke*e;
    de=Kde*de;
  /* 将误差e模糊化*/
    int j=0;
    for(int i=0;i<N;i++)
    {
        if(mf_t_e=="trimf")
          u_e[i]=trimf(e,e_mf_paras[i*3],e_mf_paras[i*3+1],e_mf_paras[i*3+2]);//e模糊化，计算它的隶属度
        else if(mf_t_e=="gaussmf")
          u_e[i]=gaussmf(e,e_mf_paras[i*2],e_mf_paras[i*2+1]);//e模糊化，计算它的隶属度
        else if(mf_t_e=="trapmf")
          u_e[i]=trapmf(e,e_mf_paras[i*4],e_mf_paras[i*4+1],e_mf_paras[i*4+2],e_mf_paras[i*4+3]);//e模糊化，计算它的隶属度

        if(u_e[i]!=0)
            u_e_index[j++]=i;                //存储被激活的模糊子集的下标，可以减小计算量
    }
    for(;j<3;j++)u_e_index[j]=0;             //富余的空间填0

    /*将误差变化率de模糊化*/
    j=0;
    for(int i=0;i<N;i++)
    {
        if(mf_t_de=="trimf")
           u_de[i]=trimf(de,de_mf_paras[i*3],de_mf_paras[i*3+1],de_mf_paras[i*3+2]);//de模糊化，计算它的隶属度
        else if(mf_t_de=="gaussmf")
           u_de[i]=gaussmf(de,de_mf_paras[i*2],de_mf_paras[i*2+1]);//de模糊化，计算它的隶属度
        else if(mf_t_de=="trapmf")
           u_de[i]=trapmf(de,de_mf_paras[i*4],de_mf_paras[i*4+1],de_mf_paras[i*4+2],de_mf_paras[i*4+3]);//de模糊化，计算它的隶属度

        if(u_de[i]!=0)
            u_de_index[j++]=i;            //存储被激活的模糊子集的下标，可以减小计算量
    }
    for(;j<3;j++)u_de_index[j]=0;          //富余的空间填0

    float den=0,num=0;
    /*计算delta_Kp和Kp*/
    for(int m=0;m<3;m++)
        for(int n=0;n<3;n++)
        {
           num+=u_e[u_e_index[m]]*u_de[u_de_index[n]]*Kp_rule_matrix[u_e_index[m]][u_de_index[n]];
           den+=u_e[u_e_index[m]]*u_de[u_de_index[n]];
        }
    delta_Kp=num/den;
    delta_Kp=Ku_p*delta_Kp;
    if(delta_Kp>=delta_Kp_max)   delta_Kp=delta_Kp_max;
    else if(delta_Kp<=-delta_Kp_max) delta_Kp=-delta_Kp_max;
    Kp+=delta_Kp;
    if(Kp<0)Kp=0;
    /*计算delta_Ki和Ki*/
    den=0;num=0;
    for(int m=0;m<3;m++)
        for(int n=0;n<3;n++)
        {
           num+=u_e[u_e_index[m]]*u_de[u_de_index[n]]*Ki_rule_matrix[u_e_index[m]][u_de_index[n]];
           den+=u_e[u_e_index[m]]*u_de[u_de_index[n]];
        }

    delta_Ki=num/den;
    delta_Ki=Ku_i*delta_Ki;
    if(delta_Ki>=delta_Ki_max)   delta_Ki=delta_Ki_max;
    else if(delta_Ki<=-delta_Ki_max)  delta_Ki=-delta_Ki_max;
    Ki+=delta_Ki;
    if(Ki<0)Ki=0;
    /*计算delta_Kd和Kd*/
    den=0;num=0;
    for(int m=0;m<3;m++)
        for(int n=0;n<3;n++)
        {
           num+=u_e[u_e_index[m]]*u_de[u_de_index[n]]*Kd_rule_matrix[u_e_index[m]][u_de_index[n]];
           den+=u_e[u_e_index[m]]*u_de[u_de_index[n]];
        }
    delta_Kd=num/den;
    delta_Kd=Ku_d*delta_Kd;
    if(delta_Kd>=delta_Kd_max)   delta_Kd=delta_Kd_max;
    else if(delta_Kd<=-delta_Kd_max) delta_Kd=-delta_Kd_max;
    Kd+=delta_Kd;
    if(Kd<0)Kd=0;

    A=Kp+Ki+Kd;
    B=-2*Kd-Kp;
    C=Kd;
    delta_u=A*e+B*e_pre_1+C*e_pre_2;

    delta_u=delta_u/Ke;

    if(delta_u>=0.95*target)delta_u=0.95*target;
    else if(delta_u<=-0.95*target)delta_u=-0.95*target;

    e_pre_2=e_pre_1;
    e_pre_1=e;

    return delta_u;
}
