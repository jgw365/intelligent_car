#ifndef _car_h
#define _car_h
#include "zf_common_headfile.h"
#include "roadplan.h"
//车况结构体
typedef struct{
	float car_x, car_y, car_yaw;//车当前x，y坐标和角度
	float vel_x,vel_y,vel_w;//三个方向的车速
	float ex_car_x, ex_car_y, ex_car_yaw;//车期望x，y坐标和角度
	float ex_vel_x,ex_vel_y,ex_vel_w;//三个方向的期望车速
	float straight,ex_straight;
	float ex_car_x_p,ex_car_y_p;
}_Car_state;
extern _Car_state Car;
extern int youkuang;
extern uint16 closer_x;
extern bool check_flag;
extern bool check_flag1;
extern bool scan_flag;
extern bool place_flag;
void cal_dis();
void car_status_update();
extern bool no_find_flag;
void test_pid();
void speed_up_down(float state,float up,float down,int stay);
void speeed_area_up(float now, float area, int stay);
uint8 speed_control(float state,float up,float down,int stay);

extern bool start_parallel_flag;
extern bool fin_flag;
extern bool state1ok_flag;
extern bool flag_nothong;
extern bool ahead_flag;
extern uint8_t work_state ;   
extern uint8_t work_state_p ;
extern uint8 scan_num_time;
void state_control(uint8 state);
void state_0();
void state_1();
void state_2();
void state_3();
void state_4();
void state_5();
//zean
void state_9(int x,int y);
void make_up_error(bool dir);
extern bool scan_get_flag;   //看到无框
extern bool scan_stop_flag;
extern bool flag6;
extern int flag7;
extern float real_angle;
extern int no_find;
//
void state_10();

extern uint32 time_before;
extern uint32 time_after;
extern float w;

extern float velocity_global_lim;
extern bool flag1;
extern bool flag2;
extern bool flag3;
extern bool flag4;
extern bool flag8;
extern int zean_flag;
extern bool adjust_car_jw_flag;
extern bool return_flag;
extern bool zean_dir;
extern float ac_vel_y;
extern float ac_vel_x;

extern  float vx;
extern  float vy;
extern float w1;
void duty_zero();
void plan_vel();
void beep_boom(float ms,float times);
void correct_plan_vel();

#endif