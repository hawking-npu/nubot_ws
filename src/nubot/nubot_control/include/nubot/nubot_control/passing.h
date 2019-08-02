#ifndef PASSING_H
#define PASSING_H
#include <cmath>
#include"nubot/core/core.hpp"
#include "nubot/nubot_control/world_model_info.h"
#include "nubot/nubot_control/plan.h"
#include "nubot/nubot_control/dribblestate.hpp"

const int maxseentime = 1.0*1000/update_T;

namespace nubot{

struct BallKnow
{
    DPoint ball_seen_;
    int seen_time_;
};

class Passing
{
public:
    enum DishColor
    {
        RED = 1,
        BLUE = 2,
        WHITE = 3,
        YELLOW = 4
    };

    Passing();
    Passing(World_Model_Info & _world_model, Plan & _plan, DribbleState & _dribble);
    ~Passing();

    void init();
    void process();
    void update();
    void process_level(int ball_id_, DishColor color_);
    bool move2kick(DishColor color_);
    bool CatchBall(DPoint target);

public:
   World_Model_Info * world_model_;
   Plan * m_plan_;
   DPoint robot_pos_;
   Angle robot_ori_;
   DPoint ball_pos_;
   DribbleState * m_dribble_;

public:
   DPoint dish_[5];
   DPoint ball_[5];
   int level;
   DPoint kick_[5];
   bool kick_enable_;
   BallKnow ball_know[5];
   bool have_kick[5];
   bool can_dribble[5];
   bool flag_face2kick;

   double dish_y[6];
   double dish_x_;
   double kick_x_;
   double ball_x_;
};

}
#endif // PASSING_H
