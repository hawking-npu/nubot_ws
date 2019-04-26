#ifndef SUBTARGETS_H
#define SUBTARGETS_H

#include <cmath>
#include <vector>
#include <algorithm>
#include "nubot/core/core.hpp"
#include "nubot/nubot_control/world_model_info.h"

using namespace std;
namespace nubot{
class Subtargets
{
public:
        Subtargets();
#define FIR first
#define SEC second
using namespace std;
const double eps = 1e-8;
const double PI = acos(-1.0);
std::vector<nubot::DPoint> first_point, second_point;
int dcmp(double x)
{
    if(fabs(x)<eps)
        return 0;
    return x < 0? -1: 1;
}
struct pnode
{
    double x,y;
    pnode(double x=0.0,double y=0.0):x(x),y(y){}
    pnode operator - (const pnode &b)const{ return pnode(x-b.x,y-b.y);}
    pnode operator + (const pnode &b)const{ return pnode(x+b.x,y+b.y);}
    bool operator < (const pnode &b)const
    {
        return dcmp(x - b.x)<0 || ( dcmp(x-b.x)==0 && dcmp(y-b.y)<0);
    }
};
struct circle
{
    double x,y,r;
    circle(double x=0.0,double y=0.0,double r=0.0):x(x),y(y),r(r){}
    pnode point(double a)
    {
        return pnode( x+r*cos(a),y+r*sin(a));
    }
};
int get_tangents(circle A, circle B, pnode *a, pnode *b){
    int cnt = 0;        //存切点用
    if(dcmp(A.r - B.r) < 0)
    {
        swap(A, B);
        swap(a, b);
    }
    double d = sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));     //圆心距
    double rdiff = A.r - B.r;      //两圆半径差
    double rsum  = A.r + B.r;       //两圆半径和
    if(dcmp(d - rdiff) < 0)
        return 0;        //1.内含
    double base = atan2(B.y - A.y, B.x - A.x);      //向量AB的极角
    if(dcmp(d) == 0) return -1;        //2.重合
    if(dcmp(d - rdiff) == 0)
    {       //3.内切
        a[cnt] = b[cnt] = A.point(base);
        cnt++;
        return 1;
    }
    double ang = acos((A.r - B.r) / d);
    a[cnt] = A.point(base + ang); b[cnt] = B.point(base + ang); cnt++;      //4.相交（外切、外离的外公切线也在此求出）
    a[cnt] = A.point(base - ang); b[cnt] = B.point(base - ang); cnt++;      //两条外公切线的切点
    if(dcmp(d - rsum) == 0)
    {        //5.外切
        a[cnt] = b[cnt] = A.point(base);
        cnt++;
    }
    else if(dcmp(d - rsum) > 0)
    {      //6.外离
        double ang = acos((A.r + B.r) / d);
        a[cnt] = A.point(base + ang); b[cnt] = B.point(PI + base + ang); cnt++;
        a[cnt] = A.point(base - ang); b[cnt] = B.point(PI + base - ang); cnt++;
    }
    return cnt;
}
typedef pnode myvec;
double length( myvec a)
{
    return sqrt( a.x * a.x + a.y * a.y );
}
int getpoint(double x1, double y1, double r1, double x2, double y2, double r2)
{
    circle A,B;
    pnode a[10],b[10];
    A.x=x1, A.y=y1, A.r=r1;
    B.x=x2, B.y=y2, B.r=r2;
    int ans = get_tangents(A,B,a,b);
    if( ans==-1 || ans ==0 )
    {
        return ans;
    }
    vector<pair<pnode,pnode> > group;
    for( int i = 0; i < ans;++i)
        group.push_back( make_pair(a[i],b[i]));
    sort( group.begin(),group.end() );
    first_point.clear();
    second_point.clear();
    DPoint qaq, qwq;
    for( vector<pair<pnode,pnode> > ::iterator i = group.begin();i!=group.end();i++ )
    {
          qaq.x_=i->FIR.x, qaq.y_=i->FIR.y;
          qwq.x_=i->SEC.x, qwq.y_=i->SEC.y;
          first_point.push_back(qaq);
          second_point.push_back(qwq);
    }
    return ans;
}



	int
        Min_num(int n,double *q);
        double
        Min(int n,double *q);
        int
        Max_num(int n,double *q);
        double
        Max(int n,double *q);
        void
        subtarget(DPoint target, DPoint robot_pos_, bool avoid_ball);
    
public:

        World_Model_Info * world_model_;
        DPoint subtargets_pos_;
        DPoint robot_pos_;
        DPoint ball_pos_;
};
}
#endif //SUBTARGETS_H	 

