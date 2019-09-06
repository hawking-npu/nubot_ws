#include <nubot/nubot_control/passing.h>
#include <cstdlib>

using namespace std;
using namespace nubot;

Passing::Passing()
{
    init();
}

Passing::Passing(World_Model_Info &_world_model, Plan &_plan, DribbleState &_dribble)
{
    world_model_ = & _world_model;
    m_plan_ = & _plan;
    m_dribble_ = & _dribble;

    init();
    //1-red 2-white 3-blue 4-yellow
    //level1 1-red
    //level2 1-red 4-yellow
    //level3 1-yellow 2-blue 4-red
    //level4 1-white 2-yellow 3-red 4-blue
}

Passing::~Passing()
{
}

void Passing::process()
{
    if(level == 1)
    {
        if(have_kick[1]==false)
        {
            process_level(1, RED);
        }
        else
        {
            m_plan_->m_behaviour_.clearBehaviorState();
        }
    }
    else if(level == 2)
    {
        if(have_kick[1]==false)
        {
            process_level(1, RED);
        }
        else if(have_kick[4]==false)
        {
            process_level(4, YELLOW);
        }
        else
        {
            m_plan_->m_behaviour_.clearBehaviorState();
        }
    }
    else if(level == 3)
    {
        if(have_kick[1]==false)
        {
            process_level(1, YELLOW);
        }
        else if(have_kick[2]==false)
        {
            process_level(2, BLUE);
        }
        else if(have_kick[4]==false)
        {
            process_level(4, RED);
        }
        else
        {
            m_plan_->m_behaviour_.clearBehaviorState();
        }
    }
    else if(level == 4)
    {
        if(have_kick[1]==false)
        {
            process_level(1, WHITE);
        }
        else if(have_kick[2]==false)
        {
            process_level(2, YELLOW);
        }
        else if(have_kick[3]==false)
        {
            process_level(3, RED);
        }
        else if(have_kick[4]==false)
        {
            process_level(4, BLUE);
        }
        else
        {
            m_plan_->m_behaviour_.clearBehaviorState();
        }
    }
}

void Passing::process_level(int ball_id_, DishColor color_)    //every ball kick and move process
{
    if(m_dribble_->is_dribble_)
    {
        flag_move2catch = false;
        ball_know[ball_id_].seen_time_ = 0;
        ball_know[ball_id_].ball_seen_ = robot_pos_;
		move2kick(color_);
        if(flag_move2kick && fabs((dish_[color_]-robot_pos_).angle().radian_-robot_ori_.radian_)<3.0*DEG2RAD && velzero())
        {
            checktime_++;
			if(checktime_>=checktime)
			{
				kick_enable_ = true;
			}
        }
        else
        {
            checktime_ = 0;
        }
    }
	else if(kick_enable_)
    {
        checktime_ = 0;
        have_kick[ball_id_] = true;
        kick_enable_ = false;
        flag_face2kick = false;
        flag_move2kick = false;
    }
    else //catch ball
    {
        if(ball_know[ball_id_].seen_time_>maxseentime)
        {
            m_plan_->move2Positionwithobs_noball(DPoint(move_x_,ball_[ball_id_].y_));
            m_plan_->positionAvoidObs(ball_[ball_id_]);
        }
        else
        {
            if((level==3 && ball_id_==1) || (level==4 && (ball_id_==1 || ball_id_==2 || ball_id_==3)))
            {
                if(robot_pos_.distance(DPoint(move_x_,ball_know[ball_id_].ball_seen_.y_)) < 10.0)
                {
                    flag_move2catch = true;
                }
                if(flag_move2catch == false)
                {
                    move2catchball(ball_know[ball_id_].ball_seen_);
                }
                else
                {
                    m_plan_->catchMotionlessBall(ball_know[ball_id_].ball_seen_);
                }
            }
            else
            {
                m_plan_->catchMotionlessBall(ball_know[ball_id_].ball_seen_);
            }
        }
    }
}

void Passing::move2kick(DishColor color_)    //isdribble. move to the position of kick
{
    ROS_INFO("move2kick");
    DPoint target = kick_[color_];
    DPoint br = target - robot_pos_;
    m_plan_->positionAvoidObs2(br.angle().radian_, MAXW/2.0, 5.0*DEG2RAD);
    if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD)
    {
        flag_face2kick = true;
    }
    if(flag_face2kick)
    {
        m_plan_->move2Positionwithobs_noball(target);
		if(robot_pos_.distance(target) < 10.0)
		{
			flag_move2kick = true;
		}
	}
    if(flag_face2kick && flag_move2kick)
    {
        m_plan_->m_behaviour_.clearBehaviorState();
        br = dish_[color_] - robot_pos_;
        m_plan_->positionAvoidObs2(br.angle().radian_, MAXW/2.0, 3.0*DEG2RAD);
    }
}

void Passing::move2catchball(DPoint target)
{
    ROS_INFO("move2catchball");
    //m_plan_->move2Positionwithobs_noball(DPoint((robot_pos_.x_>move_x_)?move_x_:robot_pos_.x_,target.y_));
    m_plan_->move2Positionwithobs_noball(DPoint(move_x_,target.y_));
    m_plan_->positionAvoidObs(target);
}

void Passing::update()
{
    ROS_INFO("update");
    robot_pos_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getLocation();
    robot_ori_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getHead();
    ball_pos_  = world_model_->BallInfo_[world_model_->AgentID_-1].getGlobalLocation();
    world_model_->Obstacles_.clear();
    world_model_->Opponents_.clear();

    if(ball_pos_.x_>=-250.0 && ball_pos_.x_ <= move_x_)
    {
        for(int i=1; i<=4; ++i)
        {
            if(ball_pos_.y_ >= (dish_y[i]+dish_y[i+1])/2)
            {
                ball_know[i].ball_seen_ = ball_pos_;
                ball_know[i].seen_time_ = 0;
                break;
            }
        }
    }
    if(level == 1)
    {
        ball_know[1].ball_seen_ = ball_pos_;
        ball_know[1].seen_time_ = 0;
    }
    ROS_INFO("ball_pos:(%f,%f)", ball_pos_.x_, ball_pos_.y_);
    ROS_INFO("maxseentime = %d", maxseentime);
    for(int i=1; i<5; ++i)
    {
        ball_know[i].seen_time_++;
        ROS_INFO("ball[%d]: time=%d, pos:(%f,%f)", i, ball_know[i].seen_time_, ball_know[i].ball_seen_.x_, ball_know[i].ball_seen_.y_);
    }
    for(int i=1; i<5; ++i)
    {
        ROS_INFO("havekicked[%d]=%d", i, have_kick[i]);
    }
    ROS_INFO("face2kick=%d", flag_face2kick);
    ROS_INFO("move22kick=%d", flag_move2kick);
    ROS_INFO("kick_enable=%d", kick_enable_);
	ROS_INFO("dribble=%d", m_dribble_->is_dribble_);
	ROS_INFO("checktime_=%d", checktime_);
}

void Passing::init()
{
    ROS_INFO("init");
    dish_y[0] = 280.0;
    dish_y[1] = 120.0;
    dish_y[2] = 40.0;
    dish_y[3] = -40.0;
    dish_y[4] = -120.0;
    dish_y[5] = -280.0;

    dish_x_ = 165.0;
    kick_x_ = 0.0;
    ball_x_ = -165.0;

    dish_[RED]    = DPoint(dish_x_, dish_y[1]);
    dish_[BLUE]   = DPoint(dish_x_, dish_y[2]);
    dish_[WHITE]  = DPoint(dish_x_, dish_y[3]);
    dish_[YELLOW] = DPoint(dish_x_, dish_y[4]);

    ball_[1] = DPoint(ball_x_, dish_y[1]);
    ball_[2] = DPoint(ball_x_, dish_y[2]);
    ball_[3] = DPoint(ball_x_, dish_y[3]);
    ball_[4] = DPoint(ball_x_, dish_y[4]);

    kick_[RED]    = DPoint(kick_x_, dish_[RED].y_);
    kick_[BLUE]   = DPoint(kick_x_, dish_[BLUE].y_);
    kick_[WHITE]  = DPoint(kick_x_, dish_[WHITE].y_);
    kick_[YELLOW] = DPoint(kick_x_, dish_[YELLOW].y_);

    for(int i=0; i<5; ++i)
    {
        ball_know[i].ball_seen_ = DPoint(0.0, 0.0);
        ball_know[i].seen_time_ = 10000;
    }

    level = 0;
    kick_enable_ = false;
    memset(have_kick, false, sizeof(have_kick));
    flag_face2kick = false;
    flag_move2kick = false;
    flag_move2catch = false;

    move_x_ = -50.0;
	checktime_ = 0;
}

bool Passing::velzero()
{
    if((int)(m_plan_->m_behaviour_.app_vx_*20) == 0)
    {
        if((int)(-m_plan_->m_behaviour_.app_vy_*20) == 0)
        {
            if((int)(m_plan_->m_behaviour_.app_w_*1400) == 0)
            {
                return true;
            }
        }
    }
    return false;
}
