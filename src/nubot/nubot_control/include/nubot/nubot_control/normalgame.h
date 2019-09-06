#ifndef NORMALGAME_H
#define NORMALGAME_H
#include <cmath>
#include"nubot/core/core.hpp"
#include "nubot/nubot_control/world_model_info.h"
#include "nubot/nubot_control/plan.h"
#include "nubot/nubot_control/dribblestate.hpp"
namespace nubot{

class NormalGame
{

public:
    NormalGame();
    NormalGame(World_Model_Info & _world_model, Plan & _plan, DribbleState &_dribble);
    ~NormalGame();
   void process(int role_ = NOROLE);
   void asGoalie();
   void asActive();
   void asAssist();
   void one2one();
   void update();
   void init();
   bool velzero();

public:
   World_Model_Info * world_model_;
   Plan * m_plan_;
   DPoint robot_pos_;
   Angle robot_ori_;
   DPoint ball_pos_;
   DPoint ball_vel_;
   DPoint opp_robot_[3];//goalie active me
   DribbleState * m_dribble_;

   Actions selected_action_;
   bool kick_enable_;
};

}
#endif // NORMALGAME_H
