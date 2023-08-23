
#ifndef _servo_h
#define _servo_h
#include "zf_common_headfile.h"

#define SERVO1_MOTOR_PWM             (PWM4_MODULE3_CHA_C31)//(PWM4_MODULE3_CHA_C31)                                     //PWM2_MODULE1_CHA_C8              9v   下舵机
#define SERVO3_MOTOR_PWM             (PWM2_MODULE0_CHA_C6)                          // 定义上臂舵机对应引脚             转盘  7.4v
#define SERVO2_MOTOR_PWM             (PWM4_MODULE2_CHA_C30)                          // 定义下臂舵机对应引脚             上舵机   7.4v

#define SERVO_MOTOR_FREQ            (50 )                                           // 定义主板上舵机频率  请务必注意范围 50-300
#define SERVO_MOTOR_L_MAX           (30 )                                           // 定义主板上舵机活动范围 角度
#define SERVO_MOTOR_R_MAX           (115)                                           // 定义主板上舵机活动范围 角度


#define SERVO_MOTOR_DUTY180(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90))
#define SERVO_MOTOR_DUTY360(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*((float)(x)/1000.0))    //单位：us
#define SERVO_MOTOR_DUTY360_(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/180.0)) //单位：角度
	
#define servo2_error -500  //主车
//#define servo2_error -20  //副车
//#define servo3_error 61   //1号斗
#define servo3_error 28   //2号斗
#define delayx 0                 //
#define delayy 6  
#define two_error -10   //副车
extern float servo1_motor_duty;                                                  // 舵机动作角度
extern float servo1_motor_dir ;                                                      // 舵机动作状态
extern int servo_temp[6];
extern int servo_temp1,servo_temp2,servo_temp3;
void core_servo_move(int dir);
void unload(int dir);
void servo_move1();
void servo_move2();
void servo_init();
extern bool taking_flag;
void servo_back();
//void pick();
void pick_1();
void pick_2();
void pick_up1();
void pick_up2();
void put_down();
void put_up();
void ready_core(int dir);
extern int servo_temp_[6];
extern int my_arry[6];
#if (SERVO_MOTOR_FREQ<50 || SERVO_MOTOR_FREQ>300)
    #error "SERVO_MOTOR_FREQ ERROE!"
#endif


#endif