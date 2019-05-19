#ifndef SUBTARGETS_H
#define SUBTARGETS_H

#include <cmath>
#include <vector>
#include <algorithm>
#include "nubot/core/core.hpp"
#include "nubot/nubot_control/world_model_info.h"
#include "nubot/nubot_control/mydefine.hpp"

#include <ros/ros.h>

using namespace std;

namespace nubot{
    class Subtargets
    {
    public:
    Subtargets();
    std::vector<nubot::DPoint> first_point, second_point;
    int dcmp(double x)
    {
        if(fabs(x)<eps)
            return 0;
        return x < 0? -1: 1;
    }
    struct circle
    {
        double x,y,r;
        circle(double x=0.0,double y=0.0,double r=0.0):x(x),y(y),r(r){}
        DPoint point(double a)
        {
            return DPoint( x+r*cos(a),y+r*sin(a));
        }
    };
    int get_tangents(circle A, circle B, DPoint *a, DPoint *b)
    {
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
    static bool cmp(pair<DPoint,DPoint> a, pair<DPoint,DPoint> b)
    {
        bool flag=0;
        if(fabs(a.first.x_<b.first.x_)>eps && a.first.x_<b.first.x_) flag=1;//<
        else if(fabs(a.first.x_<b.first.x_)<eps && a.first.y_<b.first.y_) flag=1;//<
        if(flag) return true;
        else if(a.first==b.first)
        {
            flag=0;
            if(fabs(a.second.x_<b.second.x_)>eps && a.second.x_<b.second.x_) flag=1;//<
            else if(fabs(a.second.x_<b.second.x_)<eps && a.second.y_<b.second.y_) flag=1;//<
            if(flag)
                return true;
        }
        return false;
    }
    int getpoint(double x1, double y1, double r1, double x2, double y2, double r2)
    {
        circle A(x1, y1, r1),B(x2, y2, r2);
        DPoint a[10],b[10];
        int ans = get_tangents(A,B,a,b);
        if( ans==-1 || ans ==0 )
        {
            return ans;
        }
        vector< pair<DPoint,DPoint> > group;
        for( int i = 0; i < ans;++i)
            group.push_back( make_pair(a[i],b[i]));
        sort(group.begin(),group.end(),cmp);
        first_point.clear();
        second_point.clear();
        //DPoint qaq, qwq;
        for( vector< pair<DPoint,DPoint> > ::iterator i = group.begin();i!=group.end();i++ )
        {
            first_point.push_back(i->first);
            second_point.push_back(i->second);
        }
        return ans;
    }



    int Min_num(int n,double *q);
    double Min(int n,double *q);
    int Max_num(int n,double *q);
    double Max(int n,double *q);
    void subtarget(DPoint target, DPoint robot_pos_, bool avoid_ball);
    
public:

        World_Model_Info * world_model_;
        DPoint subtargets_pos_;
        DPoint robot_pos_;
        DPoint ball_pos_;
};
}
#endif //SUBTARGETS_H	 

