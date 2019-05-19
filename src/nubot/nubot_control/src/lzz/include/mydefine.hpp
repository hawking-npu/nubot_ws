#ifndef MYDEFINE_H
#define MYDEFINE_H

#include "nubot/core/core.hpp"
#include <cmath>

using namespace std;

const double eps = 1e-8;
const double PI = acos(-1.0);
const double inf = 0x7fffffff;

const double update_T = 30;//30ms更新一次
const double delay_time=120;//延迟时间

////需要修改
const double max_w = 6.0;//rad/s
const double max_wacc = 3.0;//rad/s
const double max_vel = 500.0;//cm/s
const double max_acc = 250.0;//cm2/s
const double my_radius = 20.0;//cm
const double obs_radius = 20.0;//cm

#endif // MYDEFINE_H
