#include "nubot/nubot_control/bezier.h"
#include "nubot/core/core.hpp"
#include "nubot/nubot_control/mydefine.hpp"
using namespace  nubot;
/*
public:
public:
        Behaviour * behaviour_;
        World_Model_Info * world_model_;

        double bezier_s;
        bool   bezier_updateflag;
        double bezier_vr ,bezier_wr ;
        double bezier_vm ;
        double bezier_p ,bezier_q ,bezier_rtheta ;
        double last_bezier_rtheta;

        boost::ptr_list<DPoint> Active_ControlPointList;//主动控制点?
        boost::ptr_list<DPoint> Passive_ControlPointList;//被动控制点?
        */
Bezier::Bezier(double s,boost::ptr_list<DPoint> &controlpoint,double &p ,double &q, double &diffp,double &diffq,double &double_diffp, double &double_diffq)
{
    ////已用
}
Bezier::Bezier(double s,boost::ptr_list<DPoint> &controlpoint,double &p ,double &q, double &diffp,double &diffq)
{
    ////已用
}

double CalculatediffS(double &s, double &rho,double &phid,double v0,   double c, double alpha,double p,double q,double diffp,double diffq);
double CalculatediffS(double &s, double &rho,double &phid,double lamda,double v0,             double p,double q,double diffp,double diffq);
double CalculatePhid(double rho, double basic_phid, double thetar, double eps);
double Bezier::Curvature(double diffp, double diffq, double double_diffp, double double_diffq)//曲率半径rho=1/曲率
{//p=x, q=y
    //K=fabs(double_diffq)/pow(1+diffq*diffq,3/2);
    //rho=1/K;
}
double Bernstein(double s , int i , int N);//Bernstein条件  判断多个进程能否并行

bool BezierPathFollow(boost::ptr_list<DPoint> &controlpoint,double v0,double c , double alpha ,double k, double lamda);//路径跟踪
void Bezier::FromPath2Trajectory(double &s,double &rtheta ,double vr,double wr,double diffp,double diffq,double double_diffp,double double_diffq,double vprofile ,double aprofile)//从路径到轨迹
{

}
bool Bezier::BezierTrajectoryTracking(boost::ptr_list<DPoint> &controlpoint,double v0,double a)//轨迹跟踪
{//判断速度和加速度能否满足
    DPoint now_point, next_point;
    now_point = world_model_->RobotInfo_[world_model_->AgentID_-1].getLocation();
    if(controlpoint.empty())
        return true;
    next_point=controlpoint.front();
    controlpoint.erase(controlpoint.begin());
    double distance_next=now_point.distance(next_point);
    double vx=distance_next*1000.0/update_T;
    if(vx<=max_vel && (vx-v0)*1.0/update_T<=max_acc)
    {
        return true;
    }
    return false;
}


//采用随机数生成8个随机点，对第一组随机数进行排序，第二组随机数不排序，填充到QTableWidget
//获取数据点，根据数据点反求控制点。
void Bezier::get_controlpoint(DPoint *Point_online)
{
    /*此段程序将数据点反求出控制点,八个数据点*/
    double l[7];  //存放数据点之间的距离，l[0]代表第一个数据点到第二个数据点之间的距离
    double L=0;//存放总长度
    double u[14];//节点矢量
    for(int i=0;i<7;i++)
    {
        l[i]=Point_online[i].distance(Point_online[i+1]);//两点之间距离
        L=L+l[i];
    }
    //节点矢量计算
    for(int i=0;i<4;i++) u[i]=0;
    for(int i=0;i<4;i++) u[i+10]=1;
    u[4]=l[0]/L;
    u[5]=(l[0]+l[1])/L;
    u[6]=(l[0]+l[1]+l[2])/L;
    u[7]=(l[0]+l[1]+l[2]+l[3])/L;
    u[8]=(l[0]+l[1]+l[2]+l[3]+l[4])/L;
    u[9]=(l[0]+l[1]+l[2]+l[3]+l[4]+l[5])/L;

    double detal[13];
    for(int i=0;i<13;i++)
    {
        detal[i]=u[i+1]-u[i];
    }

    //根据节点矢量，反算控制点，其中需要使用矩阵求逆
    double a[8],b[8],c[8],e[8],f[8];  //角标0-8
    a[0]=1-(detal[3]*detal[4])/pow((detal[3]+detal[4]),2);
    b[0]=(detal[3]/(detal[3]+detal[4]))*(detal[4]/(detal[3]+detal[4])-detal[3]/(detal[3]+detal[4]+detal[5]));
    c[0]=(pow((detal[3]),2))/((detal[3]+detal[4])*(detal[3]+detal[4]+detal[5]));
    e[0]=(1.0/3)*(Point_online[0].x_+2*Point_online[1].x_);  //X轴坐标
    f[0]=(1.0/3)*(Point_online[0].y_+2*Point_online[1].y_);  //Y轴坐标


    a[7]=-(pow((detal[9]),2))/((detal[8]+detal[9])*(detal[8]+detal[8]+detal[9]));
    b[7]=(detal[9]/(detal[8]+detal[9]))*(detal[9]/(detal[8]+detal[8]+detal[9])-detal[8]/(detal[8]+detal[9]));
    c[7]=detal[8]*detal[9]/pow((detal[8]+detal[9]),2)-1;
    e[7]=-(1.0/3)*(Point_online[7].x_+2*Point_online[6].x_);
    f[7]=-(1.0/3)*(Point_online[7].y_+2*Point_online[6].y_);


    for(int i=1;i<7;i++)
    {
        a[i]=(pow((detal[i+3]),2))/(detal[i+1]+detal[i+2]+detal[i+3]);
        b[i]=detal[i+3]*(detal[i+1]+detal[i+2])/(detal[i+1]+detal[i+2]+detal[i+3])+detal[i+2]*(detal[i+3]+detal[i+4])/(detal[i+2]+detal[i+3]+detal[i+4]);
        c[i]=pow((detal[i+2]),2)/(detal[i+2]+detal[i+3]+detal[i+4]);
        e[i]=(detal[i+2]+detal[i+3])*Point_online[i].x_;
        f[i]=(detal[i+2]+detal[i+3])*Point_online[i].y_;
    }

    double matrix[8][8];
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            matrix[i][j]=0;
        }
    }

    /*对系数矩阵赋值*/
    matrix[0][0]=a[0];
    matrix[0][1]=b[0];
    matrix[0][2]=c[0];

    for(int i=1;i<7;i++)
    {
        matrix[i][i-1]=a[i];
        matrix[i][i]=b[i];
        matrix[i][i+1]=c[i];
    }

    matrix[7][5]=a[7];
    matrix[7][6]=b[7];
    matrix[7][7]=c[7];


    double matrix_after[NN][NN];
    matrix_inv(matrix,matrix_after,NN);  //求逆矩阵

    double sum_x[8]={},sum_y[8]={};

    DPoint ctrlPoints[10];//控制点
    DPoint temp_point;
    ctrlPoints[0]=Point_online[0];
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            sum_x[i]=sum_x[i]+matrix_after[i][j]*e[j];
            sum_y[i]=sum_y[i]+matrix_after[i][j]*f[j];
        }
        temp_point.x_=sum_x[i], temp_point.y_=sum_y[i];
        ctrlPoints[i+1]=temp_point;
    }
    ctrlPoints[9]=Point_online[7];
    generateCurve(ctrlPoints, u);
}


//矩阵求逆
void Bezier::matrix_inv(double matrix_before[NN][NN],double matrix_after[NN][NN], int n)
{
    bool flag;//标志位，如果行列式为0，则结束程序
    flag=GetMatrixInverse(matrix_before,n,matrix_after);
}

//按第一行展开计算|A|
double Bezier::getA(double arcs[NN][NN],int n)
{
    if(n==1)
    {
        return arcs[0][0];
    }
    double ans = 0;
    double temp[NN][NN]={0.0};
    int i,j,k;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n-1;j++)
        {
            for(k=0;k<n-1;k++)
            {
                temp[j][k] = arcs[j+1][(k>=i)?k+1:k];

            }
        }
        double t = getA(temp,n-1);
        if(i%2==0)
        {
            ans += arcs[0][i]*t;
        }
        else
        {
            ans -=  arcs[0][i]*t;
        }
    }
    return ans;
}

//计算每一行每一列的每个元素所对应的余子式，组成A*
void Bezier::getAStart(double arcs[NN][NN],int n,double ans[NN][NN])
{
    if(n==1)
    {
        ans[0][0] = 1;
        return;
    }
    int i,j,k,t;
    double temp[NN][NN];
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            for(k=0;k<n-1;k++)
            {
                for(t=0;t<n-1;t++)
                {
                    temp[k][t] = arcs[k>=i?k+1:k][t>=j?t+1:t];
                }
            }

            ans[j][i]  =  getA(temp,n-1);  //此处顺便进行了转置
            if((i+j)%2 == 1)
            {
                ans[j][i] = - ans[j][i];
            }
        }
    }
}

//得到给定矩阵src的逆矩阵保存到des中。
bool Bezier::GetMatrixInverse(double src[NN][NN],int n,double des[NN][NN])
{
    double flag=getA(src,n);
    double t[NN][NN];
    if(0==flag)
    {
        return false;//如果算出矩阵的行列式为0，则不往下进行
    }
    else
    {
        getAStart(src,n,t);
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                des[i][j]=t[i][j]/flag;
            }
        }
    }
    return true;
}

//根据控制点绘制B样条曲线：
void Bezier::generateCurve(DPoint *ctrlPoints, double *u)
{
    int i=3;
    DPoint t;
    for(double u1=0; u1<1; u1+=0.001)///
    {
        //DPoint tmp(0,0);
        if(u1>u[i+1])
        {
            i=i+1;
        }
        for(int k=0; k<4;k++)
        {
            t = ctrlPoints[i-k];
            t*=Nu(i-k,u1,3,u);
            //tmp+=t;
        }
        behaviour_->move2Position(0.5,0.5,t,max_vel,DPoint(0,0),Angle(0));///仅考虑自身，且未使用模糊PID控制修改pval和dval
        //curvePoints.push_back(tmp);
    }
}

double Bezier::Nu(int i,double u2,int k,double *u)
{
    if(k==0)
    {
        if((u2>=u[i])&&(u2<u[i+1]))
            return 1;
        else
            return 0;
    }
    else
    {
        double a1=(u2-u[i]),c1=(u[i+k]-u[i]);
        double b1=(u[i+k+1]-u2),d1=(u[i+k+1]-u[i+1]);
        if(d1<0.001)
        {
            b1=0;
            d1=1;
        }
        if(c1<0.001)
        {
            a1=0;
            c1=1;
        }
        double nu=(a1/c1)*Nu(i,u2,k-1,u)+(b1/d1)*Nu(i+1,u2,k-1,u);
        return nu;
    }
}
