#include "nubot/nubot_control/subtargets.h"
#include "nubot/core/core.hpp"
#include "nubot/nubot_control/mydefine.hpp"
using namespace nubot;

Subtargets::Subtargets()
{
    subtargets_pos_ = DPoint(0,0);
    robot_pos_ = DPoint(0,0);
    ball_pos_ = DPoint(0,0);
}

int Subtargets::Min_num(int n,double *q)
{
    double minn = INF;
    int temp = INF;
    for(int i=0; i<n; ++i)
        if(q[i] < minn)
        {
            minn = q[i];
            temp = i;
        }
    return temp;
}
double Subtargets::Min(int n,double *q)
{
    double minn = INF;
    for(int i=0; i<n; ++i)
        if(q[i] < minn)
            minn = q[i];
    return minn;
}
int Subtargets::Max_num(int n,double *q)
{
    double maxn = -INF;
    int temp = INF;
    for(int i=0; i<n; ++i)
        if(q[i] > maxn)
        {
            maxn = q[i];
            temp = i;
        }
    return temp;
}
double Subtargets::Max(int n,double *q)
{
    double maxn = -INF;
    for(int i=0; i<n; ++i)
        if(q[i] > maxn)
            maxn = q[i];
    return maxn;
}
void Subtargets::subtarget(DPoint target, DPoint robot_pos_, bool avoid_ball)
{
    /*生成子目标点，如果与目标点重合，则结束
    //否则将机器人的坐标替换为生成的子目标点的坐标&&重新计算
    //std::vector<nubot::Robot>      RobotInfo_; //! 机器人信息
    //std::vector<nubot::BallObject> BallInfo_;  //!
    //std::vector<nubot::DPoint>     Obstacles_; //!障碍物
    //std::vector<nubot::DPoint>     Opponents_; //!多个机器人障碍物融合信息

World_Model_Info * world_model_;
DPoint subtargets_pos_;
DPoint robot_pos_;
DPoint ball_pos_;
    //此处为世界坐标系*/

    BallObject ballinfo = world_model_->BallInfo_[world_model_->AgentID_-1];
    ball_pos_ = ballinfo.getGlobalLocation();

    //坐标转换
    //robot_pos_

    DPoint tran_ob;
    std::vector<nubot::DPoint> myObstacles_;//自身坐标下，筛选保留x>0的点
    std::vector<nubot::DPoint> _myObstacles;
    _myObstacles = world_model_->Obstacles_;
    if(avoid_ball)
        _myObstacles.push_back(ball_pos_);
    //ROS_INFO("Subtargets 1myObstacles_num: %ld", _myObstacles.size());
    DPoint ori;
    double theta_t;
    DPoint tar_now = target;
    tar_now -= robot_pos_;
    if(tar_now.x_ == 0)
    {
        if(tar_now.y_>0) { theta_t = PI/2; }
        else if(tar_now.y_<0) { theta_t = -PI/2; }
        else { theta_t = 0; }
    }
    else { theta_t = atan(tar_now.y_/tar_now.x_); }
    theta_t = -theta_t;
    ori = tar_now;
    tar_now.x_ = ori.x_*cos(theta_t)-ori.y_*sin(theta_t);
    tar_now.y_ = ori.x_*sin(theta_t)+ori.y_*cos(theta_t);

    myObstacles_.clear();
    for(int i=0; i<_myObstacles.size(); ++i)
    {
        tran_ob = _myObstacles.at(i);
        tran_ob -= robot_pos_;

        ori = tran_ob;

        tran_ob.x_ = ori.x_*cos(theta_t)-ori.y_*sin(theta_t);
        tran_ob.y_ = ori.x_*sin(theta_t)+ori.y_*cos(theta_t);
        if(tran_ob.x_ >= 0)
            myObstacles_.push_back(tran_ob);
    }

    //确定第一障碍物,建立分组
    std::vector <int> setB, setG;
    setB.clear(), setG.clear();


    //ROS_INFO("Subtargets 2myObstacles_num: %ld", myObstacles_.size());

    //确定集合B：机器人沿直线运动到目标点会撞到的障碍物
    DPoint now_ob;
    for(int i = 0; i < myObstacles_.size(); ++i)
    {
        now_ob = myObstacles_.at(i);
        if(now_ob.x_ > 0 && now_ob.x_ < target.distance(now_ob))
        {
            if(abs(now_ob.y_) < obs_radius+my_radius)//障碍物i和机器人圆的半径
            {
                setB.push_back(i);
            }
        }
    }
    int first_ob_num = INF;
    double min_Ob_x_ = INF;
    for(int i=0; i<setB.size(); ++i)
    {
        now_ob = myObstacles_.at(setB.at(i));
        if(now_ob.x_ < min_Ob_x_)
        {
            min_Ob_x_ = now_ob.x_;
            first_ob_num = setB.at(i);
        }
    }
    if(first_ob_num==INF)//无障碍物
    {
        subtargets_pos_ = target;
        return;
    }
    else
    {
        //确定集合G：需要规避的障碍物集合
        DPoint findG_a, findG_b;
        bool used[myObstacles_.size()] = {false};
        for(int i=0; i<setB.size(); ++i)
        {
            findG_a = myObstacles_.at(setB.at(i));
            setG.push_back(setB.at(i));
            used[setB.at(i)] = true;
            for(int j=i+1; j<myObstacles_.size(); ++j)
            {
                findG_b = myObstacles_.at(j);
                if(findG_a.distance(findG_b) < 2*my_radius)
                {
                    if(!used[setB.at(i)])
                    {
                        used[setB.at(i)] = true;
                        setG.push_back(setB.at(i));
                    }
                    if(!used[j])
                    {
                        used[j] = true;
                        setG.push_back(j);
                    }
                }
            }
        }
    }
    //计算子目标点坐标
    //确定集合G的边
    double min_Ob_y_ = INF;
    int min_Ob_num = INF;
    for(int i=0; i<setG.size(); ++i)
    {
        now_ob = myObstacles_.at(setG.at(i));
        //if(abs(now_ob.y_)<min_Ob_y_)
        if(now_ob.norm() < min_Ob_y_)
        {
            //min_Ob_y_=abs(now_ob.y_);
            min_Ob_y_ = now_ob.norm();
            min_Ob_num = setG.at(i);
        }
    }
    if(min_Ob_num == INF)//无障碍物
    {
        subtargets_pos_ = target;
        return;
    }
    double now_ob_angle = INF;
    double max_angle = -INF;
    double max_angle_num;
    int flag_sub_edge = 0;
    now_ob = myObstacles_.at(min_Ob_num);
    if(now_ob.y_ >= 0)
    {
        flag_sub_edge = 1;
        for(int i=0; i<setG.size(); ++i)
        {
            now_ob = myObstacles_.at(setG.at(i));
            if(now_ob.y_ >= 0)
            {
                now_ob_angle = fabs(now_ob.y_)*1.0/now_ob.x_;//x won't be 0;
                if(now_ob_angle > max_angle)
                {
                    max_angle = now_ob_angle;
                    max_angle_num = setG.at(i);
                }
            }
        }
    }
    else if(now_ob.y_ < 0)
    {
        flag_sub_edge = -1;
        for(int i=0; i<setG.size(); ++i)
        {
            now_ob = myObstacles_.at(setG.at(i));
            if(now_ob.y_ < 0)
            {
                now_ob_angle = fabs(now_ob.y_)*1.0/now_ob.x_;//x won't be 0;
                if(now_ob_angle > max_angle)
                {
                    max_angle = now_ob_angle;
                    max_angle_num = setG.at(i);
                }
            }
        }
    }
    //!自身坐标系
    //画圆求切点
    //(x-x1)^2+(y-y1)^2=(r0+r1)^2  delta=0
    //(x1+k*y1)^2+(k*k+1)(r0+r1)^2=0
    //double a=now_ob.y_*now_ob.y_+(my_radius+obs_radius)*(my_radius+obs_radius);
    //double b=2*now_ob.x_*now_ob.y_;
    //double c=now_ob.x_*now_ob.x_+(my_radius+obs_radius)*(my_radius+obs_radius);
    now_ob = myObstacles_.at(max_angle_num);
    double a = square(now_ob.x_)-square(my_radius+obs_radius);
    double b = -2*now_ob.x_*now_ob.y_;
    double c = square(now_ob.y_)-square(my_radius+obs_radius);
    double delta = square(b)-4*a*c;
    double k1, k2;
    double k;
    if(now_ob.x_ != 0) { k = now_ob.y_/now_ob.x_; }
    else if(now_ob.y_ > 0) { k = INF; }
    else { k = -INF; }
    double use_k;
    if(delta >= 0)
    {
        k1 = (-b+sqrt(delta))/(2*a);
        k2 = (-b-sqrt(delta))/(2*a);
        if(k >= 0)
        {
            if(k1 > k2) { use_k = k2; }
            else { use_k = k1; }
        }
        else
        {
            if(k1 < k2) { use_k = k2; }
            else { use_k = k1; }
        }
        //y=use_k*x
        double len = sqrt(square(now_ob.norm())-square(my_radius+obs_radius));
        double thet = atan(use_k);
        subtargets_pos_.x_ = len*cos(thet), subtargets_pos_.y_=len*sin(thet);
        //!转回世界坐标
        ori = subtargets_pos_;
        theta_t = -theta_t;
        //tran_ob.x_ = tran_ob.x_*cos(theta_t)-tran_ob.y_*sin(theta_t);
        //tran_ob.y_ = tran_ob.x_*sin(theta_t)+tran_ob.y_*cos(theta_t);
        tran_ob.x_ = ori.x_*cos(theta_t)-ori.y_*sin(theta_t);
        tran_ob.y_ = ori.x_*sin(theta_t)+ori.y_*cos(theta_t);
        tran_ob += robot_pos_;
        subtargets_pos_ = tran_ob;
    }
    else { subtargets_pos_ = target; }

    /*
    now_ob=myObstacles_.at(max_angle_num);
    //圆心连线
    //y=nowob.y/nowob.x *x;
    //k=nowob.y/nowob.x;=y1/x1;
    double k=now_ob.y_/now_ob.x_;
    double a=1;
    double b=-2*now_ob.x_;
    double c=now_ob.x_*now_ob.x_;
    c=c-(my_radius+obs_radius)*(my_radius+obs_radius)/(1/(k*k)+1);
    //解出x，y=-kx+kx1-y1
    double delta=b*b-4*a*c;
    double x1, x2, y1, y2, k1, k2;
    if(delta>=0)
    {
        x1=(-b+sqrt(delta))/2/a;
        x2=(-b-sqrt(delta))/2/a;
        y1=-k*x1+k*now_ob.x_-now_ob.y_;
        y2=-k*x2+k*now_ob.x_-now_ob.y_;
        k1=y1/x1;
        k2=y2/x2;
        if(k>=0)
        {
            if(k1>k2)//k1
            {
                subtargets_pos_.x_=x1, subtargets_pos_.y_=y1;
            }
            else
            {
                subtargets_pos_.x_=x2, subtargets_pos_.y_=y2;
            }
        }
        else
        {
            if(k1<k2)//k1
            {
                subtargets_pos_.x_=x1, subtargets_pos_.y_=y1;
            }
            else
            {
                subtargets_pos_.x_=x2, subtargets_pos_.y_=y2;
            }
        }
        //!转回世界坐标
        tran_ob=subtargets_pos_;
        theta_t=-theta_t;
        tran_ob.x_=tran_ob.x_*cos(theta_t)-tran_ob.y_*sin(theta_t);
        tran_ob.y_=tran_ob.x_*sin(theta_t)+tran_ob.y_*cos(theta_t);
        tran_ob.x_+=robot_pos_.x_, tran_ob.y_+=robot_pos_.y_;
        subtargets_pos_=tran_ob;
    }
    else
    {
        subtargets_pos_=target;
    }
*/

/*
    int line_num;




    line_num=getpoint(0.0, 0.0, my_radius, now_ob.x_, now_ob.y_, obs_radius);
    DPoint point_one, point_two;
    double k, b, ans_k, ans_b, ans_line;
    double point_k=now_ob.y_/now_ob.x_;
    int ans_point;
    for(int i=0; i<line_num; ++i)
    {
        point_one=Subtargets::first_point.at(i);
        point_two=Subtargets::second_point.at(i);
        if(point_two.y_*now_ob.y_<0)
            continue;
        //两个圆在切线两侧
        if(now_ob.x_>=my_radius+obs_radius)
        {
            if(fabs(point_one.x_-point_two.x_)<1e-7)
            {
                ans_b=my_radius;
                ans_point=i;
                break;
                //斜率不存在
            }
            else
            {
                k=(point_one.y_-point_two.y_)/(point_one.x_-point_two.x_);
                b=point_one.y_-k*point_one.x_;
                if(b*(k*now_ob.x_+b-now_ob.y_)<0)
                {
                    if(fabs(k)>fabs(point_k))
                    {
                        ans_k=k, ans_b=b;
                        ans_point=i;
                        break;
                    }
                }
            }
        }
        else
        {
            if(fabs(point_one.x_-point_two.x_)<1e-7)
            {
                continue;
                //斜率不存在
            }
            else
            {
                k=(point_one.y_-point_two.y_)/(point_one.x_-point_two.x_);
                b=point_one.y_-k*point_one.x_;
                if(b*(k*now_ob.x_+b-now_ob.y_)<0)
                {
                    if(k*point_k<0)
                    {
                        ans_k=k, ans_b=b;
                        ans_point=i;
                        break;
                    }
                }
            }
        }
    }
    DPoint point_xx=Subtargets::first_point.at(ans_point);
    ans_line=point_xx.distance(Subtargets::second_point.at(ans_point));
    if(ans_b!=my_radius)
    {
        double theta_ans=atan(ans_k);
        if(now_ob.x_>=obs_radius+my_radius)
        {
            subtargets_pos_.x_=ans_line*cos(theta_ans);
            subtargets_pos_.y_=ans_line*sin(theta_ans);
        }
        else
        {
            subtargets_pos_.x_=-ans_line*cos(theta_ans);
            subtargets_pos_.y_=-ans_line*sin(theta_ans);
        }
    }
    else
    {
        if(now_ob.y_>=0)
        {
            subtargets_pos_.x_=0.0;
            subtargets_pos_.y_=ans_line;
        }
        else
        {
            subtargets_pos_.x_=0.0;
            subtargets_pos_.y_=-ans_line;
        }
    }

    if(subtargets_pos_==tar_now)
    {
        subtargets_pos_=target;
        return;
    }
    else
    {
        //!转回世界坐标
        tran_ob=subtargets_pos_;
        theta_t=-theta_t;
        tran_ob.x_=tran_ob.x_*cos(theta_t)-tran_ob.y_*sin(theta_t);
        tran_ob.y_=tran_ob.x_*sin(theta_t)+tran_ob.y_*cos(theta_t);
        tran_ob.x_+=robot_pos_.x_, tran_ob.y_+=robot_pos_.y_;
        subtargets_pos_=tran_ob;
    }*/
}
