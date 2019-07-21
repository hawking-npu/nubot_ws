#include "nubot/nubot_control/subtargets.h"
#include "nubot/core/core.hpp"
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


    //坐标转换
    //robot_pos_

    DPoint tran_ob;
    std::vector<nubot::DPoint> myObstacles_;//自身坐标下，筛选保留x>0的点
    std::vector<nubot::DPoint> _myObstacles;
    _myObstacles = world_model_->Opponents_;
    if(avoid_ball)
        _myObstacles.push_back(ball_pos_);
    //ROS_INFO("Subtargets 1myObstacles_num: %ld", _myObstacles.size());
    DPoint ori;
    double theta_t;
    DPoint tar_now = target;
    tar_now -= robot_pos_;
    theta_t = atan2(tar_now.y_, tar_now.x_);
    theta_t = -theta_t;
    ori = tar_now;
    tar_now.x_ = ori.x_*cos(theta_t)-ori.y_*sin(theta_t);
    tar_now.y_ = ori.x_*sin(theta_t)+ori.y_*cos(theta_t);

    myObstacles_.clear();
    for(int i=0; i<_myObstacles.size(); ++i)
    {
        tran_ob = _myObstacles[i];
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
        now_ob = myObstacles_[i];
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
        now_ob = myObstacles_[setB[i]];
        if(now_ob.x_ < min_Ob_x_)
        {
            min_Ob_x_ = now_ob.x_;
            first_ob_num = setB[i];
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
            findG_a = myObstacles_[setB[i]];
            setG.push_back(setB[i]);
            used[setB[i]] = true;
            for(int j=i+1; j<myObstacles_.size(); ++j)
            {
                findG_b = myObstacles_[j];
                if(findG_a.distance(findG_b) < 2*my_radius)
                {
                    if(!used[setB[i]])
                    {
                        used[setB[i]] = true;
                        setG.push_back(setB[i]);
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
        now_ob = myObstacles_[setG[i]];
        //if(abs(now_ob.y_)<min_Ob_y_)
        if(now_ob.norm() < min_Ob_y_)
        {
            //min_Ob_y_=abs(now_ob.y_);
            min_Ob_y_ = now_ob.norm();
            min_Ob_num = setG[i];
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
    now_ob = myObstacles_[min_Ob_num];
    if(now_ob.y_ >= 0)
    {
        flag_sub_edge = 1;
        for(int i=0; i<setG.size(); ++i)
        {
            now_ob = myObstacles_[setG[i]];
            if(now_ob.y_ >= 0)
            {
                now_ob_angle = fabs(now_ob.y_)*1.0/now_ob.x_;//x won't be 0;
                if(now_ob_angle > max_angle)
                {
                    max_angle = now_ob_angle;
                    max_angle_num = setG[i];
                }
            }
        }
    }
    else if(now_ob.y_ < 0)
    {
        flag_sub_edge = -1;
        for(int i=0; i<setG.size(); ++i)
        {
            now_ob = myObstacles_[setG[i]];
            if(now_ob.y_ < 0)
            {
                now_ob_angle = fabs(now_ob.y_)*1.0/now_ob.x_;//x won't be 0;
                if(now_ob_angle > max_angle)
                {
                    max_angle = now_ob_angle;
                    max_angle_num = setG[i];
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
    now_ob = myObstacles_[max_angle_num];
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
}
