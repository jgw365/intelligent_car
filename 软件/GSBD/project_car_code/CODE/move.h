#ifndef _move_h
#define _move_h
#include "zf_common_headfile.h"

void run_straight(float velocity,float dis_cm);
//转一个固定的角度
void go_next(float next_x,float next_y);
void model_2_keep();
void run_translation(float velocity_x,float velocity_y,float dis_cm_x,float dis_cm_y);
void correct_move();
#endif