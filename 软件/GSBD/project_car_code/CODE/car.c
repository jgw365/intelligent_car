#include "car.h"
//��������״̬
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
uint8 state_9_num=0;//�ڼ���ɨͼ
bool zean_dir=0;
bool scan_get_flag=0;
bool flag6=0;
int32 flag7=0;
bool flag8=0;
bool change_flag=0;   //9�Ժ󲻽���λ�ø���
int zean_flag=0;// ɨͼ������
float real_angle;
int no_find=0;
int six_div=0;
//������־λ
bool start_parallel_flag=0;
bool fin_flag=0;//�ܵ��Ƿ���� ��һ������
bool state1ok_flag=0;
bool ahead_flag=0;
bool scan_flag=0;
bool place_flag=0;
//����״̬��־λ
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
uint8 game_state=0; //5�����6���� 1Ϊ5,2Ϊ6
//����״̬��λ�� Vx Vy Vw �Ƕȣ� ex ��������ֵ  
//������x������  ��ǰΪy������ ˳ʱ��Ϊw������  
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
	float dis_all=(float)sqrt((double)(dis_x2*dis_x2 + dis_y2*dis_y2));  //���������֮��
	//�⻷2ms �ڻ�1ms
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
//���䷽ʽ����
void speeed_area_up(float now, float area, int stay)
{    float k_up;
	float  k_down;
	k_up=now/area;
	if(now<=area&&now>=0)                                  //�ٶȷֶη���
	velocity_global_lim=(int)(k_up*(velocity_global_lim_max-stay)+stay);
	else 
		velocity_global_lim=velocity_global_lim_max;
}
//�Ӽ��ٺ��� upΪ�������䣬downͬ��stayΪ���ٶ�
void speed_up_down(float state,float up,float down,int stay)
{    float k_up;
	float  k_down;
	k_up=(velocity_global_lim_max-stay)/(up*velocity_global_lim_max);            //����б��
	k_down=(velocity_global_lim_max-stay)/(down*velocity_global_lim_max);
	if(state<=up&&state>=0)                                  //�ٶȷֶη���
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
	k_up=(velocity_global_lim_max-stay)/up;            //����б��
	k_down=(velocity_global_lim_max-stay)/down;
	if(state<=up&&state>=0)                                  //�ٶȷֶη���
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
	//�ش�����
	//static uint16_t reply = 0;
	//ͨ��Э��
	
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
		else if((gpio_get_level(SW1))&&(!gpio_get_level(SW2)))   // off on   ��ʳ
		{
			six_div=1;
			game_state=2;
		}
		else if((!gpio_get_level(SW1))&&(gpio_get_level(SW2))) //on off     ˮ��
		{
			six_div=2;
			game_state=2;
		}
		else      //on on      �߲�
		{
			six_div=3;
			game_state=2;
		}
	velocity_global_lim_max=20;
	//ѡ��5�����6����
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
	
//	//�ȴ���ͼ����
	while(map_read[0][0] == 0 || map_read[0][1] == 0)
	{
		system_delay_ms(1);
	}
	//�������
	//�����������
	gpio_set_level(B11,1);
	for(int i=0;map_read[i][0]!=0 || map_read[i][1]!=0;i++)
	{
		map_sum++;
	}
//	//zean:����·���滮
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
	//Ŀǰ��ʼλ������Ϊ0
	//IMU.now_x = 30;
	//IMU.now_y = -30;
	state_control(1);  //�������  �ٰ�uart3�ľ�����������װ��
//    char text[15];	
//	for(int i=0;i<map_sum;i++)
//	{
//		sprintf(text,"%d %d\n",(map_read[i][0]+10)/20,(map_read[i][1]+10)/20);
//		wireless_uart_send_string(text);
//		system_delay_ms(5);
//	}
	//while(1);
	system_delay_ms(300);                  //���ͼ
    gpio_set_level(B11,0);
	system_delay_ms(200);                  //���ͼ
	
    go_next(10,60);       //����
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
			core_servo_move(my_arry[object_type[two_con-1]%10-1]);  //0��4
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
				core_servo_move(my_arry[object_type[two_con-1]%10]);  //1��5
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
		//���Դ�΢������ͷ
		
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
	//�ȴ�����
	find_flag = 0;
	system_delay_ms(100);
	adjust_car_jw_flag=1;
	if(find_flag==0)
	{
	start_back_y=Car.car_y;
	start_back_x=Car.car_x;
	}
	if(find_flag == 0)   //δ��������
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
	//�����Ա���  ��error0 ����error1
};
void make_up_error(bool dir)//1��0��
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
			//�¼ӵĶ��� �ڱ��߾����Ƕ�ʱ�������x��λ��
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
	Car.car_x = map_read[two_con][0] ;//���ڳ�ǰ��25cm
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
			core_servo_move(my_arry[object_type[two_con-1]%10-1]);  //0��4
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
				core_servo_move(my_arry[object_type[two_con-1]%10]);  //1��5
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
	
	else if(map_num==map_sum&&(map_sum-no_find)<place_all&&state_9_num!=3)//ûʰȡ��           ��ûȫʰȡ��Ҳ����
	{ 
		work_state=9;
		scan_get_flag=0;
		return;
	}
	
	uart_write_byte((UART_1),8);
	img_error[0]=0;img_error[1]=0;
	flag6=1;
	
	//��֤����������̫��
	go_next(0,350);
	while(1)
	{
		//�ȴ�����
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
		//�ȴ�����
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
			//�ȴ�����
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
	//�ȴ�����
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
	//�ȴ�����
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
	if(game_state==1)      //5����
	{				
	go_next(350,530);      //attention      //����  cl��530
	core_servo_move(my_arry[0]);
	while(1)
	{
		//�ȴ�����
		system_delay_ms(1);
		if(fin_flag)
		{
			//֪ͨ����
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
			//֪ͨ����
	
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
			//֪ͨ����
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
			//֪ͨ����
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
	if(game_state==2)      //6����
	{
		
	go_next(40,460);      //attention
	core_servo_move(my_arry[0]);
	while(1)
	{
		//�ȴ�����
		system_delay_ms(1);
		if(fin_flag)
		{
			//֪ͨ����
			ready_core(my_arry[0]);
    system_delay_ms(200);
    unload(my_arry[0]);	
			break;
		}
	}
	go_next(30,530);
	while(1)
	{
		//�ȴ�����
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
//		//�ȴ�����
//		system_delay_ms(1);
//		if(fin_flag)
//		{
//			//֪ͨ����
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
			//֪ͨ����
	
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
			//֪ͨ����
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
			//֪ͨ����
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
void state_5()         //�ؿ�
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
int16 start_place_y[3]={-15,155,345};//ÿ�κ���ɨͼ��ʼyֵ��  ���޸ģ�����ֵ
int16 start_place_x[3]={85,685,15};//ÿ�κ���ɨͼ��ʼxֵ��  ���޸ģ�����ֵ
int16 end_place_x[3]={685,15,685};//ÿ�κ���ɨͼ����xֵ��  ���޸ģ�����ֵ

//int16 start_place_y[3]={60,205,285};//ÿ�κ���ɨͼ��ʼyֵ��  ���޸ģ�����ֵ
//int16 start_place_x[3]={55,435,55};//ÿ�κ���ɨͼ��ʼxֵ��  ���޸ģ�����ֵ
//int16 end_place_x[3]={435,60,435};//ÿ�κ���ɨͼ����xֵ��  ���޸ģ�����ֵ
bool scan_stop_flag=0;
uint8 scan_num_time=0;
void state_9(int x,int y)
{	
	//1.����ʼɨͼ�ĵ�
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
	
	scan_get_flag=0;//ÿ����ǰ����0
	scan_num_time=0;
	scan_flag=1;//С���飺�������Ǹ�Ϊ��Ҫʱ�ٴ�����ͷ
	uart_rx_interrupt(UART_3, 1);  
	//gpio_set_level(B11,1);
	system_delay_ms(100);
	//gpio_set_level(B11,0);
	scan_stop_flag=0;//��������
//	sprintf(textx,"%d\n",(int)scan_flag);
//	wireless_uart_send_string(textx);
	img_error[0]=0;img_error[1]=0;
	go_next(end_place_x[state_9_num],start_place_y[state_9_num]);//zean attention  ���ر仯
	int8 old_map_sum=map_sum;
	while(1)
	{
		if(scan_get_flag==1)   //work_state==9ʱ�Żᱻ����
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
			if(state_9_num==3||(map_sum-no_find)>=place_all)//��ͼƬ©���ˣ�Ҳû�취ֻ�ܻؿ���
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
	//����ʱ���ӵ������
	scan_stop_flag=1;//ͣ��
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
	if(map_sum==old_map_sum) //������map_sum==old_map_sum
	{
		if(map_num==map_sum)
		state_9(last_place[0],last_place[1]);
		else
			work_state=1;
	}
	else //������
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