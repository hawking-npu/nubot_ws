#include "nubot/nubot_control/fieldinformation.h"

using namespace nubot;

const int simLOCATIONERROR = 0;

FieldInformation::FieldInformation()
{
     xline_.push_back(FIELD_XLINE1);  /** 900，0*/
     xline_.push_back(FIELD_XLINE2);
     xline_.push_back(FIELD_XLINE3);
     xline_.push_back(FIELD_XLINE4);
     xline_.push_back(FIELD_XLINE5);
     xline_.push_back(FIELD_XLINE6);
     xline_.push_back(FIELD_XLINE7); /** -900，6*/

     yline_.push_back(FIELD_YLINE1);  /** 600，0 */
     yline_.push_back(FIELD_YLINE2);
     yline_.push_back(FIELD_YLINE3);
     yline_.push_back(FIELD_YLINE4);
     yline_.push_back(FIELD_YLINE5);
     yline_.push_back(FIELD_YLINE6); /** -600，5 */

     /** 场地中存在的所有的白线线段*/
     x_white_line_.push_back(LineSegment(DPoint(xline_[0],yline_[5]),DPoint(xline_[0],yline_[0])));
     x_white_line_.push_back(LineSegment(DPoint(xline_[1],yline_[3]),DPoint(xline_[1],yline_[2])));
     x_white_line_.push_back(LineSegment(DPoint(xline_[2],yline_[4]),DPoint(xline_[2],yline_[1])));
     x_white_line_.push_back(LineSegment(DPoint(xline_[3],yline_[5]),DPoint(xline_[3],yline_[0])));
     x_white_line_.push_back(LineSegment(DPoint(xline_[4],yline_[4]),DPoint(xline_[4],yline_[1])));
     x_white_line_.push_back(LineSegment(DPoint(xline_[5],yline_[3]),DPoint(xline_[5],yline_[2])));
     x_white_line_.push_back(LineSegment(DPoint(xline_[6],yline_[5]),DPoint(xline_[6],yline_[0])));

     y_white_line_.push_back(LineSegment(DPoint(xline_[6],yline_[5]),DPoint(xline_[0],yline_[5])));
     y_white_line_.push_back(LineSegment(DPoint(xline_[6],yline_[0]),DPoint(xline_[0],yline_[0])));
     y_white_line_.push_back(LineSegment(DPoint(xline_[6],yline_[4]),DPoint(xline_[4],yline_[4])));
     y_white_line_.push_back(LineSegment(DPoint(xline_[0],yline_[4]),DPoint(xline_[2],yline_[4])));
     y_white_line_.push_back(LineSegment(DPoint(xline_[6],yline_[1]),DPoint(xline_[4],yline_[1])));
     y_white_line_.push_back(LineSegment(DPoint(xline_[0],yline_[1]),DPoint(xline_[2],yline_[1])));
     y_white_line_.push_back(LineSegment(DPoint(xline_[6],yline_[3]),DPoint(xline_[5],yline_[3])));
     y_white_line_.push_back(LineSegment(DPoint(xline_[0],yline_[3]),DPoint(xline_[1],yline_[3])));
     y_white_line_.push_back(LineSegment(DPoint(xline_[6],yline_[2]),DPoint(xline_[5],yline_[2])));
     y_white_line_.push_back(LineSegment(DPoint(xline_[0],yline_[2]),DPoint(xline_[1],yline_[2])));

    centercircle_.radius_=FIELD_CENTER_RADIUS;
    centercircle_.center_=DPoint2d(0,0);

    postcircle_.resize(4);
    for(size_t i=0; i< 4;i++)
       postcircle_[i].radius_=FIELD_POST_RADIUS;
    postcircle_[0].center_=DPoint2d(xline_[0],-yline_[0]);
    postcircle_[1].center_=DPoint2d(xline_[0],yline_[0]);
    postcircle_[2].center_=DPoint2d(-xline_[0],yline_[0]);
    postcircle_[3].center_=DPoint2d(-xline_[0],-yline_[0]);

    /** 守门员处的几个关键位置*/
    oppGoal_[GOAL_UPPER]    = DPoint(xline_[0],100);
    oppGoal_[GOAL_MIDUPPER] = DPoint(xline_[0],40);
    oppGoal_[GOAL_MIDDLE]   = DPoint(xline_[0],0);
    oppGoal_[GOAL_MIDLOWER] = DPoint(xline_[0],-40);
    oppGoal_[GOAL_LOWER]    = DPoint(xline_[0],-100);

    ourGoal_[GOAL_UPPER]    = DPoint(xline_[6],100);
    ourGoal_[GOAL_MIDUPPER] = DPoint(xline_[6],40);
    ourGoal_[GOAL_MIDDLE]   = DPoint(xline_[6],0);
    ourGoal_[GOAL_MIDLOWER] = DPoint(xline_[6],-40);
    ourGoal_[GOAL_LOWER]    = DPoint(xline_[6],-100);


    our_penaltyarea_[0] = DPoint(xline_[6],yline_[1]);  //(-900,217)
    our_penaltyarea_[1] = DPoint(xline_[4],yline_[1]);  //(-675,217)
    our_penaltyarea_[2] = DPoint(xline_[4],yline_[4]);  //(-675,-217)
    our_penaltyarea_[3] = DPoint(xline_[6],yline_[4]);  //(-900,-217)

    opp_penaltyarea_[0] = DPoint(xline_[0],yline_[1]);
    opp_penaltyarea_[1] = DPoint(xline_[2],yline_[1]);
    opp_penaltyarea_[2] = DPoint(xline_[2],yline_[4]);
    opp_penaltyarea_[3] = DPoint(xline_[0],yline_[4]);

    our_goalarea_[0] = DPoint(xline_[6],yline_[2]);   //(-900,117)   我方小禁区
    our_goalarea_[1] = DPoint(xline_[5],yline_[2]);   //(-825,117)
    our_goalarea_[2] = DPoint(xline_[5],yline_[3]);   //(-825,-117)
    our_goalarea_[3] = DPoint(xline_[6],yline_[3]);   //(-900,-117)

    opp_goalarea_[0] = DPoint(xline_[0],yline_[2]);   //(900,117)    对方小禁区
    opp_goalarea_[1] = DPoint(xline_[1],yline_[2]);   //(825,117)
    opp_goalarea_[2] = DPoint(xline_[1],yline_[3]);   //(825,-117)
    opp_goalarea_[3] = DPoint(xline_[0],yline_[3]);   //(900,-117)
}

FieldInformation::FieldInformation(string infopath)
{}

/* 是否在场地内 */
bool FieldInformation::isInInterRect(DPoint world_pt,double shrink)
{
    return (world_pt.x_>xline_[6]-shrink &&
            world_pt.x_<xline_[0]+shrink &&
            world_pt.y_>yline_[5]-shrink &&
            world_pt.y_<yline_[0]+shrink);
}

/* 是否在场地外 */
bool FieldInformation::isInOuterRect(DPoint world_pt,double shrink)
{
    return (world_pt.x_<xline_[6]-shrink ||
            world_pt.x_>xline_[0]+shrink ||
            world_pt.y_<yline_[5]-shrink ||
            world_pt.y_>yline_[0]+shrink);
}

/* 是否在对方半场 */
bool FieldInformation::isOppField(DPoint world_pt)
{
    bool rtvl = false;
    static bool inoppfield = false;

    if(!inoppfield && world_pt.x_ > 0)
    {
        inoppfield  =  true;
    }
    else if(inoppfield && world_pt.x_ >  -simLOCATIONERROR)
    {
        inoppfield =  true;
    }
    else
    {
        inoppfield =  false;
    }
    rtvl = inoppfield;

    return rtvl;
}

/* 是否在己方半场*/
bool FieldInformation::isOurField(DPoint world_pt)
{
    bool rtvl = false;
    static bool inourfield = false;

    if(!inourfield && world_pt.x_ < 0)
    {
        inourfield  =  true;
    }
    else if(inourfield && world_pt.x_ < simLOCATIONERROR)
    {
        inourfield =  true;
    }
    else
    {
        inourfield =  false;
    }
    rtvl =  inourfield;

    return rtvl;
}

/* 是否在对方大禁区 */
bool FieldInformation::isOppPenalty(DPoint world_pt)
{
    bool rtvl = false;
    static bool inopppenalty = false;

    if(!inopppenalty)
    {
        if(world_pt.x_ > opp_penaltyarea_[1].x_ && world_pt.x_ < opp_penaltyarea_[0].x_ &&
           world_pt.y_ > opp_penaltyarea_[2].y_ && world_pt.y_ < opp_penaltyarea_[1].y_)
        {
            inopppenalty = true;
        }
        else
            inopppenalty = false;
    }
    else if(inopppenalty)
    {
        if(world_pt.x_ > (opp_penaltyarea_[1].x_ - simLOCATIONERROR) && world_pt.x_ < (opp_penaltyarea_[0].x_ + simLOCATIONERROR) &&
           world_pt.y_ > (opp_penaltyarea_[2].y_ - simLOCATIONERROR) && world_pt.y_ < (opp_penaltyarea_[1].y_ + simLOCATIONERROR))
        {
            inopppenalty = true;
        }
        else
            inopppenalty = false;
    }

    rtvl =  inopppenalty;
    return rtvl;
}

/* 是否在我方大禁区 */
bool FieldInformation::isOurPenalty(DPoint world_pt)
{
    bool rtvl = false;
    static bool inourpenalty = false;

    if(!inourpenalty)
    {
        if(world_pt.x_ < our_penaltyarea_[1].x_ && world_pt.x_ > our_penaltyarea_[0].x_ &&
           world_pt.y_ > our_penaltyarea_[2].y_ && world_pt.y_ < our_penaltyarea_[1].y_)
        {
            inourpenalty = true;
        }
        else
            inourpenalty = false;
    }
    else if(inourpenalty)
    {
        if(world_pt.x_ < (our_penaltyarea_[1].x_ +simLOCATIONERROR) && world_pt.x_ > (our_penaltyarea_[0].x_ -simLOCATIONERROR) &&
           world_pt.y_ > (our_penaltyarea_[2].y_-simLOCATIONERROR) &&  world_pt.y_ < (our_penaltyarea_[1].y_+simLOCATIONERROR))
        {
            inourpenalty = true;
        }
        else
            inourpenalty = false;
    }

    rtvl =  inourpenalty;
    return rtvl;
}

/* 是否在对方小禁区 */
bool FieldInformation::isOppArea(DPoint world_pt)
{
    bool rtvl = false;
    static bool inopparea = false;

    if(!inopparea)
    {
        if(world_pt.x_ > opp_goalarea_[1].x_ && world_pt.x_ < opp_goalarea_[0].x_ &&
           world_pt.y_ > opp_goalarea_[2].y_ && world_pt.y_ < opp_goalarea_[1].y_)
        {
            inopparea = true;
        }
        else
            inopparea = false;
    }
    else if(inopparea)
    {
        if(world_pt.x_ > (opp_goalarea_[1].x_ - simLOCATIONERROR) && world_pt.x_ < (opp_goalarea_[0].x_ + simLOCATIONERROR) &&
           world_pt.y_ > (opp_goalarea_[2].y_-simLOCATIONERROR) && world_pt.y_ < (opp_goalarea_[1].y_+simLOCATIONERROR))
        {
            inopparea = true;
        }
        else
            inopparea = false;
    }

    rtvl =  inopparea;
    return rtvl;
}

/* 是否在我方小禁区 */
bool FieldInformation::isOurArea(DPoint world_pt)
{
    bool rtvl = false;
    static bool inourarea = false;

    if(!inourarea)
    {
        if(world_pt.x_ < our_goalarea_[1].x_ && world_pt.x_ > our_goalarea_[0].x_ &&
           world_pt.y_ > our_goalarea_[2].y_ && world_pt.y_ < our_goalarea_[1].y_)
        {
            inourarea = true;
        }
        else
            inourarea = false;
    }
    else if(inourarea)
    {
        if(world_pt.x_ < (our_goalarea_[1].x_+simLOCATIONERROR) && world_pt.x_ > (our_goalarea_[0].x_ -simLOCATIONERROR) &&
           world_pt.y_ > (our_goalarea_[2].y_-simLOCATIONERROR) &&  world_pt.y_ < (our_goalarea_[1].y_+simLOCATIONERROR))
        {
            inourarea = true;
        }
        else
        {
            inourarea = false;
        }
    }

    rtvl =  inourarea;
    return rtvl;
}
