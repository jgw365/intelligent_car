#include "car.h"
//车辆运行状态
float velocity_global_lim_max=20;
float velocity_global_lim=0;
int youkuang=12;
uint8_t work_state = 99;   
uint8_t work_state_p = 0;
float start_back_y;
float start_back_x;
float ac_vel_y;
float ac_vel_x;
float cal_x=10;
uint8 identify_buffer[32];
struct data dataz;
uint32 time_before=0;
uint32 time_after=0;
bool no_find_flag=0;
uint16 closer_x;
//int xxx[5]={0};
//zean
int last_place[2]={0};
uint8 state_9_num=0;//第几次扫图
bool zean_dir=0;
bool scan_get_flag=0;
bool flag6=0;
int32 flag7=0;
bool flag8=0;
bool change_flag=0;   //9以后不进行位置更新
int zean_flag=0;// 扫图不更新
float real_angle;
int no_find=0;
int six_div=0;
//车况标志位
bool start_parallel_flag=0;
bool fin_flag=0;//跑点是否完成 下一步纠正
bool state1ok_flag=0;
bool ahead_flag=0;
bool scan_flag=0;
bool place_flag=0;
//调试状态标志位
bool flag1=0;
bool flag2=0;
bool flag3=0;
bool flag4=0;
bool flag_nothong=0;
bool return_flag=0;
float vx;
float vy;
float w1=0;
float w=0;
bool adjust_car_jw_flag=0;
uint8 game_state=0; //5分类或6分类 1为5,2为6
//车身状态（位置 Vx Vy Vw 角度） ex 代表期望值  
//向右有x正方向  向前为y正方向 顺时针为w正方向  
_Car_state Car = 
{
	.vel_x = 0,
    .vel_y = 0,
    .vel_w = 0,
    .car_x =15,
	.car_y =0,
	.car_yaw = 0,
	.ex_vel_x = 0,
    .ex_vel_y = 0,
    .ex_vel_w = 0,
    .ex_car_x = 15,
	.ex_car_y = 0,
	.ex_car_yaw = 0,
	.straight=0,
	.ex_straight=0,
	.ex_car_x_p=0,
	.ex_car_y_p=0
}; 
void cal_dis()
{   
	 vx=(motor1.dis-motor2.dis+motor3.dis-motor4.dis)/1.414/4.0;
	 vy=(motor1.dis+motor2.dis+motor3.dis+motor4.dis)/1.414/4.0;
	//Car.vel_x=(motor1.dis-motor2.dis+motor3.dis-motor4.dis)/1.414/4.0;
   //Car.vel_y=(motor1.dis+motor2.dis+motor3.dis+motor4.dis)/1.414/4.0;
	ac_vel_y=(motor1.vel_ac+motor2.vel_ac+motor3.vel_ac+motor4.vel_ac)/1.414/4.0;
	ac_vel_x=(motor1.vel_ac-motor2.vel_ac+motor3.vel_ac-motor4.vel_ac)/1.414/4.0;
	float imu_yaw=IMU.Yaw*pi/180;
    Car.car_x+=0.95*(vy*sin(imu_yaw)+vx*cos(imu_yaw));
	//Car.car_x = *cal_x;
	Car.car_y += vy*cos(imu_yaw)-vx*sin(imu_yaw);
	Car.straight+=(motor1.dis+motor2.dis+motor3.dis+motor4.dis)/1.414/4.0;
}
void plan_vel()
{   static float vel_toge_p=0;
	float dis_x = Car.ex_car_x -  Car.car_x;
	float dis_y = Car.ex_car_y -  Car.car_y;
	float dis_x1 = Car.ex_car_x_p -  Car.car_x;
	float dis_y1 = Car.ex_car_y_p -  Car.car_y;
	float dis_x2 = Car.ex_car_x_p -  Car.ex_car_x ;
	float dis_y2 = Car.ex_car_y_p -  Car.ex_car_y;
	float dis=(float)sqrt((double)(dis_x*dis_x + dis_y*dis_y));
	float dis_ago=(float)sqrt((double)(dis_x1*dis_x1 + dis_y1*dis_y1));
	float dis_all=(float)sqrt((double)(dis_x2*dis_x2 + dis_y2*dis_y2));  //两次坐标点之差
	//外环2ms 内环1ms
	float vel_toge;
//	if (dis_ago<5){vel_toge=common_pid(&pid5,dis_ago);}
//	else{vel_toge=common_pid(&pid5,dis);}
	speeed_area_up(dis_ago,30,5);
//	if(dis_ago>30)
//	{
//		if(fabs(dis_all-dis_ago)<=40)
//	    {
//			 velocity_global_lim=((0.5*dis+5)>velocity_global_lim?velocity_global_lim:(0.5*dis+5));
//			if(velocity_global_lim>velocity_global_lim_max)
//				velocity_global_lim=velocity_global_lim_max;
//	    }
//	}
	vel_toge=common_pid(&pid5,dis);
//	vel_toge=0.7*vel_toge+0.3*vel_toge_p;
	Car.ex_vel_x=vel_toge*dis_x/dis;
	Car.ex_vel_y=vel_toge*dis_y/dis;
	vel_toge_p=vel_toge;
//	speed_up_down(dis_ago/dis_all,0.25,0,5);
	
	
}	
//区间方式加速
void speeed_area_up(float now, float area, int stay)
{    float k_up;
	float  k_down;
	k_up=now/area;
	if(now<=area&&now>=0)                                  //速度分段防打滑
	velocity_global_lim=(int)(k_up*(velocity_global_lim_max-stay)+stay);
	else 
		velocity_global_lim=velocity_global_lim_max;
}
//加减速函数 up为加速区间，down同理，stay为初速度
void speed_up_down(float state,float up,float down,int stay)
{    float k_up;
	float  k_down;
	k_up=(velocity_global_lim_max-stay)/(up*velocity_global_lim_max);            //加速斜率
	k_down=(velocity_global_lim_max-stay)/(down*velocity_global_lim_max);
	if(state<=up&&state>=0)                                  //速度分段防打滑
	velocity_global_lim=(int)(k_up*(state)*velocity_global_lim_max+stay);
	else 
		//if(state<=(1-down)&&state>up)
		velocity_global_lim=velocity_global_lim_max;
//	else if(state<=1&&state>=(1-down))
//	{velocity_global_lim=(int)(k_down*(1-state)*velocity_global_lim_max+stay);
////	  if(velocity_global_lim<=10)
////		  velocity_global_lim=10;
//	}
//	else
//		velocity_global_lim=stay;
};
uint8 speed_control(float state,float up,float down,int stay)
{   float k_up;
	float  k_down;
	k_up=(velocity_global_lim_max-stay)/up;            //加速斜率
	k_down=(velocity_global_lim_max-stay)/down;
	if(state<=up&&state>=0)                                  //速度分段防打滑
	return (int)(k_up*(state)+stay);
	else if(state<=(1-down)&&state>up)
		return velocity_global_lim_max;
	else if(state<=1&&state>=(1-down))
	{ return (int)(k_down*(1-state)+stay);
//	  if(velocity_global_lim<=10)
//		  velocity_global_lim=10;
	}
	else
		return stay;
}
void car_status_update()
{   //static int cnt=0;
	if((work_state == 1) ||((work_state == 2&&adjust_car_jw_flag==1)&&find_flag==0)|| (work_state == 4&&taking_flag==0) ||work_state == 9||work_state==0)
	{
		if ((fabs_(Car.ex_car_y-Car.car_y)<4)&&(fabs_(Car.ex_car_x-Car.car_x)<4))
		{
//			cnt++;
//			if(cnt==200){
			//start_parallel_flag=0;
			fin_flag=1;
			wheel_control(0,0,0);
			//gpio_set_level(BEEP_PIN,1);
			Car.ex_car_x_p=Car.ex_car_x;
		    Car.ex_car_y_p=Car.ex_car_y;
			//cnt=0;
			//}
		}
		else
		{  
			plan_vel();
			wheel_control(Car.ex_vel_x,Car.ex_vel_y,0);
		}
//		if(taking_flag)
//		{
//			if(work_state==1)
//				xxx[0]=1;
//			if(((work_state == 2&&adjust_car_jw_flag==1)&&find_flag==0))
//				xxx[0]=2;
//			if((work_state == 4&&taking_flag==0))
//				xxx[0]=3;
//			if(work_state==9)
//				xxx[0]=4;
//			if(work_state==0)
//				xxx[0]=5;
//		}
	}
	else if(work_state == 5)
	{ 
		
//	if(taking_flag){xxx[1]=1;}
	approach_img_back();
	wheel_control(Car.ex_vel_x,Car.ex_vel_y,0);
	
	}
	else if((work_state == 2 || work_state == 3)&&taking_flag==0)
	{
//if(taking_flag){xxx[2]=1;}		
		if(control_flag==1)
		{approach_img();}
		wheel_control(Car.ex_vel_x,Car.ex_vel_y,0);
		control_flag=0;
	}
	else if(work_state==8)
	{
//		if(taking_flag){xxx[3]=1;}
		wheel_control(Car.ex_vel_x,0,0);
	}
	else if(taking_flag==1)
	{       
		img_error[0] = 0;
			img_error[1] = 0;
		Car.ex_car_x = Car.car_x;
		Car.ex_car_y = Car.car_y;
		motor1.duty=0;
		motor2.duty=0;
		motor3.duty=0;motor4.duty=0;
		
        wheel_control(0,0,0);
	}
	else
	{
//		if(taking_flag){xxx[4]=1;}
	wheel_control(0,0,0);
	}
}
void test_pid()
{if(flag2==1)
	{
		if ((fabs_(Car.ex_car_y-Car.car_y)<2)&&(fabs_(Car.ex_car_x-Car.car_x)<2))
		{

			fin_flag=1;
			wheel_control(0,0,0);
			//gpio_set_level(BEEP_PIN,1);
			Car.ex_car_x_p=Car.ex_car_x;
		    Car.ex_car_y_p=Car.ex_car_y;
			//cnt=0;
			//}
		}
		else
		{  
			plan_vel();
			wheel_control(Car.ex_vel_x,Car.ex_vel_y,0);
		}
		
	}
	else if((flag4==1||flag3==1)&&taking_flag==0)
	{//gpio_set_level(BEEP_PIN,1);
		if(control_flag==1)
		{approach_img();}
		wheel_control(Car.ex_vel_x,Car.ex_vel_y,0);
		control_flag=0;}
	else if(taking_flag==1)
	{       img_error[0] = 0;
			img_error[1] = 0;
			Car.ex_vel_x = 0;
			Car.ex_vel_y = 0;
            wheel_control(0,0,0);}
	else
		{
	wheel_control(0,0,0);
	}
}

void state_control(uint8 state)
{
	//uint8_t head[2] = {0xed,0x92};
	//重传次数
	//static uint16_t reply = 0;
	//通信协议
	
	while(1){
		if(state != 5)
			uart_write_byte((UART_4), (state));
		else
			uart_write_byte((UART_1), (state));
		system_delay_ms(45);
		if(reply_state == state) 
		{
			work_state = state;
		//	gpio_set_level(BEEP_PIN,0);
			return;
		}
		//reply++;
		//vofa_data[2] = (float)reply;
	}
	
}

void state_0()
{	
	//zean
		if((gpio_get_level(SW1))&&(gpio_get_level(SW2))) game_state=1;   //off  off
		else if((gpio_get_level(SW1))&&(!gpio_get_level(SW2)))   // off on   粮食
		{
			six_div=1;
			game_state=2;
		}
		else if((!gpio_get_level(SW1))&&(gpio_get_level(SW2))) //on off     水果
		{
			six_div=2;
			game_state=2;
		}
		else      //on on      蔬菜
		{
			six_div=3;
			game_state=2;
		}
	velocity_global_lim_max=20;
	//选择5分类或6分类
	gpio_set_level(B11,1);
	system_delay_ms(500);
	gpio_set_level(B11,0);
	while(1)
	{	key_scanner();
		if( KEY_SHORT_PRESS == key_get_state(KEY_2))     
		{ 
			place_all+=1;
		}			
		if( KEY_SHORT_PRESS == key_get_state(KEY_3))       
		{ 
			place_all-=1;
		}
		
	tft180_show_string(0,2*lcd_wide,"place_all");
	 tft180_show_int(5*lcd_wide,3*lcd_wide,place_all,3);
		
    if( KEY_SHORT_PRESS == key_get_state(KEY_4))       
		{ 
			break;
		}		
	}
	key_clear_all_state();
	gpio_set_level(B11,1);
	system_delay_ms(500);
	gpio_set_level(B11,0);
	
	while(1)
	{	key_scanner();
		if( KEY_SHORT_PRESS == key_get_state(KEY_2))     
		{ 
			velocity_global_lim_max+=2;
		}			
		if( KEY_SHORT_PRESS == key_get_state(KEY_3))       
		{ 
			velocity_global_lim_max-=2;
		}
		tft180_show_string(0,4*lcd_wide,"vel");
	 tft180_show_float(3*lcd_wide,5*lcd_wide,velocity_global_lim_max,3,1);
    if( KEY_SHORT_PRESS == key_get_state(KEY_4))       
		{ 
			break;
		}		
	}
	key_clear_all_state();
	gpio_set_level(B11,1);
	system_delay_ms(500);
	gpio_set_level(B11,0);
	while(1)
	{	
		key_scanner();
		if( KEY_SHORT_PRESS == key_get_state(KEY_2))     
		{ 
			youkuang++;
		}			
		if( KEY_SHORT_PRESS == key_get_state(KEY_3))       
		{ 
			youkuang--;
		}
		tft180_show_string(0,6*lcd_wide,"youkuang");
	 tft180_show_int(3*lcd_wide,7*lcd_wide,youkuang,3);
    if( KEY_SHORT_PRESS == key_get_state(KEY_4))       
		{ 
			break;
		}		
	}
	 uart_write_byte((UART_4),youkuang);
	 tft180_show_string(0,0*lcd_wide,"game_state");
	 tft180_show_int(5*lcd_wide,1*lcd_wide,game_state,3);
	
//	//等待地图到来
	while(map_read[0][0] == 0 || map_read[0][1] == 0)
	{
		system_delay_ms(1);
	}
	//接收完成
	//计算坐标个数
	gpio_set_level(B11,1);
	for(int i=0;map_read[i][0]!=0 || map_read[i][1]!=0;i++)
	{
		map_sum++;
	}
//	//zean:加入路径规划
	for(int i=0;i<map_sum;i++)
	{
		dataz.map_read[i][0]=map_read[i][0];
		dataz.map_read[i][1]=map_read[i][1];
	}
	dataz=calc_path(dataz,map_sum);
	for(int i=0;i<map_sum;i++)
	{
		map_read[i][0]=dataz.map_read[i][0];
		map_read[i][1]=dataz.map_read[i][1];
	}
	//目前初始位置设置为0
	//IMU.now_x = 30;
	//IMU.now_y = -30;
	state_control(1);  //后期想改  再把uart3的纠正开启开关装上
//    char text[15];	
//	for(int i=0;i<map_sum;i++)
//	{
//		sprintf(text,"%d %d\n",(map_read[i][0]+10)/20,(map_read[i][1]+10)/20);
//		wireless_uart_send_string(text);
//		system_delay_ms(5);
//	}
	//while(1);
	system_delay_ms(300);                  //抽地图
    gpio_set_level(B11,0);
	system_delay_ms(200);                  //抽地图
	
    go_next(10,60);       //出库
	while(1)
	{if(fin_flag==1)
	   break;
	}
	system_delay_ms(200);
	work_state = 1;
	uart_write_byte((UART_1),1);
	system_delay_ms(45);
};
void state_1()
{	bool corr_cnt=0;
	corr_cnt=0;
	timer_clear(GPT_TIM_1);
	uint32 putdown_time = 0;//timer_get(GPT_TIM_1);;
//	map_read[map_num][0]=((int)map_read[map_num][0]+20)/20*20-10;
//	map_read[map_num][1]=((int)map_read[map_num][1]+20)/20*20-10;
	find_flag=0;
	//zean
	if(taking_flag==1)
	{     
		taking_flag=0;
		if(game_state==1)
		{
			core_servo_move(my_arry[object_type[two_con-1]%10-1]);  //0到4
			go_next(map_read[map_num][0],map_read[map_num][1]-12);
			pick_2();
			system_delay_ms(100);
			if(my_arry[object_type[two_con-1]%10-1]>=4)
				pick_up2();
			else
				pick_up1();
		}
		else
		{
			if(six_div==(object_type[two_con-1]/10))
			{
				core_servo_move(my_arry[0]);
				go_next(map_read[map_num][0],map_read[map_num][1]-12);
				pick_2();
				system_delay_ms(100);
				pick_up1();
			}
			else
			{
				core_servo_move(my_arry[object_type[two_con-1]%10]);  //1到5
				go_next(map_read[map_num][0],map_read[map_num][1]-12);
				pick_2();
				system_delay_ms(100);
				if(my_arry[object_type[two_con-1]%10]>=4)
					pick_up2();
				else
					pick_up1();
			}
		}
//		char text[10];
//		for(int i=0;i<=4;i++)
//		if(xxx[i]){
//			sprintf(text,"%d %d\n",i,xxx[i]);
//			wireless_uart_send_string(text);
//		}
	}
	else
		go_next(map_read[map_num][0],map_read[map_num][1]-12);
	while(1)
	{
		
	if((!corr_cnt)&&(timer_get(GPT_TIM_1) >= 700 ))
	{
		//可以打开微调摄像头
		
		corr_cnt=1;
	}
	if(fin_flag == 1)   
	{
		state1ok_flag=1; 
		work_state=2;
		break;
	}
	if(map_num < place_all && find_flag == 1 )
	{	//work_state=2;
		
		work_state=2;
		ahead_flag=1;
		break;
	}
	
	}
	
	
//	if(map_num <map_sum && ahead_flag == 0)
//	{
//		work_state=2;
//		//state_control(2);
//	}
	state1ok_flag = 0;
	//等待坐标
	find_flag = 0;
	system_delay_ms(100);
	adjust_car_jw_flag=1;
	if(find_flag==0)
	{
	start_back_y=Car.car_y;
	start_back_x=Car.car_x;
	}
	if(find_flag == 0)   //未看到后退
	{	find_flag=0;
		 go_next(start_back_x,start_back_y-30);
		 while(1)
		{
			system_delay_ms(1);
			if(fin_flag==1)
			{
				break;
			}
			if (find_flag==1) break;
		}
		 if (find_flag==1)
		{
			motor1.duty=0;
			motor2.duty=0;
			motor3.duty=0;
			motor4.duty=0;
			adjust_car_jw_flag=0;
			 return;
		}
		
		
		//find_flag=0;
		 go_next(start_back_x,start_back_y+10);
		 while(1)
	 {
		 system_delay_ms(1);
		 if(fin_flag==1)
		 {
		 break;
		 }
		 if (find_flag==1) break;
	 }
		 if (find_flag==1)
		{
		motor1.duty=0;
		motor2.duty=0;
		motor3.duty=0;
		motor4.duty=0;
		adjust_car_jw_flag=0;
		 return;
		}
		
	//	find_flag=0;
		 go_next(start_back_x-20,start_back_y);
		 while(1)
	 {
		 system_delay_ms(1);
		 if(fin_flag==1)
		 {
		 break;
		 }
		 if (find_flag==1) break;
	 }
		 if (find_flag==1)
		{
		motor1.duty=0;
		motor2.duty=0;
		motor3.duty=0;
		motor4.duty=0;
		adjust_car_jw_flag=0;
		 return;
		}
		
		//find_flag=0;
		 go_next(start_back_x+20,start_back_y);
		 while(1)
	 {
		 system_delay_ms(1);
		 if(fin_flag==1)
		 {
		 break;
		 }
		 if (find_flag==1) break;
	 }
		 if (find_flag==1)
		{
		motor1.duty=0;
		motor2.duty=0;
		motor3.duty=0;
		motor4.duty=0;
		adjust_car_jw_flag=0;
		 return;
		}
		work_state=3;
		no_find_flag=1;
		no_find++;
		
	}
	  
	
	adjust_car_jw_flag=0;
	motor1.duty=0;
	motor2.duty=0;
	motor3.duty=0;
	motor4.duty=0;
	//待测试变量  是error0 还是error1
};
void make_up_error(bool dir)//1右0左
{
	flag6=1;flag7=0;flag8=1;
	zean_dir=dir;
	int old_state=work_state;
	work_state=8;
	uart_write_byte((UART_1),8);
	system_delay_ms(500);
	Car.ex_vel_x=(dir==1?20:-20);
	Car.ex_vel_y=0;
	img_error[0]=1000;
	char text[35];
	uart_write_byte((UART_1),8);
	system_delay_ms(20);
	while(1)
	{	
		//system_delay_ms(1000);
		if(flag7>2)
		{
			flag8=0;
			Car.ex_vel_x=0;
			uart_write_byte((UART_4),4);
			//新加的东西 在边线纠正角度时候纠正下x的位置
			Car.car_x=(dir==0)?0:700;
//				char testx[25];
//              sprintf(testx,"num:%f \n",Car.car_x);
//	            wireless_uart_send_string(testx);
			system_delay_ms(100);
			flag6=0;
			uart_write_byte((UART_4),5);
			angle_p=0;
//			if(dir==1)
//			{
//				if(real_angle<0)
//					real_angle*=0.56;
//				else
//					real_angle*=1;
//				
//			}
//			else
//			{
//				if(real_angle<0)
//					real_angle*=0.56;
//				else
//					real_angle*=1;
//			}
            if(fabs(real_angle)>15)
				real_angle=0;
			if(real_angle<0)
			{
				Car.car_yaw=-real_angle;
				IMU.Yaw=-real_angle;
			}
			else
			{
				Car.car_yaw=360.0-real_angle;
				IMU.Yaw=360.0-real_angle;
			}
			Car.car_yaw=-real_angle;
			IMU.Yaw=-real_angle;
			system_delay_ms(100);
			//sprintf(text,"angle:%f\n",real_angle);
			//wireless_uart_send_string(text);
			break;
		}
	}
	
	work_state=old_state;
	uart_write_byte((UART_1),1);
	system_delay_ms(100);
//	gpio_set_level(B11,0);
	//while(1);
}
void state_2()
{    
//	img_error[0]=0;
//	img_error[1]=1;
	while(1)
	{	  

//			 tft180_show_string(0,5*lcd_wide,"kind");
//			 tft180_show_int(5*lcd_wide,5*lcd_wide,object_type[two_con],3);	
		if(fabs(img_error[0]) <= 3&& fabs(img_error[1]) <= 3 && find_flag == 1)
		{  
           //state_control(2);
			uart_write_byte((UART_4),6);
			while(object_type[two_con] == 0)
			{
				//gpio_set_level(B11,1);
				system_delay_ms(1);
			}
			uart_write_byte((UART_4),5);
		}
	
		
		else{continue;}	
	
		break;
	}
	
			int8 pick_cnt=0;
			while(1)
			{
				if (control_1_flag==1)
				{
					if (fabs(img_error[0]) <= 1.5&& fabs(img_error[1]) <= 1.5)
					{pick_cnt++;
					}
					else{pick_cnt=0;}
					control_1_flag=0;
					
				}
				if (pick_cnt==4)
				{
					pick_cnt=0;
					break;
				}
				
			}
	
    motor1.duty = 0;
	motor2.duty = 0;
	motor3.duty = 0;
	motor4.duty = 0;	
	taking_flag=1;
	if(no_find_flag==0&&change_flag==0&&zean_flag==0)
	{
	Car.car_x = map_read[two_con][0] ;//放在车前面25cm
	Car.car_y = map_read[two_con][1]-12/*- 10*cos(IMU.Yaw*pi/180)*/;
	}
	else if(zean_flag)
	{
		zean_flag--;
	}
    no_find_flag=0;
		//	gpio_set_level(B11,1);
			uart_rx_interrupt(UART_3, 1);   
           	scan_flag=1;		 
			system_delay_ms(200);
			scan_flag=0;
		//	gpio_set_level(B11,0);
			//zean
      //uart_rx_interrupt(UART_3, 0);   	
			if(taking_flag==1)
			{	
	      // system_delay_ms(1000);
			//servo_move1();
			pick_1();
			system_delay_ms(50);
			}	
	
		find_flag = 0; 
	
	 work_state=3;
};
void state_3()
{   
	//gpio_set_level(B11,0);
	map_num++;
	two_con++;
	if(map_num < map_sum)
		work_state = 1;
	else
	    work_state = 4;
	if(Car.car_x<=450)      //attention       zean bug : &&Car.car_x>=50
	closer_x=Car.car_x;
	else 
		closer_x=400;
};
void state_4()
{  
	//zean
	gpio_set_level(B11,1);
	if(taking_flag==1)
	{     
		if(game_state==1)
		{
			core_servo_move(my_arry[object_type[two_con-1]%10-1]);  //0到4
			pick_2();
			system_delay_ms(100);
			if(my_arry[object_type[two_con-1]%10-1]>=4)
				pick_up2();
			else
				pick_up1();
		}
		else
		{
			if(six_div==(object_type[two_con-1]/10))
			{
				core_servo_move(my_arry[0]);
				pick_2();
				system_delay_ms(100);
				pick_up1();
			}
			else
			{
				core_servo_move(my_arry[object_type[two_con-1]%10]);  //1到5
				pick_2();
				system_delay_ms(100);
				if(my_arry[object_type[two_con-1]%10]>=4)
					pick_up2();
				else
					pick_up1();
			}
		}
		taking_flag=0;
//		char text[10];
//		for(int i=0;i<=4;i++)
//		if(xxx[i]){
//			sprintf(text,"%d %d\n",i,xxx[i]);
//			wireless_uart_send_string(text);
//		}
	}
	gpio_set_level(B11,0);
	//char textx[35];
	//sprintf(textx,"lx%d ly%d num:%d sum:%d\n",last_place[0],last_place[1],map_num,map_sum);
	
	if((last_place[0]!=0||last_place[1]!=0)&&(map_sum-no_find)<place_all)
	{
		int x=last_place[0],y=last_place[1];
		last_place[0]=0;last_place[1]=0;
		work_state=9;
		state_9(x,y);
		return ;
	}
	
	else if(map_num==map_sum&&(map_sum-no_find)<place_all&&state_9_num!=3)//没拾取完           加没全拾取到也结束
	{ 
		work_state=9;
		scan_get_flag=0;
		return;
	}
	
	uart_write_byte((UART_1),8);
	img_error[0]=0;img_error[1]=0;
	flag6=1;
	
	//保证不会离上线太近
	go_next(0,350);
	while(1)
	{
		//等待跑完
		system_delay_ms(1);
		if(fin_flag)
		{
			break;
		}
	}
	system_delay_ms(100);
	if(img_error[0]!=0)
	{
		Car.car_x=-img_error[0];

//		char textz[35];
//		sprintf(textz,"Car_x:%f\n",Car.car_x);
//		wireless_uart_send_string(textz);
	}
	else
	{
		go_next(-20,350);
		while(1)
		{
		//等待跑完
		system_delay_ms(1);
		if(fin_flag)
		{
			break;
		}
		}
		system_delay_ms(100);
		if(img_error[0]!=0)
		{
			Car.car_x=-img_error[0];
		}
		else
		{
			go_next(20,350);
			while(1)
			{
			//等待跑完
				system_delay_ms(1);
			if(fin_flag)
			{
				break;
			}
			}
			system_delay_ms(100);
			if(img_error[0]!=0)
			{
				Car.car_x=-img_error[0];
        
			}
		}
		
	}
	go_next(100,Car.car_y);
	while(1)
	{
	//等待跑完
	system_delay_ms(1);
	if(fin_flag)
	{
		break;
	}
	}
	
	velocity_global_lim_max=10;
	img_error[0]=0;img_error[1]=0;
	go_next(100,600);
	while(1)
	{
	//等待跑完
		if(img_error[1]!=0)
		{
		//	gpio_set_level(B11,1);
		    Car.ex_car_x=Car.car_x;
			Car.ex_car_y=Car.car_y;
			motor1.duty = 0;
			motor2.duty = 0;
			motor3.duty = 0;
			motor4.duty = 0;
			wheel_control(0,0,0);
			break;
		}
	//system_delay_ms(1);
		if(fin_flag)
		{
			break;
		}
	}
	Car.car_y=500-img_error[1]-3;
//	char text[35];
//	sprintf(text,"car_y;%f\n",Car.car_y);
//	wireless_uart_send_string(text);
        
	flag6=0;
	img_error[0]=0;img_error[1]=0;
	uart_write_byte((UART_1),1);
	velocity_global_lim_max=25;
	put_down();
	if(game_state==1)      //5分类
	{				
	go_next(350,530);      //attention      //比赛  cl，530
	core_servo_move(my_arry[0]);
	while(1)
	{
		//等待跑完
		system_delay_ms(1);
		if(fin_flag)
		{
			//通知放下
	ready_core(my_arry[0]);
    system_delay_ms(200);
    unload(my_arry[0]);
			break;
		}
	}
	go_next(730,250);          //attention
	while(1)
	{
		system_delay_ms(1);
		if(fin_flag)
		{
			//通知放下
	
			break;
		}
	}
	uart_write_byte((UART_1),8);
	img_error[0]=0;img_error[1]=0;
	flag6=1;
	system_delay_ms(100);
	if(img_error[0]!=0)
	{

		Car.car_x = 700-img_error[0];
		go_next(730,250);
		while(1)
		{
			if(fin_flag)
				break;
		}
	}
	else
	{	
		int8 cnt_816=0;
		while(1)
		{
			cnt_816++;
			if(cnt_816>2)
				break;
			go_next(Car.car_x+20,Car.car_y);
			while(!fin_flag);
			system_delay_ms(100);
			if(img_error[0]!=0)
			{
				Car.car_x = 700-img_error[0];
				go_next(730,250);
				while(!fin_flag);
				break;
			}
		}
	}
	
	
	uart_write_byte((UART_1),1);
	img_error[0]=0;img_error[1]=0;
	flag6=0;
	ready_core(my_arry[1]);
    system_delay_ms(200);
	unload(my_arry[1]);
	
	go_next(250,-30);        //attention
	while(1)
	{
		system_delay_ms(1);
		if(fin_flag)
		{
			//通知放下
			break;
		}
	}
	uart_write_byte((UART_1),8);
	img_error[0]=0;img_error[1]=0;
	flag6=1;
	system_delay_ms(100);
	if(img_error[1]!=0)
	{
		Car.car_y = -3-img_error[1];
		go_next(250,-30);
		while(1)
		{
			if(fin_flag)
				break;
		}
	}
	else
	{	
		int8 cnt_817=0;
		while(1)
		{
			cnt_817++;
			if(cnt_817>2)
				break;
			go_next(Car.car_x,Car.car_y-20);
			while(!fin_flag);
			system_delay_ms(100);
			if(img_error[1]!=0)
			{
				Car.car_y = -3-img_error[1];
				go_next(250,-30);
				while(!fin_flag);
				break;
			}
		}
	}
	
	
	uart_write_byte((UART_1),1);
	img_error[0]=0;img_error[1]=0;
	flag6=0;
	ready_core(my_arry[2]);
	system_delay_ms(200);
	unload(my_arry[2]);
	
	go_next(-65,200);        //attention
	while(1)
	{
		system_delay_ms(1);
		if(fin_flag)
		{
			//通知放下
			ready_core(my_arry[3]);
			system_delay_ms(200);
			unload(my_arry[3]);
			break;
		}
	}
			system_delay_ms(100);
			pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(60+servo3_error));
			system_delay_ms(100);
			pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(20+servo3_error));
	fin_flag=0;
	
}
	if(game_state==2)      //6分类
	{
		
	go_next(40,460);      //attention
	core_servo_move(my_arry[0]);
	while(1)
	{
		//等待跑完
		system_delay_ms(1);
		if(fin_flag)
		{
			//通知放下
			ready_core(my_arry[0]);
    system_delay_ms(200);
    unload(my_arry[0]);	
			break;
		}
	}
	go_next(30,530);
	while(1)
	{
		//等待跑完
		system_delay_ms(1);
		if(fin_flag)
		{
	ready_core(my_arry[1]);
    system_delay_ms(500);
    unload(my_arry[1]);
			break;
		}
	}
	go_next(30,400);
	while(!fin_flag);
	
//	go_next(350,540);      //attention
//	while(1)
//	{
//		//等待跑完
//		system_delay_ms(1);
//		if(fin_flag)
//		{
//			//通知放下
//	ready_core(my_arry[1]);
//    system_delay_ms(500);
//    unload(my_arry[1]);
//			break;
//		}
//	}
	
	go_next(730,250);          //attention
	while(1)
	{
		system_delay_ms(1);
		if(fin_flag)
		{
			//通知放下
	
			break;
		}
	}
	uart_write_byte((UART_1),8);
	img_error[0]=0;img_error[1]=0;
	flag6=1;
	system_delay_ms(100);
	if(img_error[0]!=0)
	{

		Car.car_x = 700-img_error[0];
		go_next(730,250);
		while(1)
		{
			if(fin_flag)
				break;
		}
	}
	else
	{	
		int8 cnt_816=0;
		while(1)
		{
			cnt_816++;
			if(cnt_816>2)
				break;
			go_next(Car.car_x+20,Car.car_y);
			while(!fin_flag);
			system_delay_ms(100);
			if(img_error[0]!=0)
			{
				Car.car_x = 700-img_error[0];
				go_next(730,250);
				while(!fin_flag);
				break;
			}
		}
	}
	
	
	uart_write_byte((UART_1),1);
	img_error[0]=0;img_error[1]=0;
	flag6=0;
	ready_core(my_arry[2]);
    system_delay_ms(200);
	unload(my_arry[2]);
	
	go_next(250,-30);        //attention
	while(1)
	{
		system_delay_ms(1);
		if(fin_flag)
		{
			//通知放下
			break;
		}
	}
	uart_write_byte((UART_1),8);
	img_error[0]=0;img_error[1]=0;
	flag6=1;
	system_delay_ms(100);
	if(img_error[1]!=0)
	{
		Car.car_y = -3-img_error[1];
		go_next(250,-30);
		while(1)
		{
			if(fin_flag)
				break;
		}
	}
	else
	{	
		int8 cnt_817=0;
		while(1)
		{
			cnt_817++;
			if(cnt_817>2)
				break;
			go_next(Car.car_x,Car.car_y-20);
			while(!fin_flag);
			system_delay_ms(100);
			if(img_error[1]!=0)
			{
				Car.car_y = -3-img_error[1];
				go_next(250,-30);
				while(!fin_flag);
				break;
			}
		}
	}
	
	
	uart_write_byte((UART_1),1);
	img_error[0]=0;img_error[1]=0;
	flag6=0;
	ready_core(my_arry[3]);
	system_delay_ms(200);
	unload(my_arry[3]);
	go_next(-65,200);        //attention
	while(1)
	{
		system_delay_ms(1);
		if(fin_flag)
		{
			//通知放下
			ready_core(my_arry[4]);
			system_delay_ms(200);
			unload(my_arry[4]);
			break;
		}
	}
			system_delay_ms(100);
			pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(60+servo3_error));
			system_delay_ms(100);
			pwm_set_duty(SERVO3_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY360_(20+servo3_error));
	fin_flag=0;
	
}
	work_state=5;
    uart_write_byte((UART_1),5);
	go_next(30,200);  
	while(1)
	{
		system_delay_ms(1);
		if(img_error[0]!=0)
			break;
		if(fin_flag)
		{
			break;
		}
	}
}
void state_5()         //回库
{
//	func_int_to_str((char*)identify_buffer,cor_img);
//	wireless_uart_send_buff(identify_buffer,strlen((const char *)identify_buffer));
//    wireless_uart_send_string(".\r\n");
//       memset(identify_buffer, 0, 32);
	
	if(img_error[1]!=0&&Car.car_y<10)
	{ 
	gpio_set_level(B11,1);
	motor1.duty = 0;
	motor2.duty = 0;
	motor3.duty = 0;
	motor4.duty = 0;	
	wheel_control(0,0,0);
	work_state=10;
	}
};
int16 start_place_y[3]={-15,155,345};//每次横向扫图初始y值，  待修改（比赛值
int16 start_place_x[3]={85,685,15};//每次横向扫图初始x值，  待修改（比赛值
int16 end_place_x[3]={685,15,685};//每次横向扫图结束x值，  待修改（比赛值

//int16 start_place_y[3]={60,205,285};//每次横向扫图初始y值，  待修改（美誉值
//int16 start_place_x[3]={55,435,55};//每次横向扫图初始x值，  待修改（美誉值
//int16 end_place_x[3]={435,60,435};//每次横向扫图结束x值，  待修改（美誉值
bool scan_stop_flag=0;
uint8 scan_num_time=0;
void state_9(int x,int y)
{	
	//1.到开始扫图的点
	change_flag=1;
	work_state=9;
	//char textx[35];
	if(x==0&&y==0&&state_9_num!=1)
	make_up_error(state_9_num%2);
	static bool out_side=0;
	if(state_9_num==0&&out_side==0)
	{
		go_next(85,Car.car_y);
		while(!fin_flag);
		out_side=1;
	}	
	if(x==0&&y==0)
		go_next(start_place_x[state_9_num],start_place_y[state_9_num]);
	else
	{
		go_next(x,y);
		//gpio_set_level(B11,1);
	}
	while(1)
	{
		if(fin_flag==1)
		{
			break;
		}
	}
	if(Car.ex_car_y==start_place_y[0]&&Car.ex_car_x==start_place_x[0])
	{
		uart_write_byte((UART_1),8);
		uart_write_byte((UART_3),7);
		img_error[0]=0;img_error[1]=0;
		flag6=1;
		system_delay_ms(100);
		while(1)
		{
			static int cnt_814=0;
			if (cnt_814>=3)
				break;
			if (img_error[1]!=0)
			{
			Car.car_y=-img_error[1]-3;
			break;
			}
			else
			{
				go_next(Car.car_x,Car.car_y-10);
				while(1)
				{
					if(fin_flag==1)
					{
					break;
					}
				}
			}
			cnt_814++;
		}
		
		
		flag6=0;
		img_error[0]=0;img_error[1]=0;
		uart_write_byte((UART_1),1);
	}
	
	
	
	
	//if(x==0&&y==0)
	//make_up_error(state_9_num%2);
	
	scan_get_flag=0;//每次跑前必清0
	scan_num_time=0;
	scan_flag=1;//小提议：考不考虑改为需要时再打开摄像头
	uart_rx_interrupt(UART_3, 1);  
	//gpio_set_level(B11,1);
	system_delay_ms(100);
	//gpio_set_level(B11,0);
	scan_stop_flag=0;//动起来了
//	sprintf(textx,"%d\n",(int)scan_flag);
//	wireless_uart_send_string(textx);
	img_error[0]=0;img_error[1]=0;
	go_next(end_place_x[state_9_num],start_place_y[state_9_num]);//zean attention  场地变化
	int8 old_map_sum=map_sum;
	while(1)
	{
		if(scan_get_flag==1)   //work_state==9时才会被激活
		{   
		//	gpio_set_level(B11,1);
			Car.ex_car_x=Car.car_x;
			Car.ex_car_y=Car.car_y;
			last_place[0]=(int)Car.car_x;
			last_place[1]=(int)Car.car_y;
			motor1.duty = 0;
			motor2.duty = 0;
			motor3.duty = 0;
			motor4.duty = 0;
			wheel_control(0,0,0);
			break;
		}
		if(fin_flag==1)
		{
			state_9_num++;
			if(state_9_num==3||(map_sum-no_find)>=place_all)//有图片漏掉了，也没办法只能回库了
			{
				work_state=4;
				return;
			}
			else
			{
			if(Car.ex_car_y==start_place_y[0]&&Car.ex_car_x==end_place_x[0])
			{
				uart_write_byte((UART_1),8);
				uart_write_byte((UART_3),9);
				img_error[0]=0;img_error[1]=0;
				flag6=1;
				system_delay_ms(100);
				while(1)
				{
					static int cnt_815=0;
					if (cnt_815>=2)
						break;
					if (img_error[1]!=0)
					{
						Car.car_y=-img_error[1]-3;
				 
						break;
					}
					else
					{
						if(cnt_815==0)
						{
							go_next(Car.car_x,Car.car_y-20);
						}
						if(cnt_815==1)
						{
							go_next(Car.car_x,Car.car_y+30);
						}
					}
					while(1)
					{
						if(fin_flag==1)
						{
						break;
						}
					}
					cnt_815++;
				}
				flag6=0;
				img_error[0]=0;img_error[1]=0;
				uart_write_byte((UART_1),1);
			}
				
				work_state=9;
				return;
			}
			
		}
	}
	system_delay_ms(600);
	//gpio_set_level(B11,0);
	//加延时，加电机置零
	scan_stop_flag=1;//停了
	system_delay_ms(400);
	scan_flag=0;
	//uart_rx_interrupt(UART_3, 0); 
//	for(int i=map_num;i<map_sum;i++)
//	{
//		sprintf(textx,"%d %d\n",map_read[i][0],map_read[i][1]);
//	wireless_uart_send_string(textx);
//	}
//   wireless_uart_send_string("\n");
//	sprintf(textx,"new:%d old:%d\n",map_sum,old_map_sum);
//	wireless_uart_send_string(textx);
//	wireless_uart_send_string("\n");
	if(map_sum==old_map_sum) //看错了map_sum==old_map_sum
	{
		if(map_num==map_sum)
		state_9(last_place[0],last_place[1]);
		else
			work_state=1;
	}
	else //看对了
		work_state=1;
	return ;
}			
void state_10()
{
	while(1)
	{}
}
void duty_zero()
{
	motor1.duty=0;
	motor2.duty=0;
	motor3.duty=0;
	motor4.duty=0;
}
void beep_boom(float ms,float times)
{   
	ms*=1000;
	for(int i=0;i<times;i++)
	{
	gpio_set_level(BEEP_PIN,1);
	system_delay_ms(ms);
	gpio_set_level(BEEP_PIN,0);
	system_delay_ms(ms);
	}
};
void correct_plan_vel()
{
	
}