#ifndef PLAN_H
#define PLAN_H

#include <cmath>
#include "nubot/core/core.hpp"
#include "nubot/nubot_control/subtargets.h"
#include "nubot/nubot_control/behaviour.hpp"
#include "nubot/nubot_control/world_model_info.h"
#include "nubot/nubot_control/nubot_control.h"
#include "nubot/nubot_control/dribblestate.hpp"

using namespace std;
namespace nubot{
class Plan
{
public:
        Plan();

        /***********catch ball***********/
        void catchBall();
        void catchBallForCoop();
        void catchBallSlowly();
        void catchMovingBall(); // 移动的球
        void catchMotionlessBall(); // 静止的球

        /***********postion*************/
        void positionAvoidObs(DPoint target, double maxw = MAXW, double angle_thres = 5.0/180.0/*float theta, float stopdis, float stoptheta*/);
        void positionAvoidObs2(double targetangle, double maxw = MAXW, double angle_thres = 5.0/180.0);
        void driblleControl(DPoint target,double acc,double sacc,double lvel,double maxvel = MAXVEL);
        //void move2Positionwithobs(DPoint target);  // move to the target point with obstacles avoidance
        void move2Positionwithobs_noball(DPoint target, double distance_thres = 10.0, float maxvel = MAXVEL, float maxacc = max_acc, bool avoid_ball=false);

        /***********PE and PO***********/
        double PECrossBackMIdlleLine(double direction);
        double PEOutField(double direction);
        double PEInOurPenaty(double direction);
        double PObleDirection4OurField(double direction, double predictlen, double cobledirection, double kobledirection);
        double PObleDirection(double direction, double predictlen, double cobledirection,double kobledirection);

        /***********check***********/
        bool IsNullInTrap(double direction, double swidth, double lwidth, double len);
        bool pnpoly(const std::vector<DPoint> & pts, const DPoint & test_pt);
        /*********find and search*********/
        double FindBstDirectionForAvoid();
        double FindBstDirectionForAvoid2(DPoint target);
        int    GetAvoidState();
        double SearchDirectionforMinPEPoint(double oridirection,double step,int lefttime,int righttime);
        bool   SearchMinPE4PassThroughforOurField(double &direction,double pridictlen,DPoint trap[4],double step,int flg);
        bool   SearchMinPE4PassThrough(double &direction, double pridictlen, DPoint trap[4], double step, int flg);

        void   update();


        /*********round with soccer************/
        void ballRoundTrack();


        int oppneartargetid(DPoint target);
        int ourneartargetid(DPoint target, int avoidid = 0);

public:
        World_Model_Info * world_model_;
        Behaviour  m_behaviour_;
        Subtargets m_subtargets_;
        DribbleState * m_dribble_;

        float kp;
        float kalpha;
        float kbeta;
        bool  inourfield_;
        bool  inoppfield_;
        double lastdirection;

        DPoint robot_pos_;
        Angle  robot_ori_;
        DPoint robot_vec_;
        DPoint ball_pos_;
        DPoint ball_vec_;

        vector<DPoint> target_;

public:
        bool   isinposition_;
};
}
#endif //PLAN_H
