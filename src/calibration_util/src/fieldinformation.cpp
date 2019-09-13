#include "fieldinformation.h"

using namespace nubot;

FieldInformation::FieldInformation()
{
     //ROBOCUP 1800*1200
//     xline_.push_back(900);
//     xline_.push_back(835);
//     xline_.push_back(685);
//     xline_.push_back(0);
//     xline_.push_back(-685);
//     xline_.push_back(-835);
//     xline_.push_back(-900);

//     yline_.push_back(600);
//     yline_.push_back(325);
//     yline_.push_back(175);
//     yline_.push_back(-175);
//     yline_.push_back(-325);
//     yline_.push_back(-600);
//  centercircle_.radius_=150;
//  centercircle_.center_=DPoint2d(0,0);


     //FIRA@Home 600*400
     xline_.push_back(300);
     xline_.push_back(260);
     xline_.push_back(235);
     xline_.push_back(0);
     xline_.push_back(-235);
     xline_.push_back(-260);
     xline_.push_back(-300);

     yline_.push_back(200);
     yline_.push_back(100);
     yline_.push_back(80);
     yline_.push_back(-80);
     yline_.push_back(-100);
     yline_.push_back(-200);

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

     centercircle_.radius_=60;
     centercircle_.center_=DPoint2d(0,0);


    /*
    x_white_line_.push_back(LineSegment(DPoint(900,-600),DPoint(900,600)));
    x_white_line_.push_back(LineSegment(DPoint(825,-175),DPoint(825,175)));
    x_white_line_.push_back(LineSegment(DPoint(675,-325),DPoint(675,325)));
    x_white_line_.push_back(LineSegment(DPoint(0,-600),DPoint(0,600)));
    x_white_line_.push_back(LineSegment(DPoint(-675,-325),DPoint(-675,325)));
    x_white_line_.push_back(LineSegment(DPoint(-825,-175),DPoint(-825,175)));
    x_white_line_.push_back(LineSegment(DPoint(-900,-600),DPoint(-900,600)));


    y_white_line_.push_back(LineSegment(DPoint(-900,-600),DPoint( 900,-600)));
    y_white_line_.push_back(LineSegment(DPoint(-900, 600),DPoint( 900, 600)));
    y_white_line_.push_back(LineSegment(DPoint(-900,-325),DPoint(-675,-325)));
    y_white_line_.push_back(LineSegment(DPoint( 900,-325),DPoint( 675,-325)));
    y_white_line_.push_back(LineSegment(DPoint(-900,-175),DPoint(-825,-175)));
    y_white_line_.push_back(LineSegment(DPoint( 900,-175),DPoint( 825,-175)));
    y_white_line_.push_back(LineSegment(DPoint(-900,175),DPoint(-825,175)));
    y_white_line_.push_back(LineSegment(DPoint( 900,175),DPoint( 825,175)));
    y_white_line_.push_back(LineSegment(DPoint(-900,325),DPoint(-675,325)));
    y_white_line_.push_back(LineSegment(DPoint( 900,325),DPoint( 675,325)));*/



    postcircle_.resize(4);
    for(size_t i=0; i< 4;i++)
         postcircle_[i].radius_=25;
    postcircle_[0].center_=DPoint2d(xline_[0],-yline_[0]);
    postcircle_[1].center_=DPoint2d(xline_[0],yline_[0]);
    postcircle_[2].center_=DPoint2d(-xline_[0],yline_[0]);
    postcircle_[3].center_=DPoint2d(-xline_[0],-yline_[0]);

    opp_goal_[0] = DPoint(xline_[6],yline_[3]);
    opp_goal_[0] = DPoint(xline_[6],yline_[2]);
    our_goal_[0] = DPoint(xline_[0],yline_[3]);
    our_goal_[0] = DPoint(xline_[0],yline_[2]);

    goalcircle_.resize(2);
    for(size_t i=0; i<2; i++)
        goalcircle_[i].radius_=60;
    goalcircle_[0].center_=DPoint2d(225.0,0.0);
    goalcircle_[1].center_=DPoint2d(-225.0,0.0);
}

FieldInformation::FieldInformation(string infopath)
{
	
}

bool 
FieldInformation::isInInterRect(DPoint world_pt,double shrink)
{
    return (world_pt.x_>xline_[6]-shrink &&
            world_pt.x_<xline_[0]+shrink &&
            world_pt.y_>yline_[5]-shrink &&
            world_pt.y_<yline_[0]+shrink);
}

bool
FieldInformation::isInOuterRect(DPoint world_pt,double shrink)
{
    return (world_pt.x_<xline_[6]-shrink ||
            world_pt.x_>xline_[0]+shrink ||
            world_pt.y_<yline_[5]-shrink ||
            world_pt.y_>yline_[0]+shrink);
}


bool 
FieldInformation::isOppfield(DPoint world_pt)
{
	return (world_pt.x_>0);
}

bool 
FieldInformation::isOurfield(DPoint world_pt)
{
	return (world_pt.x_<0);
}
