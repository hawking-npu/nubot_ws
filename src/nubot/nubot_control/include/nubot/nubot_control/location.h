#ifndef LOCATION_H
#define LOCATION_H
#include <cmath>
#include"nubot/core/core.hpp"
#include "nubot/nubot_control/world_model_info.h"
#include "nubot/nubot_control/plan.h"
#include "nubot/nubot_control/dribblestate.hpp"

namespace nubot{

class Location
{

public:
    Location();
    Location(World_Model_Info & _world_model, Plan & _plan, DribbleState & _dribble);
    ~Location();
    void process(int usednum[], int n=5);
    void update();
    void init();

public:
    World_Model_Info * world_model_;
    Plan * m_plan_;
    int flag_location;
    DPoint allPoints[30];
    DPoint robot_pos_;
    Angle robot_ori_;
    DPoint ball_pos_;
    DPoint ball_vel_;
    DribbleState * m_dribble_;
    bool flag_face2target;
    DPoint target;
    DPoint br;
};

}
#endif // LOCATION_H
