#include "servo.h"
float servo1_motor_duty = 90.0;                                                  // 舵机动作角度
float servo1_motor_dir = 1;                                                      // 舵机动作状态
int servo_temp[6]={0,90,180,270,0,180};// 新版
int servo_temp1=50,servo_temp2=1500,servo_temp3=1760;
int my_arry[6]={0,4,3,2,1,5};//新版
int servo_temp_[6]={15,15,45,15,15,15};
void ready_core(int dir)
{
	pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo3_error));
}
void unload(int dir)//扔下 对应位置的图片
{
	
	
	if(dir==0)//ready_core(0)
	{
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[dir]+25+servo3_error));
		system_delay_ms(50);
		gpio_set_level(C5,1);
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[dir]+90+servo3_error));
		system_delay_ms(100);
		gpio_set_level(C5,0);
		system_delay_ms(10);
		
	}
	else if(dir==1)//ready_core(1)
	{
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+15+servo3_error));
		system_delay_ms(50);
		gpio_set_level(C7,1);
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+25+servo3_error));
		system_delay_ms(50);
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+5+servo3_error));
		system_delay_ms(50);
		gpio_set_level(C7,0);
		system_delay_ms(10);
		
	}
	else if(dir==2)
	{
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+100+servo3_error));
		system_delay_ms(100);
		gpio_set_level(C10,1);
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+120+servo3_error));
		system_delay_ms(50);
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+100+servo3_error));
		system_delay_ms(100);
		gpio_set_level(C10,0);
		system_delay_ms(10);
	}
	else if(dir==3)
	{
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+15+servo3_error));
		system_delay_ms(50);
		gpio_set_level(C11,1);
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+25+servo3_error));
		system_delay_ms(50);
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+5+servo3_error));
		system_delay_ms(50);
		gpio_set_level(C11,0);
		system_delay_ms(10);
	}
	else if(dir==4)
	{
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+25+servo3_error));//老版15
		system_delay_ms(50);
		gpio_set_level(C12,1);
		pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+90+servo3_error));//老版25
		system_delay_ms(50);
		//pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[0]+5+servo3_error));
		system_delay_ms(50);
		gpio_set_level(C12,0);
		system_delay_ms(10);
	}
}
void servo_init()
{ servo_temp2=1350+servo2_error+650;
    pwm_init(SERVO1_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);
	pwm_init(SERVO2_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);
	pwm_init(SERVO3_MOTOR_PWM, SERVO_MOTOR_FREQ, 0);
	gpio_init(C13, GPO, GPIO_LOW, GPO_PUSH_PULL);
	pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
	pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1+two_error));
	pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(45+servo3_error));
	gpio_init(C5, GPO, GPIO_LOW, GPO_PUSH_PULL);
	gpio_init(C7, GPO, GPIO_LOW, GPO_PUSH_PULL);
	gpio_init(C10, GPO, GPIO_LOW, GPO_PUSH_PULL);
	gpio_init(C11, GPO, GPIO_LOW, GPO_PUSH_PULL);
	gpio_init(C12, GPO, GPIO_LOW, GPO_PUSH_PULL);
	core_servo_move(0);
	
}
void core_servo_move(int dir)   //旋转木盒到相应位置   （对应0-4 ！！！！！！！！！！！）
{
	pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(servo_temp[dir]+servo3_error));
	return ;
}
void servo_back()
{pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(90));
	pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(180));
}

void put_down()
{
	servo_temp2=1350+servo2_error+650;
	servo_temp1=118;
	pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
	pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
	return ;
}
void put_up()
{ //servo_temp2=1350+servo2_error+650;
//pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
	pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(50));
	pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
}

//void pick()
//{
//	while(servo_temp2>1400+servo2_error)
//	{
//		servo_temp2-=5;
//		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
//		system_delay_ms(2+delayx);
//	}
//	while(servo_temp1<110)
//	{
//		servo_temp1+=1;
//		pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
//		system_delay_ms(3+delayx);
//	}
//	while(servo_temp1>70)
//	{
//		 servo_temp1-=1;
//     pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
//     system_delay_ms(2+delayx);
//	}
//	
//}
void pick_1()
{
	while(servo_temp2>1400+servo2_error)
	{
		servo_temp2-=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(2+delayx);
	}
	while(servo_temp1<110)
	{
		servo_temp1+=1;
		pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		system_delay_ms(3+delayx);
	}
}
void pick_2()
{
	while(servo_temp1<110)
	{
		servo_temp1+=1;
		pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		system_delay_ms(3+delayx);
	}
	while(servo_temp1>70)
	{
		 servo_temp1-=1;
     pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
     system_delay_ms(2+delayx);
	}
}

void pick_up1()
{
	while (servo_temp2<2250+servo2_error)//2250
	{
		servo_temp2+=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(3+delayx);
	}
 while (servo_temp1>51)//
 {
	 servo_temp1-=1;
	 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
	 system_delay_ms(5+delayx+delayy);
 }
	while (servo_temp2<2450+servo2_error)
	{
		servo_temp2+=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(5+delayx+delayy);
	}
	 while (servo_temp1>40)
 {
	 servo_temp1-=1;
	 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
	 system_delay_ms(5+delayx+delayy);
 }
	system_delay_ms(100);
	gpio_set_level(C13,1);
	system_delay_ms(500);
 
	while (servo_temp1<50)
	{
		 servo_temp1+=1;
		 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		 	system_delay_ms(2+delayx);
	}
	while (servo_temp2>1350+servo2_error+650)
	{
		servo_temp2-=5;
	  pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
	  system_delay_ms(1+delayx);
	}
	gpio_set_level(C13,0);
	while (servo_temp1>45)
	{
		 servo_temp1-=1;
		 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		 	system_delay_ms(1+delayx);
	}	
}

void pick_up2()
{
	while (servo_temp2<2250+servo2_error)
	{
		servo_temp2+=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(3+delayx);
	}
	 while (servo_temp1>49)
 {
	 servo_temp1-=1;
	 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
	 system_delay_ms(5+delayx+delayy);
 }
	while (servo_temp2<2200+servo2_error)
	{
		servo_temp2+=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(5+delayx+delayy);
	}
 while (servo_temp1>36)//向里推
 {
	 servo_temp1-=1;
	 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
	 system_delay_ms(5+delayx);
 }
	while (servo_temp2<2380+servo2_error)
	{
		servo_temp2+=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(5+delayx);
	}
	system_delay_ms(100);
	gpio_set_level(C13,1);
	system_delay_ms(500);

	while (servo_temp1<50)//1舵机复位
	{
		 servo_temp1+=1;
		 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		 	system_delay_ms(1+delayx);
	}
	while (servo_temp2>1350+servo2_error+650)//2舵机复位
	{
		servo_temp2-=5;
	  pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
	  system_delay_ms(1+delayx);
	}
	gpio_set_level(C13,0);
	while (servo_temp1>40)//1舵机复位
	{
		 servo_temp1-=1;
		 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		 	system_delay_ms(1+delayx);
	}	 	 
	
}



//void servo_move1()//放入下面的框
//{
//	while(servo_temp2<1400+servo2_error)
//	{
//		servo_temp2+=5;
//		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
//		system_delay_ms(3+delayx);
//	}
//	while(servo_temp2>1400+servo2_error)
//	{
//		servo_temp2-=5;
//		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
//		system_delay_ms(3+delayx);
//	}
//	while(servo_temp1<110)
//	{
//		servo_temp1+=1;
//		pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
//		system_delay_ms(3+delayx);
//	}
//	while(servo_temp1>70)
//	{
//		 servo_temp1-=1;
//     pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
//     system_delay_ms(3+delayx);
//	}
//	while (servo_temp2<2250+servo2_error)//2250
//	{
//		servo_temp2+=5;
//		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
//		system_delay_ms(3+delayx);
//	}
// while (servo_temp1>54)
// {
//	 servo_temp1-=1;
//	 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
//	 system_delay_ms(5+delayx+delayy);
// }
//	while (servo_temp2<2400+servo2_error)
//	{
//		servo_temp2+=5;
//		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
//		system_delay_ms(5+delayx+delayy);
//	}
//	 while (servo_temp1>42)
// {
//	 servo_temp1-=1;
//	 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
//	 system_delay_ms(5+delayx+delayy);
// }
//	system_delay_ms(100);
//	gpio_set_level(C13,1);
//	system_delay_ms(500);
// 
//	//舵机复位
//	while (servo_temp1<50)//1舵机复位
//	{
//		 servo_temp1+=1;
//		 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
//		 	system_delay_ms(2+delayx);
//	}
//	gpio_set_level(C13,0);
//	while (servo_temp2>1350+servo2_error+650)//2舵机复位
//	{
//		servo_temp2-=5;
//	  pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
//	  system_delay_ms(1+delayx);
//	}
//	while (servo_temp1>45)//1舵机复位
//	{
//		 servo_temp1-=1;
//		 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
//		 	system_delay_ms(1+delayx);
//	}	 
//}
void servo_move2()//放入上面的框
{
	while(servo_temp2>1400+servo2_error)
	{
		servo_temp2-=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(3+delayx);
	}
	while(servo_temp1<110)
	{
		servo_temp1+=1;
		pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		system_delay_ms(3+delayx);
	}
	system_delay_ms(200);
	while(servo_temp1>70)
	{
		 servo_temp1-=1;
     pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
     system_delay_ms(3+delayx);
	}
	while (servo_temp2<2200+servo2_error)
	{
		servo_temp2+=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(3+delayx);
	}
 while (servo_temp1>38)
 {
	 servo_temp1-=1;
	 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
	 system_delay_ms(5+delayx);
 }
	while (servo_temp2<2450+servo2_error)
	{
		servo_temp2+=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(5+delayx);
	}
	system_delay_ms(100);
	gpio_set_level(C13,1);
	system_delay_ms(500);
	gpio_set_level(C13,0);

	while (servo_temp1<50)//1舵机复位
	{
		 servo_temp1+=1;
		 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		 	system_delay_ms(1+delayx);
	}
	while (servo_temp2>1350+servo2_error+650)//2舵机复位
	{
		servo_temp2-=5;
	  pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
	  system_delay_ms(1+delayx);
	}
	while (servo_temp1>40)//1舵机复位
	{
		 servo_temp1-=1;
		 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		 	system_delay_ms(1+delayx);
	}	 	 
}


void servo_move1()//·?è?????μ??ò
{
	while(servo_temp2<1400+servo2_error)
	{
		servo_temp2+=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(3+delayx);
	}
	while(servo_temp2>1400+servo2_error)
	{
		servo_temp2-=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(3+delayx);
	}
	while(servo_temp1<110)
	{
		servo_temp1+=1;
		pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		system_delay_ms(3+delayx);
	}
	while(servo_temp1>70)
	{
		 servo_temp1-=1;
     pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
     system_delay_ms(3+delayx);
	}
	while (servo_temp2<2250+servo2_error)//2250
	{
		servo_temp2+=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(3+delayx);
	}
 while (servo_temp1>54)
 {
	 servo_temp1-=1;
	 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
	 system_delay_ms(5+delayx+delayy);
 }
	while (servo_temp2<2400+servo2_error)
	{
		servo_temp2+=5;
		pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
		system_delay_ms(5+delayx+delayy);
	}
	 while (servo_temp1>42)
 {
	 servo_temp1-=1;
	 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
	 system_delay_ms(5+delayx+delayy);
 }
	system_delay_ms(100);
	gpio_set_level(C13,1);
	system_delay_ms(500);
 
	//???ú?′??
	while (servo_temp1<50)//1???ú?′??
	{
		 servo_temp1+=1;
		 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		 	system_delay_ms(2+delayx);
	}
	gpio_set_level(C13,0);
	while (servo_temp2>1350+servo2_error+650)//2???ú?′??
	{
		servo_temp2-=5;
	  pwm_set_duty(SERVO2_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360(servo_temp2));
	  system_delay_ms(1+delayx);
	}
	while (servo_temp1>45)//1???ú?′??
	{
		 servo_temp1-=1;
		 pwm_set_duty(SERVO1_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY180(servo_temp1));
		 	system_delay_ms(1+delayx);
	}	 
}

