#ifndef MYDEFINE_H
#define MYDEFINE_H

#include "nubot/core/core.hpp"
#include "nubot/core/DPoint.hpp"
#include <cmath>

using namespace std;
using namespace nubot;

const double EPS = 0.1;
const double PI = acos(-1.0);
const double INF = 0x3f3f3f;

////可能需要修改
const double update_T = 15;//30ms更新一次
const DPoint oppGoalPosr = DPoint( 890, -100);
const DPoint oppGoalPosl = DPoint( 890,  100);
const DPoint ourGoalPosr = DPoint(-890, -100);
const DPoint ourGoalPosl = DPoint(-890,  100);
const DPoint parking_point = DPoint (-800.0, 0.0);
const double LENGTH_FIX = 1.0;

////需要修改
const double max_w = 6.0;//rad/s
const double max_wacc = 3.0;//rad/s
const double max_vel = 100.0;//cm/s
const double max_acc = 50.0;//cm2/s
const double my_radius = 40.0;//cm
const double obs_radius = 40.0;//cm
const double dist_KICKGoal1 = 300.0; //踢球最远距离
const double dist_KICKGoal2 = 70.0;
const double KA_MOVE = 1.0;
const double KB_MOVE = 0.01;

#endif // MYDEFINE_H
