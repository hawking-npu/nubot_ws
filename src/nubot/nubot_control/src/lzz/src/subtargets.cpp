#include "nubot/nubot_control/subtargets.h"
#include "nubot/core/core.hpp"
#include "nubot/nubot_control/mydefine.hpp"
using namespace nubot;
/*
World_Model_Info * world_model_;
DPoint subtargets_pos_;
DPoint robot_pos_;
DPoint ball_pos_;
*/
Subtargets::Subtargets()
{

}


int Subtargets::Min_num(int n,double *q)
{
    double minn=inf;
    int temp=inf;
    for(int i=0; i<n; i++)
        if(q[i]<minn)
        {
            minn=q[i];
            temp=i;
        }
    return temp;
}
double Subtargets::Min(int n,double *q)
{
    double minn=inf;
    for(int i=0; i<n; i++)
        if(q[i]<minn)
            minn=q[i];
    return minn;
}
int Subtargets::Max_num(int n,double *q)
{
    double maxn=0;
    int temp=inf;
    for(int i=0; i<n; i++)
        if(q[i]>maxn)
        {
            maxn=q[i];
            temp=i;
        }
    return temp;
}
double Subtargets::Max(int n,double *q)
{
    double maxn=0;
    for(int i=0; i<n; i++)
        if(q[i]>maxn)
            maxn=q[i];
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
    //此处为世界坐标系*/


    //坐标转换
    //robot_pos_

    DPoint tran_ob;
    std::vector<nubot::DPoint> myObstacles_;//自身坐标下，筛选保留x>0的点
    std::vector<nubot::DPoint> _myObstacles;
    _myObstacles=world_model_->Obstacles_;
    if(avoid_ball)
        _myObstacles.push_back(ball_pos_);
    double ori_x, ori_y;
    double theta_t;
    DPoint tar_now=target;
    tar_now.x_-=robot_pos_.x_, tar_now.y_-=robot_pos_.y_;
    if(tar_now.x_==0)
    {
        if(tar_now.y_>0)
        {
            theta_t=PI/2;
        }
        else if(tar_now.y_<0)
        {
            theta_t=-PI/2;
        }
        else
        {
            theta_t=0;
        }
    }
    else
    {
        theta_t=atan(tar_now.y_/tar_now.x_);
    }
    theta_t=-theta_t;
    ori_x=tar_now.x_, ori_y=tar_now.y_;
    tar_now.x_=ori_x*cos(theta_t)-ori_y*sin(theta_t);
    tar_now.y_=ori_x*sin(theta_t)+ori_y*cos(theta_t);

    myObstacles_.clear();
    for(int i=0; i<_myObstacles.size(); i++)
    {
        tran_ob=_myObstacles.at(i);
        tran_ob.x_-=robot_pos_.x_, tran_ob.y_-=robot_pos_.y_;

        ori_x=tran_ob.x_, ori_y=tran_ob.y_;

        tran_ob.x_=ori_x*cos(theta_t)-ori_y*sin(theta_t);
        tran_ob.y_=ori_x*sin(theta_t)+ori_y*cos(theta_t);
        if(tran_ob.x_>=0)
            myObstacles_.push_back(tran_ob);
    }

    //确定第一障碍物,建立分组
    std::vector <int> setB, setG;
    setB.clear(), setG.clear();


    ROS_INFO("Subtargets myObstacles_num: %ld", myObstacles_.size());

    //确定集合B：机器人沿直线运动到目标点会撞到的障碍物
    DPoint now_ob;
    for(int i = 0; i < myObstacles_.size(); i++)
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
    int first_ob_num=100;
    double min_Ob_x_=inf;
    for(int i=0; i<setB.size(); i++)
    {
        now_ob=myObstacles_.at(setB.at(i));
        if(now_ob.x_<min_Ob_x_)
        {
            min_Ob_x_=now_ob.x_;
            first_ob_num=setB.at(i);
        }
    }
    if(first_ob_num==100)//无障碍物
    {
        subtargets_pos_=target;
        return;
    }
    else
    {
        //确定集合G：需要规避的障碍物集合
        DPoint findG_a, findG_b;
        bool used[myObstacles_.size()]={0};
        for(int i=0; i<setB.size(); i++)
        {
            findG_a=myObstacles_.at(setB.at(i));
            for(int j=i+1; j<myObstacles_.size(); j++)
            {
                findG_b=myObstacles_.at(j);
                if(findG_a.distance(findG_b)<2*my_radius)
                {
                    if(!used[setB.at(i)])
                    {
                        used[setB.at(i)]=1;
                        setG.push_back(setB.at(i));
                    }
                    if(!used[j])
                    {
                        used[j]=1;
                        setG.push_back(j);
                    }
                }
            }
        }
    }
    //计算子目标点坐标
    //确定集合G的边
    double min_Ob_y_=inf;
    int min_Ob_num=100;
    for(int i=0; i<setG.size(); i++)
    {
        now_ob=myObstacles_.at(setG.at(i));
        if(abs(now_ob.y_)<min_Ob_y_)
        {
            min_Ob_y_=abs(now_ob.y_);
            min_Ob_num=setG.at(i);
        }
    }
    if(min_Ob_num==100)//无障碍物
    {
        subtargets_pos_=target;
        return;
    }
    double now_ob_angle=1000;
    double max_angle=-1;
    double max_angle_num;
    int flag_sub_edge=0;
    if(myObstacles_.at(min_Ob_num).y_>=0)
    {
        flag_sub_edge=1;
        for(int i=0; i<setG.size(); i++)
        {
            now_ob=myObstacles_.at(setG.at(i));
            if(now_ob.y_>=0)
            {
                now_ob_angle=abs(now_ob.y_)*1.0/now_ob.x_;//x won't be 0;
                if(now_ob_angle>max_angle)
                {
                    max_angle=now_ob_angle;
                    max_angle_num=setG.at(i);
                }
            }
        }
    }
    else if(myObstacles_.at(min_Ob_num).y_<0)
    {
        flag_sub_edge=-1;
        for(int i=0; i<setG.size(); i++)
        {
            now_ob=myObstacles_.at(setG.at(i));
            if(now_ob.y_<0)
            {
                now_ob_angle=abs(now_ob.y_)*1.0/now_ob.x_;//x won't be 0;
                if(now_ob_angle>max_angle)
                {
                    max_angle=now_ob_angle;
                    max_angle_num=setG.at(i);
                }
            }
        }
    }
    //!自身坐标系
    //求自身和该障碍物的切线，切线的斜率绝对值更大
    //子目标点与切线和障碍物向量相切，且不与障碍物相交
    now_ob=myObstacles_.at(max_angle_num);
    int line_num;
    line_num=getpoint(0.0, 0.0, my_radius, now_ob.x_, now_ob.y_, obs_radius);
    DPoint point_one, point_two;
    double k, b, ans_k, ans_b, ans_line;
    double point_k=now_ob.y_/now_ob.x_;
    int ans_point;
    for(int i=0; i<line_num; i++)
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
        ori_x=tran_ob.x_, ori_y=tar_now.y_;
        tran_ob.x_=ori_x*cos(theta_t)-ori_y*sin(theta_t);
        tran_ob.y_=ori_x*sin(theta_t)+ori_y*cos(theta_t);
        tran_ob.x_+=robot_pos_.x_, tran_ob.y_+=robot_pos_.y_;
        subtargets_pos_=tran_ob;
    }
}


