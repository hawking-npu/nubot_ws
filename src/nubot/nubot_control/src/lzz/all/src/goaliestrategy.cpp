#include <iostream>
#include <fstream>
#include "nubot/nubot_control/goaliestrategy.h"

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

void GoalieStrategy::setBallInfo3dRel(const nubot_common::BallInfo3d &_robot_info_3d_rel){}//you can set ball_info_3d_ using global info, or call setBallInfo3dRel() using relative info
bool GoalieStrategy::ballTrack(const int THRESH_GROUND_VEL, const bool use_parabola_fitter_){}//used in "mainCallBack()", and make a prediction if ball could goal
void GoalieStrategy::strategy(){}//used in "mainCallBack()", make decision using prediction info

int ParabolaFitter3D::addData(const double _add_z, const DPoint& _add_pos_now, const double _time){}//be called in "FlyCheckAndAddData"
ParabolaFitter3D::ParabolaFitter3D(){}
bool ParabolaFitter3D::flyCheckAndAddData(const double _z_now, const DPoint& _pos_now, const double _time){}
void ParabolaFitter3D::clearDataBuffer(){}
void ParabolaFitter3D::fitting(double* _pfitting_err){}
void ParabolaFitter3D::saveFileTXT(const char* _pFilename){}//save data buffer and fitting result
double ParabolaFitter3D::getStartTime(){}
double ParabolaFitter3D::getEndTime(){}
