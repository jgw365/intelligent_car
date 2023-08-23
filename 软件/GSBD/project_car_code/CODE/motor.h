#ifndef _motor_h
#define _motor_h
#include "zf_common_headfile.h"

//电机引脚
//#define DIR_1  D14
//#define DIR_2  D15
//#define PWM_1  PWM1_MODULE0_CHA_D12
//#define PWM_2  PWM1_MODULE0_CHB_D13
//#define DIR_3  D1
//#define DIR_4  D0
//#define PWM_3     PWM2_MODULE3_CHB_D3
//#define PWM_4    PWM2_MODULE3_CHA_D2

#define DIR_4               (D0 )
#define PWM_4               (PWM2_MODULE3_CHA_D2)

#define DIR_3               (D1 )
#define PWM_3               (PWM2_MODULE3_CHB_D3)

#define DIR_2               (D15 )
#define PWM_2              (PWM1_MODULE0_CHB_D13)

#define DIR_1               (D14 )
#define PWM_1               (PWM1_MODULE0_CHA_D12)

//运动转换 决定角速度
#define k_r_x_y 1//(0.18/2+0.21/2) //500

//电机结构体
typedef struct{
	float vel_ex;   //期望速度
	float vel_ac;   //实际速度
	float duty; 
	float dis;//距离
	float duty_p;
//可以参考上交代码 加个模式选择	
}_Motor;
extern _Motor motor1, motor2, motor3, motor4;
extern float error1,error2,error3_,error4;

void turn_control();
void motor_init(void);
void open_loop_motor_set();
void pid_motor_set();
void motor_set(float motor1_duty,float motor2_duty,float motor3_duty,float motor4_duty);
void wheel_control(float x,float y,float w);
#endif