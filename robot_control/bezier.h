#ifndef BEZIER_H
#define BEZIER_H

#include <cmath>
#include "nubot/core/core.hpp"
#include "nubot/nubot_control/behaviour.hpp"
#include "nubot/nubot_control/world_model_info.h"
#include <boost/ptr_container/ptr_list.hpp>

using namespace std;
namespace nubot{
class Bezier
{
public:
        Bezier(double s,boost::ptr_list<DPoint> &controlpoint,double &p ,double &q, double &diffp,double &diffq,double &double_diffp, double &double_diffq);
        Bezier(double s,boost::ptr_list<DPoint> &controlpoint,double &p ,double &q, double &diffp,double &diffq);

        double CalculatediffS(double &s,double &rho,double &phid,double v0,double c,double alpha,double p,double q,double diffp,double diffq);
        double CalculatediffS(double &s, double &rho,double &phid,double lamda,double v0,double p,double q,double  diffp,double diffq);
        double CalculatePhid(double rho, double basic_phid, double thetar, double eps);
        double Curvature(double diffp, double diffq, double double_diffp, double double_diffq);//曲率
        double Bernstein(double s , int i , int N);

        bool BezierPathFollow(boost::ptr_list<DPoint> &controlpoint,double v0,double c , double alpha ,double k, double lamda);//路径跟踪
        void FromPath2Trajectory(double &s,double &rtheta ,double vr,double wr,double diffp,double diffq,double double_diffp,double double_diffq,double vprofile ,double aprofile);//从路径到轨迹
        bool BezierTrajectoryTracking(boost::ptr_list<DPoint> &controlpoint,double v0,double a);//轨迹跟踪

        //获取数据点，根据数据点反求控制点。
        void get_controlpoint();
        void matrix_inv(double matrix_before[N][N],double matrix_after[N][N]);//矩阵求逆
        double getA(double arcs[N][N],int n);//按第一行展开计算|A|
        void getAStart(double arcs[N][N],int n,double ans[N][N]);//计算每一行每一列的每个元素所对应的余子式，组成A*
        bool GetMatrixInverse(double src[N][N],int n,double des[N][N]);//得到给定矩阵src的逆矩阵保存到des中


public:
        Behaviour * behaviour_;
        World_Model_Info * world_model_;

        double bezier_s;
        bool   bezier_updateflag;
        double bezier_vr ,bezier_wr ;
        double bezier_vm ;
        double bezier_p ,bezier_q ,bezier_rtheta ;
        double last_bezier_rtheta;

        boost::ptr_list<DPoint> Active_ControlPointList;//主动控制点
        boost::ptr_list<DPoint> Passive_ControlPointList;//被动控制点

};
}
#endif //BEZIER_H
