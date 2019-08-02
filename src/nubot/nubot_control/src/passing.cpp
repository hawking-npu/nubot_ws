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
        process_level(1, RED);
    }
    else if(level == 2)
    {
        process_level(1, RED);
        if(m_dribble_->is_dribble_==false && have_kick[1]==true)
        {
            can_dribble[4] = true;
            flag_face2kick = false;
        }
        if(can_dribble[4] == true)
        {
            process_level(4, YELLOW);
        }
    }
    else if(level == 3)
    {
        process_level(1, YELLOW);
        if(m_dribble_->is_dribble_==false && have_kick[1]==true)
        {
            can_dribble[2] = true;
            flag_face2kick = false;
        }
        if(can_dribble[2] == true)
        {
            process_level(2, BLUE);
        }
        if(m_dribble_->is_dribble_==false && have_kick[2]==true)
        {
            can_dribble[4] = true;
            flag_face2kick = false;
        }
        if(can_dribble[4] == true)
        {
            process_level(4, RED);
        }
    }
    else if(level == 4)
    {
        process_level(1, WHITE);
        if(m_dribble_->is_dribble_==false && have_kick[1]==true)
        {
            can_dribble[2] = true;
            flag_face2kick = false;
        }
        if(can_dribble[2] == true)
        {
            process_level(2, YELLOW);
        }
        if(m_dribble_->is_dribble_==false && have_kick[2]==true)
        {
            can_dribble[3] = true;
            flag_face2kick = false;
        }
        if(can_dribble[3] == true)
        {
            process_level(3, RED);
        }
        if(m_dribble_->is_dribble_==false && have_kick[3]==true)
        {
            can_dribble[4] = true;
            flag_face2kick = false;
        }
        if(can_dribble[4] == true)
        {
            process_level(4, BLUE);
        }
    }
}

void Passing::process_level(int ball_id_, DishColor color_)    //every ball kick and move process
{
    if(CatchBall((ball_know[ball_id_].seen_time_<maxseentime) ? ball_know[ball_id_].ball_seen_ : ball_[ball_id_]))
    {
        if(move2kick(color_))
        {
            if(m_dribble_->is_dribble_)
            {
                kick_enable_ = true;
                have_kick[ball_id_] = true;
            }
        }
    }
}

bool Passing::move2kick(DishColor color_)    //isdribble. move to the position of kick
{
    DPoint target = kick_[color_];
    DPoint br = target - robot_pos_;
    m_plan_->positionAvoidObs2(br.angle().radian_, MAXW, 3.0*DEG2RAD);
    if(fabs(br.angle().radian_-robot_ori_.radian_) < 5.0*DEG2RAD)
    {
        flag_face2kick = true;
    }
    if(flag_face2kick)
    {
        m_plan_->move2Positionwithobs_noball(target);
    }
    if(robot_pos_.distance(target) < 10.0)
    {
        m_plan_->m_behaviour_.clearBehaviorState();
        flag_face2kick = false;
        target = dish_[color_];
        br = target - robot_pos_;
        m_plan_->positionAvoidObs2(br.angle().radian_, MAXW, 3.0*DEG2RAD);
        if(fabs(br.angle().radian_-robot_ori_.radian_) < 3.0*DEG2RAD)
        {
            return true;
        }
    }
    return false;
}

bool Passing::CatchBall(DPoint target)
{
    if(m_dribble_->is_dribble_ != true)
    {
        ROS_INFO("Catch_Ball");
        DPoint br = target - robot_pos_;

        m_plan_->positionAvoidObs2(br.angle().radian_, MAXW, 3*DEG2RAD);
        if(robot_pos_.distance(target) < LIMITDRIBLLEDIS + 20.0)
        {
            if(fabs(br.angle().radian_-robot_ori_.radian_)<3*DEG2RAD)
            {
                m_plan_->move2Positionwithobs_noball(target);
            }
        }
        else
        {
            m_plan_->move2Positionwithobs_noball(target);
        }
    }
    return m_dribble_->is_dribble_;
}

void Passing::update()
{
    robot_pos_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getLocation();
    robot_ori_ = world_model_->RobotInfo_[world_model_->AgentID_-1].getHead();
    ball_pos_  = world_model_->BallInfo_[world_model_->AgentID_-1].getGlobalLocation();
    world_model_->Obstacles_.clear();
    world_model_->Opponents_.clear();

    if(ball_pos_.x_>=-250.0 && ball_pos_.x_ <= -50.0)
    {
        for(int i=4; i>=1; --i)
        {
            if(ball_pos_.y_ >= (dish_y[i]+dish_y[i+1])/2)
            {
                ball_know[i].ball_seen_ = ball_pos_;
                ball_know[i].seen_time_ = 0;
                break;
            }
        }
    }
    for(int i=0; i<5; ++i)
    {
        ball_know[i].seen_time_++;
    }

    if(m_dribble_->is_dribble_ == false)
    {
        kick_enable_ = false;
    }
}

void Passing::init()
{
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
        ball_know[i].seen_time_ = 0;
    }

    level = 0;
    kick_enable_ = false;
    memset(have_kick, false, sizeof(have_kick));
    memset(can_dribble, false, sizeof(can_dribble));
    flag_face2kick = false;
}
