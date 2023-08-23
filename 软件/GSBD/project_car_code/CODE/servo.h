
#ifndef _servo_h
#define _servo_h
#include "zf_common_headfile.h"

#define SERVO1_MOTOR_PWM             (PWM4_MODULE3_CHA_C31)//(PWM4_MODULE3_CHA_C31)                                     //PWM2_MODULE1_CHA_C8              9v   �¶��
#define SERVO3_MOTOR_PWM             (PWM2_MODULE0_CHA_C6)                          // �����ϱ۶����Ӧ����             ת��  7.4v
#define SERVO2_MOTOR_PWM             (PWM4_MODULE2_CHA_C30)                          // �����±۶����Ӧ����             �϶��   7.4v

#define SERVO_MOTOR_FREQ            (50 )                                           // ���������϶��Ƶ��  �����ע�ⷶΧ 50-300
#define SERVO_MOTOR_L_MAX           (30 )                                           // ���������϶�����Χ �Ƕ�
#define SERVO_MOTOR_R_MAX           (115)                                           // ���������϶�����Χ �Ƕ�


#define SERVO_MOTOR_DUTY180(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90))
#define SERVO_MOTOR_DUTY360(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*((float)(x)/1000.0))    //��λ��us
#define SERVO_MOTOR_DUTY360_(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/180.0)) //��λ���Ƕ�
	
#define servo2_error -500  //����
//#define servo2_error -20  //����
//#define servo3_error 61   //1�Ŷ�
#define servo3_error 28   //2�Ŷ�
#define delayx 0                 //
#define delayy 6  
#define two_error -10   //����
extern float servo1_motor_duty;                                                  // ��������Ƕ�
extern float servo1_motor_dir ;                                                      // �������״̬
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