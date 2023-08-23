#include "test.h"
//跑多边形
//void test_polygen()
//{
//	bool flag = 0;
//	int yaw_angle = 30;
//	while(1)
//	{
//		if(flag == 0 )  
//		{	run_straight(20,80);
//			system_delay_ms(100);
//			flag = 1;
//		}
//		if(flag == 1)
//		{	turn_angle(30);
//			system_delay_ms(100);
//			flag = 0;
//			yaw_angle += 30;
//			if(yaw_angle == 360) yaw_angle = 0;	
//		}
//	}
//}
bool taking_flag=0;
int time_need;
void test_pid_debug()
{	//0--135  "pid debug system"
	static int8 page_j=0;
	wireless_uart_init();
	//tft180_full(RGB565_BLACK);
	uint8 key1,key1_p;
	uint8 key2,key2_p;
	uint8 key3,key3_p;
	uint8 key4,key4_p;
	uint8 sw1,sw1_p;
	uint8 sw2,sw2_p;
	int road=200;
//	uint8 temp1; 
//	uint8 temp2;
//	uint8 temp3;
//	uint8 temp4;
//	uint8 temp5;
//	uint8 temp6;
	bool flag=1;
	uint32 get_message=0;
    uint8 data_buffer_text[32];
	uint8    data_buffer_cul[32];
    uint8 data_lent;
	
	while(1)
	{    
		
//		temp1 = gpio_get_level(KEY1);
//		temp2 = gpio_get_level(KEY2);
//		temp3 = gpio_get_level(KEY3);
//		temp4 = gpio_get_level(KEY4);
//		temp5 = gpio_get_level(SW1);
//		temp6 = gpio_get_level(SW2);
//		//按键消抖
//		system_delay_ms(10);
//		if(temp1 == gpio_get_level(KEY1))  key1 = temp1;
//		if(temp2 == gpio_get_level(KEY2))  key2 = temp2;
//		if(temp3 == gpio_get_level(KEY3))  key3 = temp3;
//		if(temp4 == gpio_get_level(KEY4))  key4 = temp4;
//		if(temp5 == gpio_get_level(SW1))   sw1 = temp5;
//		if(temp6 == gpio_get_level(SW2))   sw2 = temp6;
		key_scanner();
		if( KEY_SHORT_PRESS == key_get_state(KEY_1))       key1=1;
		else                                               key1=0;
		if( KEY_SHORT_PRESS == key_get_state(KEY_2))       key2=1;
		else                                               key2=0;
			if( KEY_SHORT_PRESS == key_get_state(KEY_3))       key3=1;
		else                                               key3=0;
			if( KEY_SHORT_PRESS == key_get_state(KEY_4))       key4=1;
		else                                               key4=0;
	
		if ((gpio_get_level(SW1))&&(gpio_get_level(SW2)))//     测试1  off and off 
		{   //此模式车不执行任何事情 作为调参和比赛的过渡状态
			//展示地图 通过屏幕
//			tft180_show_string(0,0,"show_map");
//			if (!page_j){tft180_show_string(60,0,"first_page");}
//			if (page_j==1){tft180_show_string(60,0,"second_page");}
//			if (page_j==2){tft180_show_string(60,0,"third_page");}
//			for (int i=0;i<10;i++)
//			{
//			tft180_show_int(0,(i+1)*14,map_read[i+page_j][0],5);
//			tft180_show_int(60,14*(i+1),map_read[i+page_j][1],5);
//			}
//			if (key2==1)
//			{//page_j 加加 0 1 2 
//				page_j = ((page_j==2)?2:(page_j+1));
//				tft180_clear();
//			}
//			if (key3==1)
//			{
//				page_j = ((page_j==0)?0:(page_j-1));
//				tft180_clear();
//			}
//			if (key1==1)//
//			{	tft180_clear();
//				beep_boom(2000,2);
//				break;//进入跑车模式
//			}
while(1)
{put_up();
	system_delay_ms(1000);
//pick();
system_delay_ms(1000);
}
}
		if ((!gpio_get_level(SW1))&&(gpio_get_level(SW2)))//    测试2  01 左侧on 右侧off
		{	
		//按键检测
			/* 
			//调电机闭环模式 直线前进1m  平行0.5m  同时屏蔽平行移动
			//确定个这个模式flag
			flag1=1;flag2=0;flag3=0;flag4=0;start_correct_flag=0;
			tft180_show_string(0,0*lcd_wide,"motor_close");
			system_delay_ms(10);
			*/
			
			
//			//调参模式  all pid
//			//flag1=0;flag2=1;flag3=0;flag4=0;
//			start_correct_flag=0;
//		    //更改角度环从0°到其他度数转向测试  需要转向时 可行性需要测试
//			tft180_show_string(0,0*lcd_wide,"turn_angle_test");
//			tft180_show_string(0,1*lcd_wide,"imu.kp");
//			tft180_show_float(4*lcd_wide,1*lcd_wide,IMU.kpid_out[0],3,3);
//			tft180_show_string(0,2*lcd_wide,"imu.kd");
//			tft180_show_float(4*lcd_wide,2*lcd_wide,IMU.kpid_out[2],3,3);
//			start_correct_flag=0;
			flag1=0;flag2=1;flag3=0;flag4=0;
		    map_read[0][0]=40;map_read[0][1]=460;
	        map_read[1][0]=350;map_read[1][1]=540;
	        map_read[2][0]=730;map_read[2][1]=250;
	        map_read[3][0]=250;map_read[3][1]=-30;		
			  map_read[4][0]=-30;map_read[4][1]=200;		
	  data_lent = wireless_uart_read_buff(data_buffer_text, 32);                    // 查看是否有消息 默认缓冲区是 WIRELESS_UART_BUFFER_SIZE 总共 64 字节
        if(data_lent!= 0)                                                       // 收到了消息 读取函数会返回实际读取到的数据个数
       {   get_message=func_str_to_uint((char*)data_buffer_text);
		   memset(data_buffer_text,0,32);
	   }

	//现在按30 到时候 往主板传点数
	for (int m=0;(m<5)&&(flag==0);m++)
	{	
	go_next(map_read[m][0],map_read[m][1]);
		while(1)
		{if(fin_flag==1)
			{fin_flag=0;
			system_delay_ms(500);
			break;
			}
		}
	}
	flag=1;
	//beep_boom(5,1);
	if(get_message==5)
		{  Car.car_x=0;
	       Car.car_y=0;
			system_delay_ms(500);
			flag=0;
		}
	if(get_message==1)
	{
	//IMU.kpid_out[0]-=0.2;
//		pid_adjust(&pid5,0.2,0,0); 
        road+=20;	
	}
	if(get_message==2)
	{ pid_adjust(&pid5,-0.2,0,0)  ;
	
	}
	if(get_message==3)
	{ pid_adjust(&pid5,0,0,3)  ;
	
	}
	if(get_message==4)
	{ pid_adjust(&pid5,0,0,-3)  ;
	
	}
	
			tft180_show_string(0,0*lcd_wide,"car_x");
	        tft180_show_float(4*lcd_wide,0*lcd_wide,Car.car_x,3,3);
	        tft180_show_string(0,1*lcd_wide,"car_y");
	       tft180_show_float(4*lcd_wide,1*lcd_wide,Car.car_y,3,3);
	     tft180_show_string(0,2*lcd_wide,"kp");
	tft180_show_float(4*lcd_wide,2*lcd_wide,pid5.kp,3,3);
	 tft180_show_string(0,3*lcd_wide,"kd");
	tft180_show_float(4*lcd_wide,3*lcd_wide,pid5.kd,3,3);
		 tft180_show_string(0,5*lcd_wide,"road");
	tft180_show_int(4*lcd_wide,5*lcd_wide,road,5);
	get_message=0;
			system_delay_ms(10);
		}
		
		if ((gpio_get_level(SW1))&&(!gpio_get_level(SW2)))//    测试3   01 左侧off 右侧on
		{	//识别测试
			flag1=0;flag2=0;flag3=1;flag4=0;
				uart_write_byte((UART_4), 2);
			if(fabs(img_error[0]) <= 2&& fabs(img_error[1]) <= 2 && find_flag == 1)
		   {
			//state_control(2);
			while(object_type[two_con]/10 == 0)
			 {
				//等待识别
				system_delay_ms(1);	
			 }
				system_delay_ms(50);
		    }
		   tft180_show_int(0,30,work_state,3);
			 tft180_show_float(0,60,IMU.Yaw,5,5);
			 tft180_show_int(0,90,find_flag,8);
	   }
		
//	if ((!gpio_get_level(SW1))&&(!gpio_get_level(SW2)))    //4
//		{
//			flag1=0;flag2=0;flag3=0;flag4=1;
//			//摄像头纠正调试
//	   data_lent = wireless_uart_read_buff(data_buffer_text, 32);                    // 查看是否有消息 默认缓冲区是 WIRELESS_UART_BUFFER_SIZE 总共 64 字节
//        if(data_lent!= 0)                                                       // 收到了消息 读取函数会返回实际读取到的数据个数
//       {   get_message=func_str_to_uint((char*)data_buffer_text);
//		   memset(data_buffer_text,0,32);
//	   }

//			tft180_show_string(0,0*lcd_wide,"art_correct");
//			system_delay_ms(10);
//			correct_move();
//			tft180_show_string(0,6*lcd_wide,"kp");
//	tft180_show_float(4*lcd_wide,6*lcd_wide,pid7.kp,3,3);
//			tft180_show_string(0,7*lcd_wide,"kd");
//	tft180_show_float(4*lcd_wide,7*lcd_wide,pid7.kd,3,3);
//	   tft180_show_float(0,8*lcd_wide,jiux,3,3);
//	if(get_message==1)
//	{
//	//IMU.kpid_out[0]-=0.2;
//		pid_adjust(&pid7,0.1,0,0); 
//		//pid_adjust(&pid6,0.1,0,0); 
//	}
//	if(get_message==2)
//	{ pid_adjust(&pid7,-0.1,0,0)  ;
//	//	pid_adjust(&pid6,-0.1,0,0)  ;
//	
//	}
//	if(get_message==3)
//	{ pid_adjust(&pid7,0,0,0.5)  ;
//	//	pid_adjust(&pid6,0,0,0.5)  ;
//	
//	}
//	if(get_message==4)
//	{ pid_adjust(&pid7,0,0,-0.5)  ;
//		//pid_adjust(&pid6,0,0,-0.5)  ;
////		sw1_p=sw1;
////		sw2_p=sw2;
////		if ((sw1_p!=sw1)||(sw2_p!=sw2)){tft180_clear();tft180_full(RGB565_BLACK);}
//	}
//	get_message=0;
//}
	if ((!gpio_get_level(SW1))&&(!gpio_get_level(SW2)))            //测试4
		{
			flag1=0;flag2=0;flag3=0;flag4=1;
			key_scanner();
		if( KEY_SHORT_PRESS == key_get_state(KEY_1))       key1=1;
		else                                               key1=0;
		if( KEY_SHORT_PRESS == key_get_state(KEY_2))       key2=1;
		else                                               key2=0;
			if( KEY_SHORT_PRESS == key_get_state(KEY_3))       key3=1;
		else                                               key3=0;
			if( KEY_SHORT_PRESS == key_get_state(KEY_4))       key4=1;
		else                                               key4=0;
			//摄像头纠正调试
			while(1)
	{	    tft180_show_string(0,0*lcd_wide,"art_correct");
			correct_move();
			tft180_show_string(0,6*lcd_wide,"kp");
	        tft180_show_float(4*lcd_wide,6*lcd_wide,pid6.kp,3,3);
			tft180_show_string(0,7*lcd_wide,"kd");
	        tft180_show_float(4*lcd_wide,7*lcd_wide,pid6.kd,3,3);
			 tft180_show_string(0,5*lcd_wide,"kind");
			 tft180_show_int(5*lcd_wide,5*lcd_wide,object_type[two_con],3);	
		if(fabs(img_error[0]) <= 1&& fabs(img_error[1]) <= 1 && find_flag == 1)
		{  
           uart_write_byte((UART_4), 2);
			while(object_type[two_con] == 0)
			{uart_write_byte((UART_4), 2);
			
			}
					
			//微调摄像头找到图片标志			  
			//break;
		}
		else{continue;}	
//		    img_error[0] = 0;
//			img_error[1] = 0;
//			Car.ex_vel_x = 0;
//			Car.ex_vel_y = 0;
	       if(fabs(img_error[0]) > 2|| fabs(img_error[1]) > 2)  continue;
		find_flag = 0;
			while(find_flag == 0||Car.vel_x!=0||Car.vel_y!=0)
			{
				system_delay_ms(1);
			}
			//system_delay_ms(500);
			
		//	gpio_set_level(B11,1);
			taking_flag=1;
			 system_delay_ms(10);
			if(taking_flag==1)
			{timer_start(GPT_TIM_1);//开始计时器
		    core_servo_move(object_type[two_con]%4-1);  //0到4
	        system_delay_ms(200);
				servo_move1();
		//	system_delay_ms(00);
			taking_flag=0;
			time_need=timer_get(GPT_TIM_1);
			}
		
	
if(get_message==1)
	{
	//IMU.kpid_out[0]-=0.2;
		pid_adjust(&pid7,0.1,0,0); 
		//pid_adjust(&pid6,0.1,0,0); 
	}
	if(get_message==2)
	{ pid_adjust(&pid7,-0.1,0,0)  ;
	//	pid_adjust(&pid6,-0.1,0,0)  ;
	
	}
	if(get_message==3)
	{ pid_adjust(&pid7,0,0,0.5)  ;
	//	pid_adjust(&pid6,0,0,0.5)  ;
	
	}
	if(get_message==4)
	{ pid_adjust(&pid7,0,0,-0.5)  ;
		//pid_adjust(&pid6,0,0,-0.5)  ;
//		sw1_p=sw1;
//		sw2_p=sw2;
//		if ((sw1_p!=sw1)||(sw2_p!=sw2)){tft180_clear();tft180_full(RGB565_BLACK);}
	}
	get_message=0;
		sw1_p=sw1;
		sw2_p=sw2;
		if ((sw1_p!=sw1)||(sw2_p!=sw2)){tft180_clear();tft180_full(RGB565_BLACK);}
	}
}

	}
	key_clear_all_state();	 
	}
