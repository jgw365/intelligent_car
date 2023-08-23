#include "motor.h"

float error1=0,error2=0,error3_=0,error4=0;
//左前为1号 其余顺时针
_Motor motor1 = {.vel_ex = 0,\
                 .vel_ac = 0,\
                 .duty=0,\
				 .dis=0,\
				 .duty_p=0};
_Motor motor2 = {.vel_ex = 0,\
                 .vel_ac = 0,\
                 .duty=0,\
				 .dis=0,\
				 .duty_p=0};
_Motor motor3 = {.vel_ex = 0,\
                 .vel_ac = 0,\
                 .duty=0,\
				 .dis=0,\
				 .duty_p=0};
_Motor motor4 = {.vel_ex = 0,\
                 .vel_ac = 0,\
                 .duty=0,\
				 .dis=0,\
				.duty_p=0}; 

void motor_init(void){
	  // GPIO 初始化为输出 默认上拉输出高
	  // PWM 通道初始化频率 17KHz 占空比初始为 0
	gpio_init(DIR_1, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            
    pwm_init(PWM_1, 17000, 0);                                                  
    gpio_init(DIR_2, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            
    pwm_init(PWM_2, 17000, 0);                                                  
    gpio_init(DIR_3, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            
    pwm_init(PWM_3, 17000, 0);                                                  
    gpio_init(DIR_4, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            
    pwm_init(PWM_4, 17000, 0);
}
//电机开环
void open_loop_motor_set(){
//get_wheel_vel();
//前馈参数 加了润滑脂或者改了硬件需要重新测定 
// 0.0078 接近线性
motor_set(2000,2000,2000,2000);
}
//pid速度闭环控制
void pid_motor_set(){
	float k=1;//石油大学为1 考虑测试
//	float kpid[3];
//	//-10-----10
//	if(motor1.vel_ac < 10 && motor1.vel_ac > -10)
//	{
//		kpid[0] = pid->kpid0[0];
//		kpid[1] = pid->kpid0[1];
//		kpid[2] = pid->kpid0[2];
//	}
//	//-20----20
//	else if(enc_vel < motor_vel_1 && enc_vel > -motor_vel_1)
//	{
//		kpid[0] = pid->kpid1[0];
//		kpid[1] = pid->kpid1[1];
//		kpid[2] = pid->kpid1[2];
//	}
//	//-30000---30000
//	else if(enc_vel < motor_vel_2 && enc_vel > -motor_vel_2)
//	{
//		kpid[0] = pid->kpid2[0];
//		kpid[1] = pid->kpid2[1];
//		kpid[2] = pid->kpid2[2];
//	}
//	else
//	{
//		kpid[0] = pid->kpid3[0];
//		kpid[1] = pid->kpid3[1];
//		kpid[2] = pid->kpid3[2];
//	}
	
	
	error1 = motor1.vel_ex - motor1.vel_ac;
	error2 = motor2.vel_ex - motor2.vel_ac;
	error3_ = motor3.vel_ex - motor3.vel_ac;
	error4 = motor4.vel_ex - motor4.vel_ac;
	motor1.duty += increment_pid(&pid1, error1);
	motor2.duty += increment_pid(&pid2, error2);
	motor3.duty += increment_pid(&pid3, error3_);
	motor4.duty += increment_pid(&pid4, error4);
	//show_debug=motor1.duty-motor1.duty_p-130*(error1-pid1.pre_pre_error)-12.4*error1;
	motor1.duty=limit(motor1.duty,30000);
	motor2.duty=limit(motor2.duty,30000);
	motor3.duty=limit(motor3.duty,30000);
	motor4.duty=limit(motor4.duty,30000);
	//motor_set(motor1.duty,motor2.duty,motor3.duty,motor4.duty);
	motor_set(((motor1.duty*k)+((1-k)*motor1.duty_p))/*+motor1.vel_ex/0.0078*/,\
		((motor2.duty*k)+((1-k)*motor2.duty_p))/*+motor2.vel_ex/0.0078*/,((motor3.duty*k)+((1-k)*motor3.duty_p))/*+motor3.vel_ex/0.0078*/,\
		((motor4.duty*k)+((1-k)*motor4.duty_p))/*+motor4.vel_ex/0.0078*/);
//	//motor_set(0,0,0,0);
	motor1.duty_p=((motor1.duty*k)+((1-k)*motor1.duty_p));
	motor2.duty_p=((motor2.duty*k)+((1-k)*motor2.duty_p));
	motor3.duty_p=((motor3.duty*k)+((1-k)*motor3.duty_p));
	motor4.duty_p=((motor4.duty*k)+((1-k)*motor4.duty_p));
}
void turn_control()
{	
	float w = turn_Yaw();
	w=limit(w,10);
	wheel_control(0,0,w);
}
//控制车运动 输入为 xyw三个方向的期望速度
void wheel_control(float y,float x,float w)
{
	//在此添加角度闭环
	w1=turn_Yaw();
    //前馈   
	motor1.vel_ex = (float)(y + x + k_r_x_y*(w+w1) ); //一号轮 左前轮
	motor2.vel_ex = (float)(-y + x - k_r_x_y*(w+w1)); //二号轮 右前轮

	motor3.vel_ex = (float)(y + x - k_r_x_y*(w+w1)); //三号轮 右后轮
	motor4.vel_ex = (float)(-y + x +k_r_x_y*(w+w1)); //四号轮 左后轮

}
//给四个电机pwm
void motor_set(float motor1_duty1,float motor2_duty1,float motor3_duty1,float motor4_duty1)
{
int32 motor1_duty = (int32) motor1_duty1;
int32 motor2_duty = (int32) motor2_duty1;
int32 motor3_duty = (int32) motor3_duty1;
int32 motor4_duty = (int32) motor4_duty1;

if ( motor1_duty>=0){
	  gpio_set_level(DIR_1,0); 
    pwm_set_duty(PWM_1, motor1_duty);
}
	else{gpio_set_level(DIR_1,1);
    pwm_set_duty(PWM_1,-motor1_duty);
	}
	if (motor2_duty>=0){
		gpio_set_level(DIR_2,0);
    pwm_set_duty(PWM_2,motor2_duty);
}
	else{gpio_set_level(DIR_2,1);
    pwm_set_duty(PWM_2,-motor2_duty);
	}
	if (motor3_duty>=0){
		gpio_set_level(DIR_3,0);
    pwm_set_duty(PWM_3,motor3_duty);
}
	else{gpio_set_level(DIR_3,1);
    pwm_set_duty(PWM_3,-motor3_duty);
	}
	if (motor4_duty>=0){
		gpio_set_level(DIR_4,0);
    pwm_set_duty(PWM_4,motor4_duty);
}
	else{gpio_set_level(DIR_4,1);
    pwm_set_duty(PWM_4,-motor4_duty);
	}
}

//一点点给角度期望值
//void turn_angle(float angle)
//{
//	if(angle > 0)
//	{
//		for(int16_t i = 0;i<angle;i++)
//		{
//			IMU.exp_dir -= 1;
//			systick_delay_ms(5);
//		}
//	}
//	else if(angle < 0)
//	{
//		for(int16_t i = 0;i<-angle;i++)
//		{
//			IMU.exp_dir += 1;
//			systick_delay_ms(5);
//		}
//	}
//	
//}
//	



