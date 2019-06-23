#include "nubot/nubot_control/midfieldrole.h"

using namespace nubot;

MidfieldRole::MidfieldRole()
{
}

MidfieldRole::~MidfieldRole()
{
}
/*

      World_Model_Info * world_model_;
      Plan *plan_;

     // DPoint best_pass_[4];

      DPoint midfield_pos_;
      Angle  midfield_ori_;
*/

void MidfieldRole::asAssist(){}
void MidfieldRole::asPassiver(){}
void MidfieldRole::process(){}

bool MidfieldRole::findOppRobot(){}     //盯人防守，要找到对方机器人的位置
bool MidfieldRole::isNullInTrap(const DPoint & robot_pos, const std::vector<DPoint> & obs_pts){}
bool MidfieldRole::isBestPass(DPoint world_pt){}               //是否在最佳的站位区域
bool MidfieldRole::isObsOpp(DPoint candidate_point){}          //是否在对方的射门角度内
bool MidfieldRole::isObsActiver(DPoint candidate_point){}      //是否在我方的射门角度内

