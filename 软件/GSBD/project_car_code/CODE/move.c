#include "move.h"

void run_translation(float velocity_x,float velocity_y,float dis_cm_x,float dis_cm_y)
{
	  
	Car.ex_car_x=dis_cm_x;//有待调整
	Car.ex_car_y=dis_cm_y;
	Car.ex_vel_y=0;
	Car.ex_vel_x=0;
	Car.ex_car_yaw=0;
//	start_parallel_flag=1;	
	fin_flag=0;
//	while(1)
//	{
//	if (!start_parallel_flag)
//	{	
//		Car.ex_car_x_p=Car.ex_car_x;
//		Car.ex_car_y_p=Car.ex_car_y;
//		break;
//	}
//	}
	return;
}

//配合模式2
void go_next(float next_x,float next_y)
{
	static float next_x_p=0;
	static float next_y_p=0;
	run_translation(0,0,next_x,next_y);
	//duty_zero();
	next_x_p=next_x;
	next_y_p=next_y;
}

void model_2_keep()
{   
	map_read[0][0]=30;map_read[0][1]=30;
	map_read[1][0]=100;map_read[1][1]=50;
	map_read[2][0]=0;map_read[2][1]=90;
	map_read[3][0]=10;map_read[3][1]=30;
	//现在按30 到时候 往主板传点数
	for (int m=0;m<max_pot;m++)
	{
		
	go_next(map_read[m][0],map_read[m][1]);
	}
	beep_boom(5,1);
	while(1){
	tft180_show_float(0,2*15,Car.car_x,6,6);
	tft180_show_float(0,4*15,Car.car_y,6,6);
	}
}
void correct_move()
{
	start_correct_flag=1;
//	while(1)
//	{
		tft180_show_string(0,1*lcd_wide,"x:");
		tft180_show_string(0,2*lcd_wide,"y:");
		tft180_show_float(30,1*lcd_wide,img_error[0],5,5);
		tft180_show_float(30,2*lcd_wide,img_error[1],5,5);
		tft180_show_string(0,3*lcd_wide,"vx:");
		tft180_show_string(0,4*lcd_wide,"vy:");
		tft180_show_float(30,3*lcd_wide,Car.ex_vel_x,5,5);
		tft180_show_float(30,4*lcd_wide,Car.ex_vel_y,5,5);
		//tft180_show_int(0,5*lcd_wide,cnt_,7);
	system_delay_ms(5);
//	}
}

