#include "nubot/nubot_control/bezier.h"
#include "nubot/core/core.hpp"
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
Bezier(double s,boost::ptr_list<DPoint> &controlpoint,double &p ,double &q, double &diffp,double &diffq,double &double_diffp, double &double_diffq);
Bezier(double s,boost::ptr_list<DPoint> &controlpoint,double &p ,double &q, double &diffp,double &diffq);

double CalculatediffS(double &s, double &rho,double &phid,double v0,   double c, double alpha,double p,double q,double diffp,double diffq);
double CalculatediffS(double &s, double &rho,double &phid,double lamda,double v0,             double p,double q,double diffp,double diffq);
double CalculatePhid(double rho, double basic_phid, double thetar, double eps);
double Curvature(double diffp, double diffq, double double_diffp, double double_diffq)//曲率半径rho=1/曲率
{//p=x, q=y
    //K=fabs(double_diffq)/pow(1+diffq*diffq,3/2);
    //rho=1/K;
}
double Bernstein(double s , int i , int N);//Bernstein条件  判断多个进程能否并行

bool BezierPathFollow(boost::ptr_list<DPoint> &controlpoint,double v0,double c , double alpha ,double k, double lamda);//路径跟踪
void FromPath2Trajectory(double &s,double &rtheta ,double vr,double wr,double diffp,double diffq,double double_diffp,double double_diffq,double vprofile ,double aprofile)//从路径到轨迹
{

}
bool BezierTrajectoryTracking(boost::ptr_list<DPoint> &controlpoint,double v0,double a)//轨迹跟踪
{//判断速度和加速度能否满足
    DPoint now_point, next_point;
    Robot now_robot=World_Model_Info.RobotInfo_.at(World_Model_Info::AgentID_);
    now_point=now_robot.getLocation();
    if(controlpoint.empty())
        return true;
    next_point=controlpoint.at(0);
    controlpoint.erase(controlpoint.begin());
    int time_lock=PassState.time_lock_;
    double distance_next=now_point.distance(next_point);
    double vx=distance_next*1000.0/time_lock;
    if(vx<=5/*速度最大值*/ && vx*1.0/v0<=3/*加速度最大值*/)///!
    {
        return true;
    }
    return false;
}


//采用随机数生成8个随机点，对第一组随机数进行排序，第二组随机数不排序，填充到QTableWidget
//获取数据点，根据数据点反求控制点。
void get_controlpoint(DPoint *Point_online, int n)
{
      /*
        此段程序将数据点反求出控制点,八个数据点
      */
      double l[n-1];  //存放数据点之间的距离，l[0]代表第一个数据点到第二个数据点之间的距离
      double L=0;//存放总长度
      double u[n+6];//节点矢量
      for(int i=0;i<n-1;i++)
      {
          l[i]=Point_online[i].distance(Point_online[i+1]);//两点之间距离
          L=L+l[i];
      }
      //节点矢量计算
      for(int i=0;i<4;i++)
      {
          u[i]=0;
      }
      double sum_l=0;
      for(int i=4; i<=n+1; i++)
      {
          sum_l+=l[i-4];
          u[i]=sum/L;
      }
      for(int i=n+2;i<n+6;i++)
      {
          u[i]=1;
      }

      double detal[n+5];
      for(int i=0;i<n+5;i++)
      {
          detal[i]=u[i+1]-u[i];
      }

      //根据节点矢量，反算控制点，其中需要使用矩阵求逆
      double a[n],b[n],c[n],e[n],f[n];  //角标0-8
      a[0]=1-(detal[3]*detal[4])/pow((detal[3]+detal[4]),2);
      b[0]=(detal[3]/(detal[3]+detal[4]))*(detal[4]/(detal[3]+detal[4])-detal[3]/(detal[3]+detal[4]+detal[5]));
      c[0]=(pow((detal[3]),2))/((detal[3]+detal[4])*(detal[3]+detal[4]+detal[5]));
      e[0]=(1.0/3)*(Point_online[0].x_+2*Point_online[1].x_);  //X轴坐标
      f[0]=(1.0/3)*(Point_online[0].y_+2*Point_online[1].y_);  //Y轴坐标


      a[n-1]=-(pow((detal[n+1]),2))/((detal[n]+detal[n+1])*(detal[n]+detal[n]+detal[n+1]));
      b[n-1]=(detal[n+1]/(detal[n]+detal[n+1]))*(detal[n+1]/(detal[n]+detal[n]+detal[n+1])-detal[n]/(detal[n]+detal[n+1]));
      c[n-1]=detal[n]*detal[n+1]/pow((detal[n]+detal[n+1]),2)-1;
      e[n-1]=-(1.0/3)*(Point_online[n-1].x_+2*Point_online[n-2].x_);
      f[n-1]=-(1.0/3)*(Point_online[n-1].y_+2*Point_online[n-2].y_);


      for(int i=1;i<n-1;i++)
      {
          a[i]=(pow((detal[i+3]),2))/(detal[i+1]+detal[i+2]+detal[i+3]);
          b[i]=detal[i+3]*(detal[i+1]+detal[i+2])/(detal[i+1]+detal[i+2]+detal[i+3])+detal[i+2]*(detal[i+3]+detal[i+4])/(detal[i+2]+detal[i+3]+detal[i+4]);
          c[i]=pow((detal[i+2]),2)/(detal[i+2]+detal[i+3]+detal[i+4]);
          e[i]=(detal[i+2]+detal[i+3])*Point_online[i].x_;
          f[i]=(detal[i+2]+detal[i+3])*Point_online[i].y_;
      }

      double matrix[n][n];
      for(int i=0;i<n;i++)
      {
          for(int j=0;j<n;j++)
          {
              matrix[i][j]=0;
          }
      }

      /*对系数矩阵赋值*/
      matrix[0][0]=a[0];
      matrix[0][1]=b[0];
      matrix[0][2]=c[0];

      for(int i=1;i<n-1;i++)
      {
          matrix[i][i-1]=a[i];
          matrix[i][i]=b[i];
          matrix[i][i+1]=c[i];
      }

       matrix[n-1][n-3]=a[n-1];
       matrix[n-1][n-2]=b[n-1];
       matrix[n-1][n-1]=c[n-1];


       double matrix_after[n][n];

       matrix_inv(matrix,matrix_after);  //求逆矩阵
       ///common.hpp中有4*4矩阵求逆

       double sum_x[n]={},sum_y[n]={};

       DPoint ctrlPoints[n+2];//控制点
       DPoint temp_point;
       ctrlPoints[0]=Point_online[0];
       for(int i=0;i<n;i++)
       {
           for(int j=0;j<n;j++)
           {
               sum_x[i]=sum_x[i]+matrix_after[i][j]*e[j];
               sum_y[i]=sum_y[i]+matrix_after[i][j]*f[j];
           }
           temp_point.x_=sum_x[i], temp_point.y_=sum_y[i];
           ctrlPoints[i+1]=temp_point;
       }
       ctrlPoints[n+1]=Point_online[n-1];
}


//矩阵求逆
void matrix_inv(double matrix_before[N][N],double matrix_after[N][N], int n)
{
    bool flag;//标志位，如果行列式为0，则结束程序
    flag=GetMatrixInverse(matrix_before,n,matrix_after);
}

//按第一行展开计算|A|
double getA(double arcs[N][N],int n)
{
    if(n==1)
    {
        return arcs[0][0];
    }
    double ans = 0;
    double temp[n][n]={0.0};
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
void getAStart(double arcs[N][N],int n,double ans[N][N])
{
    if(n==1)
    {
        ans[0][0] = 1;
        return;
    }
    int i,j,k,t;
    double temp[n][n];
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
bool GetMatrixInverse(double src[N][N],int n,double des[N][N])
{
    double flag=getA(src,n);
    double t[n][n];
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
/*
//根据控制点绘制B样条曲线：
void generateCurve()
{
    curvePoints.clear();
    int i=3;
    for(double u1=0; u1<1; u1+=0.001)
    {
        QPointF tmp(0,0);
        if(u1>u[i+1])
        {
            i=i+1;
        }
        for(int k=0; k<4;k++)
        {
            QPointF t = ctrlPoints[i-k];
            t*=Nu(i-k,u1,3);
            tmp+=t;
        }
        curvePoints.push_back(tmp);
    }
}

double Nu(int i,double u2,int k)
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
        double nu=(a1/c1)*Nu(i,u2,k-1)+(b1/d1)*Nu(i+1,u2,k-1);
        return nu;
    }
}
*/
