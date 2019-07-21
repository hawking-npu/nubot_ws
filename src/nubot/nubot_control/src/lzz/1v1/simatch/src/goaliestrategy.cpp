#include <iostream>
#include <fstream>
#include "nubot/nubot_control/goaliestrategy.h"

#include "nubot/nubot_control/mydefine.hpp"
#include <stdio.h>
#include <stdlib.h>

using namespace nubot;

nubot::GoalieStrategy::GoalieStrategy()
{
  robot_info_.pos.x = -850;
  robot_info_.pos.y = 0;
  robot_info_.heading.theta = 0;
  robot_info_.vtrans.x = 0;
  robot_info_.vtrans.y = 0;
  ball_info_2d_ .pos_known = false;
  ball_info_3d_.pos_known_3d = false;
  ball_info_3d_.pos_known_2d = false;
  state_ = Move2Origin;
}
/*
class GoalieStrategy
{
public:
  typedef enum {
    StandBy      = 0,
    Move2Ball    = 1,
    Move2Origin  = 2,
    Turn2Ball    = 3,
//    CatchBall    = 4,
//    KickBall     = 5,
  } GoalieState;//the goalie state of FSM
  GoalieState state_;
  DPoint dest_point_;  //Destination made by strategy()
  double dest_angle_;  //Destination made by strategy()
  double thresh_vel_;  //thresh of movement made by strategy()
  double thresh_omiga_;//thresh of movement made by strategy()
  std::string debug_str_;//the info of goalie set by strategy()

  GoalieStrategy();
  nubot_common::RobotInfo       robot_info_;
  nubot_common::BallInfo3d      ball_info1_3d_;//you can set ball_info_3d_ using global info, or call setBallInfo3dRel() using relative info
  nubot_common::BallInfo3d      ball_info2_3d_;//you can set ball_info_3d_ using global info, or call setBallInfo3dRel() using relative info
  nubot_common::BallInfo3d ball_info_3d_;//if kinect#1 see ball, it will be set to #1; else set to #2
  nubot_common::BallInfo        ball_info_2d_;
  void setBallInfo3dRel(const nubot_common::BallInfo3d &_robot_info_3d_rel);//you can set ball_info_3d_ using global info, or call setBallInfo3dRel() using relative info
  bool ballTrack( const int THRESH_GROUND_VEL=200, const bool use_parabola_fitter_=true );//used in "mainCallBack()", and make a prediction if ball could goal
  void strategy();//used in "mainCallBack()", make decision using prediction info

private:
  ParabolaFitter3D parabola_fitter_;//used in ballTrack()
  bool   predicted_3d_;             //set by ballTrack()
  bool   predicted_2d_;             //set by ballTrack()
  bool   predictec_omi_;            //set by ballTrack()
  DPoint bounding_pt_;              //set by ballTrack()
  DPoint crossing_pt_;              //set by ballTrack()
  double bounding_time_;            //set by ballTrack()
  double crossing_time_dis_;            //set by ballTrack()
  double time_now_;                 //set by ballTrack()
};
*/
void GoalieStrategy::setBallInfo3dRel(const nubot_common::BallInfo3d &_robot_info_3d_rel)//you can set ball_info_3d_ using global info, or call setBallInfo3dRel() using relative info
{
    ball_info_3d_ = _robot_info_3d_rel;
    parabola_fitter_.flyCheckAndAddData(ball_info_3d_.pos.z,
                                        DPoint(ball_info_3d_.pos.x,ball_info_3d_.pos.y), time_now_);
}

bool GoalieStrategy::ballTrack(const int THRESH_GROUND_VEL, const bool use_parabola_fitter_)//used in "mainCallBack()", and make a prediction if ball could goal
{
    if(use_parabola_fitter_ == false) { return false; }
    time_now_ = ros::Time::now().sec;
    predicted_3d_ = ball_info_3d_.pos_known_3d;
    predicted_2d_ = ball_info_3d_.pos_known_2d;
    if(parabola_fitter_.n_ >= 3)
    {
        parabola_fitter_.fitting();
        bounding_pt_       = parabola_fitter_.bounding_point_;
        bounding_time_     = parabola_fitter_.bounding_time_;
        crossing_pt_       = parabola_fitter_.crossing_point_;
        crossing_time_dis_ = parabola_fitter_.crossing_time_;
    }
}

void GoalieStrategy::strategy()//used in "mainCallBack()", make decision using prediction info
{
    //ROS_INFO("robot1 Goalie Strategy");
    double time_;

    ballTrack();
    if(crossing_time_dis_ != -1 && crossing_time_dis_ != 0)
    {
        dest_point_ = crossing_pt_;
        time_ = crossing_time_dis_;
        state_ = Move2Ball;
    }
    else if(bounding_time_ != -1 && bounding_time_ != 0)
    {
        dest_point_ = bounding_pt_;
        time_ = bounding_time_;
        state_ = Move2Ball;
    }
    else
    {
        dest_point_ = DPoint(GOAL_POS_X*1.0, GOAL_POS_Y*1.0);
        state_ = Move2Origin;
    }
    DPoint ball2d_pos = DPoint(ball_info_3d_.pos.x,ball_info_3d_.pos.y);
    DPoint tmp = ball2d_pos - dest_point_;
    dest_angle_ = atan2(tmp.x_, tmp.y_);
    if(fabs(time_now_ - time_) < 1.0)//////
    {
        state_ = CatchBall;
    }
}


ParabolaFitter3D::ParabolaFitter3D()
{
    n_            = 0;
    data_pointer_ = -1;
    fly_flag_     = 0;
    memset(t_, 0, sizeof(t_));
    memset(x_, 0, sizeof(x_));
    memset(y_, 0, sizeof(y_));
    memset(z_, 0, sizeof(z_));
}

int ParabolaFitter3D::addData(const double _add_z, const DPoint& _add_pos_now, const double _time)//be called in "FlyCheckAndAddData"
{
    if(n_ < MAXNUM_OF_FIT) { ++n_; }
    data_pointer_ = (data_pointer_+1) % MAXNUM_OF_FIT;
    t_[data_pointer_] = _time;
    z_[data_pointer_] = _add_z;
    x_[data_pointer_] = _add_pos_now.x_;
    y_[data_pointer_] = _add_pos_now.y_;
    return n_;
}

bool ParabolaFitter3D::flyCheckAndAddData(const double _z_now, const DPoint& _pos_now, const double _time)
{
    if(_z_now > 0) { fly_flag_ = 1; }
    if(fly_flag_ == 1)
    {
        n_ = addData(_z_now, _pos_now, _time);
        fly_flag_ = 0;
        return true;
    }
    return false;
}

void ParabolaFitter3D::clearDataBuffer()  //清除数据缓冲区
{
    n_            = 0;
    data_pointer_ = -1;
    memset(t_, 0, sizeof(t_));
    memset(x_, 0, sizeof(x_));
    memset(y_, 0, sizeof(y_));
    memset(z_, 0, sizeof(z_));
}

void ParabolaFitter3D::fitting(double* _pfitting_err/* 残差平方和*/) //拟合
{
    if(n_ < 3) //数据少于3个，无法拟合
    {
        return;
    }
    if(data_pointer_ != MAXNUM_OF_FIT-1 && n_ == MAXNUM_OF_FIT) //溢出
    {
        double tmp_t[MAXNUM_OF_FIT];
        double tmp_x[MAXNUM_OF_FIT];
        double tmp_y[MAXNUM_OF_FIT];
        double tmp_z[MAXNUM_OF_FIT];

        for(int i=0, j=(data_pointer_+1)%MAXNUM_OF_FIT; i<MAXNUM_OF_FIT; ++i, j=(j+1)%MAXNUM_OF_FIT)
        {
            tmp_t[i] = t_[j];
            tmp_x[i] = x_[j];
            tmp_y[i] = y_[j];
            tmp_z[i] = z_[j];
        }
        for(int i=0; i<MAXNUM_OF_FIT; ++i)
        {
            t_[i] = tmp_t[i];
            x_[i] = tmp_x[i];
            y_[i] = tmp_y[i];
            z_[i] = tmp_z[i];
        }
        data_pointer_ = MAXNUM_OF_FIT-1;
    }//第1个最早放，第MAXNUM_OF_FIT个最晚放

    //线性回归
    //x=p0*t+p1，y=p2*t+p3
    //z=-hafegravity*t^2+p4*t+p5
    sum_t_  = sum_x_  = sum_y_  = sum_z_  = 0;
    sum_tt_ = sum_tx_ = sum_ty_ = sum_tz_ = 0;
    t0_ = 0;
    for(int i=0; i<n_; ++i)
    {//sum
        sum_t_ += t_[i];
        sum_x_ += x_[i];
        sum_y_ += y_[i];
        sum_z_ += z_[i];
        t0_    += t_[i]*t_[i];
    }
    //average
    sum_tt_ = sum_t_ / (data_pointer_+1);
    sum_tx_ = sum_x_ / (data_pointer_+1);
    sum_ty_ = sum_y_ / (data_pointer_+1);
    sum_tz_ = sum_z_ / (data_pointer_+1);
    t0_     = t0_    / (data_pointer_+1);

    double a, b;
    //x=p0*t+p1
    a = b = 0;
    for(int i=0; i<n_; ++i)
    {
        b += ((t_[i]-sum_tt_)*(x_[i]-sum_tx_));
        a += ((t_[i]-sum_tt_)*(t_[i]-sum_tt_));
    }
    b = b / a;
    a = sum_tx_ - b*sum_tt_;
    model_param_[0] = b;
    model_param_[1] = a;

    //y=p2*t+p3
    a = b = 0;
    for(int i=0; i<n_; ++i)
    {
        b += ((t_[i]-sum_tt_)*(y_[i]-sum_ty_));
        a += ((t_[i]-sum_tt_)*(t_[i]-sum_tt_));
    }
    b = b / a;
    a = sum_ty_ - b*sum_tt_;
    model_param_[2] = b;
    model_param_[3] = a;

    //z=-hafegravity*t^2+p4*t+p5
    a = b = 0;
    for(int i=0; i<n_; ++i)
    {
        b += ((t_[i]-sum_tt_) * ((z_[i]+HafeGravity*t_[i]*t_[i]) - (sum_tz_+HafeGravity*t0_)));
        a += ((t_[i]-sum_tt_) * (t_[i]-sum_tt_));
    }
    b = b / a;
    a = (sum_tz_ + HafeGravity*t0_) - b*sum_tt_;
    model_param_[4] = b;
    model_param_[5] = a;


    _pfitting_err = (double *)malloc(3*sizeof(double));
    _pfitting_err[0] = _pfitting_err[1] = _pfitting_err[2] = 0;
    for(int i=0; i<n_; ++i)
    {
        _pfitting_err[0] = (x_[i] - (model_param_[0]*t_[i]+model_param_[1]))
                * (x_[i] - (model_param_[0]*t_[i]+model_param_[1]));
        _pfitting_err[1] = (y_[i] - (model_param_[2]*t_[i]+model_param_[3]))
                * (y_[i] - (model_param_[2]*t_[i]+model_param_[3]));
        _pfitting_err[2] = (z_[i] - (-HafeGravity*t_[i]*t_[i]+model_param_[4]*t_[i]+model_param_[5]))
                * (z_[i] - (-HafeGravity*t_[i]*t_[i]+model_param_[4]*t_[i]+model_param_[5]));
    }
    //when z = 0, get pos (bounding point) and time(bounding time)
    bounding_time_  = (-model_param_[4] + sqrt(model_param_[4]*model_param_[4] - 4*HafeGravity*model_param_[5]))
            / (2 * HafeGravity);
    bounding_point_ = DPoint(model_param_[0]*bounding_time_+model_param_[1],
            model_param_[2]*bounding_time_+model_param_[3]);
    if(bounding_point_.x_ < GOAL_POS_X-2) //kick on my goal when flying
    {
        bounding_time_  = (GOAL_POS_X - model_param_[1]) / model_param_[0];
        bounding_point_ = DPoint(GOAL_POS_X * 1.0, model_param_[2]*bounding_time_+model_param_[3]);
        crossing_time_  = -1;
        crossing_point_ = DPoint(0, 0);
    }
    else
    {
        crossing_time_  = (GOAL_POS_X - model_param_[1]) / model_param_[0];
        crossing_point_ = DPoint(GOAL_POS_X * 1.0, model_param_[2]*bounding_time_+model_param_[3]);
    }
}

void ParabolaFitter3D::saveFileTXT(const char* _pFilename)//save data buffer and fitting result
{
    if(data_pointer_ != MAXNUM_OF_FIT-1 && n_ == MAXNUM_OF_FIT) //溢出
    {
        double tmp_t[MAXNUM_OF_FIT];
        double tmp_x[MAXNUM_OF_FIT];
        double tmp_y[MAXNUM_OF_FIT];
        double tmp_z[MAXNUM_OF_FIT];

        for(int i=0, j=(data_pointer_+1)%MAXNUM_OF_FIT; i<MAXNUM_OF_FIT; ++i, j=(j+1)%MAXNUM_OF_FIT)
        {
            tmp_t[i] = t_[j];
            tmp_x[i] = x_[j];
            tmp_y[i] = y_[j];
            tmp_z[i] = z_[j];
        }
        for(int i=0; i<MAXNUM_OF_FIT; ++i)
        {
            t_[i] = tmp_t[i];
            x_[i] = tmp_x[i];
            y_[i] = tmp_y[i];
            z_[i] = tmp_z[i];
        }
        data_pointer_ = MAXNUM_OF_FIT-1;
    }//第1个最早放，第MAXNUM_OF_FIT个最晚放

    FILE *pFile;
    pFile=fopen(_pFilename,"w");
    if(pFile==NULL)
    {
        return;
    }

    fprintf(pFile, "fitting result\n");
    fprintf(pFile, "\n=====================================================\n\n");
    fprintf(pFile, "Formula\n");
    fprintf(pFile, "x = %f*t + %f\n", model_param_[0], model_param_[1]);
    fprintf(pFile, "y = %f*t + %f\n", model_param_[2], model_param_[3]);
    fprintf(pFile, "z = %.1f*t^2 + %f*t + %f", HafeGravity, model_param_[4], model_param_[5]);
    fprintf(pFile, "\n\n=====================================================\n\n");
    fprintf(pFile, "data buffer\n");
    char str2[5][4] = {"num","t","x","y","z"};
    fprintf(pFile, "%-8s", str2[0]);
    for(int i=1; i<5; ++i)
    {
        fprintf(pFile, "%-16s", str2[i]);
    }
    fprintf(pFile, "\n");
    for(int i=0; i<n_; ++i)
    {
        fprintf(pFile, "%-8d%-16f%-16f%-16f%-16f\n", i+1, t_[i], x_[i], y_[i], z_[i]);
    }
    fprintf(pFile, "\n=====================================================\n\n");

    fclose(pFile);
    return;
}

double ParabolaFitter3D::getStartTime()    //最早放进去的时间
{
    if(data_pointer_ != MAXNUM_OF_FIT-1 && n_ == MAXNUM_OF_FIT) //溢出
    {
        return t_[(data_pointer_+1)%MAXNUM_OF_FIT];
    }
    else if(n_ == 0)
    {
        return -1;
    }
    else
    {
        return t_[0];
    }
}

double ParabolaFitter3D::getEndTime()    //最晚放进去的时间
{
    if(data_pointer_ != MAXNUM_OF_FIT-1 && n_ == MAXNUM_OF_FIT) //溢出
    {
        return t_[n_-1];
    }
    else if(n_==0)  //没有放
    {
        return -1;
    }
    else
    {
        return t_[data_pointer_];
    }
}
