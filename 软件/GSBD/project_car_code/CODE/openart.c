#include "openart.h"
float img_error[2]={0};
float img_error_p[2]={0};
bool start_correct=0;
bool control_flag=0;
bool control_1_flag=0;
float angle_p=0;
int16 map_read[max_pot][2]={0};
int place_all=18;
//,{370,90},{330,370},{490,290},{590,170}};  //7,7  19,5  17,19  25,15  30,9  
//int16 map_read[max_pot][2]={0};//={{130,130},{410,90},{330,370},{490,290},{590,170}}; //={{130,130}};
//{{130,130}};
float scan_temp[max_pot][2]={0};
float scan_this[max_pot][2]={0};
uint8 scan_this_num[max_pot]={0};
int find_flag_num=0;
int error_count=0;
float swap_temp;
float cor_angle=0;
uint8 scan_num=0;
int cor_img=0;
int angle_time=0;
//{{130,130},{50,50},{130,130},{50,50},{130,130},{50,50},{130,130},{50,50},{130,130},{50,50},{130,130},{50,50},{130,130},{50,50},{130,130},{50,50},{130,130},{50,50},{130,130},{50,50}};
//{{90,70},{70,150},{130,270},{230,230}};
//{70,90},{50,250},{70,330},{150,290},{150,130},{230,350},{250,250},{230,90},{350,290},{410,190},{410,90},{450,350}};
//,{1,1},{200/1.414,200/1.414},{1,1},{200/1.414,200/1.414},{1,1}};
uint8 map_num=0;
uint8 map_sum=0;
uint8 object_type[max_pot] = {0};
uint8 reply_state=99;
uint8 start_correct_flag=0;
bool find_flag=0;
uint32 cnt_art=0;
uint32 cnt_art_p=0;
uint32 cnt_=0;
float jiux=0;
//uint8_t find_flag = 0;
uint8 two_con = 0; //二次识别次序控制
void openart_xy(uint8_t data)
{
	//通信协议
	uint8_t head[2] = {0xeb,0x90};
	uint8_t tail[2] = {0x0d,0x0a};
	//数据队列
	static uint8_t frame[100];
	static uint8_t last_data = 0;
	static uint8_t count = 0;
	static uint8_t receive_type = 0;
	static uint32_t start_time = 0;
	
	if(data == head[1] && last_data == head[0])
	{
		receive_type = 1;	
	}
	else if(receive_type == 1)
	{
		frame[count++] = data;
		
	}
	//接收到帧尾或超时结束
	if(receive_type == 1 && data == tail[1] && last_data == tail[0])
	{
		//检查
		if(count%2!=0)
		{
			//printf("receice_error_404\r\n");
			return;
		}

		//数据写入坐标数组
		for(uint8_t i=0,j=0;i<count-2;i=i+4,j++)
		{
			
			//x
			map_read[j][0] = frame[i]*100+frame[i+1];
			//y
			map_read[j][1] = frame[i+2]*100+frame[i+3];
		
		}
		
		//lcd_showuint16(1,1,get_time_ms() - start_time);
		//全部重置
		start_time = 0;
		receive_type = 0;
		last_data = 0;
		for(int i=0;i<count;i++)  frame[i] = 0;
		count = 0;
	}
    last_data = data;
} 

void openart_error(uint8_t data)
{
	//通信协议
	uint8_t head1[2] = {0xec,0x91};
	uint8_t tail[2] = {0x0d,0x0a};
	uint8_t head2[2] = {0xcf,0x66};
	//数据队列
	static uint8_t error[6];

	static uint8_t last_data = 0;
	static uint8_t count = 0;
	static uint8_t receive_type = 0;
	static uint8_t num = 0;
	if(work_state==5)
	{img_error[1]=0;
	img_error[0]=0;
	}
	if((data == head1[1] && last_data == head1[0])||(data == head2[1] && last_data == head2[0]&&work_state==5))
	{
		receive_type = 1;	
	}
	else if(receive_type == 1)
	{
		error[count++] = data;
	}
	//接收到帧尾或超时结束
	if(receive_type == 1 && data == tail[1] && last_data == tail[0])
	{
		//检查
		if(count%2!=0)
		{
			//printf("receice_error_404\r\n");
			return;
		}
		if(work_state==2)
		{//数据写入数组
		//x
		img_error[0] = (float)(error[0]%10*100+error[1])/10.f;
		if(error[0] >= 10) img_error[0] = -img_error[0];
		//y
		img_error[1] = (float)(error[2]%10*100+error[3])/10.f;
		if(error[2] >= 10) img_error[1] = -img_error[1];
		}
		if(work_state==5)
		{//数据写入数组
		//x
		img_error[0] = (float)(error[0]%10*100+error[1])/10.f;
		if(error[0] >= 10) img_error[0] = -img_error[0];
		//y
		img_error[1] = (float)(error[2]%10*100+error[3])/10.f;
		if(error[2] >= 10) img_error[1] = -img_error[1];
		cor_img=img_error[0]+20;        //attention
		}
		receive_type = 0;
		last_data = 0;
		for(int i=0;i<count;i++)  error[i] = 0;
		count = 0;
		//刷新接收error
		//error_flag = 1;
		//看到标志
		//find_flag_num++;
		//if(find_flag_num>1)
		find_flag = 1;
//	char text[35];
//		sprintf(text,"img_error[0]:%d,img_error[1]:%d\n",img_error[0],img_error[1]);
//	wireless_uart_send_string(text);
		
		//find_flag_num=0; 
		control_flag=1;
		control_1_flag=1;
	}
    last_data = data;
}

void openart2_receive(uint8_t data){
	//通信协议
	uint8_t head[2] = {0xed,0x92};
	static uint8_t last_data = 0;
	static uint8_t receive_type = 0;
	if(last_data == head[0] && data == head[1]){
		receive_type = 1;
	
	}
	else if(receive_type == 1){
		//回传的状态
		reply_state = data;
		receive_type = 0;
		last_data = 0;
	}
	last_data = data;
}

void openart_type(uint8_t data)
{
	//通信协议
	uint8_t head[2] = {0xef,0x93};
	static uint8_t last_data = 0;
	static uint8_t receive_type = 0;
	if(last_data == head[0] && data == head[1])
		{
		receive_type = 1;
	    }
	else if(receive_type == 1){
		//two_con为当前所在点数
		object_type[two_con] = data;
		receive_type = 0;
		last_data = 0;
		flag_nothong=1;
	}
	last_data = data;
}

//float xkpid[3] = {0.05f,0.06f,0.f};
//float ykpid[3] = {0.05f,0.06f,0.f};
//接近图片，前摄像头
void approach_img()
{
	
	static float vel_p[2]={0};
	//异常值舍去
	if(abs(img_error[0]) > 70 || abs(img_error[1]) > 70)  return;
	
	float vel_x,vel_y;
//	if (fabs_(img_error[0])<=1.5)
//	{img_error[0]=0;
//	}
//	if (fabs_(img_error[1])<=1.5)
//	{img_error[1]=0;
//	}

	
//	if (fabs_(img_error[1])<=10) 
//	   {float a=1.2;
//		if(new_common_pid(&pid7,img_error[1])>0)
//			vel_y=new_common_pid(&pid7,img_error[1])+a;
//		else
//			vel_y=new_common_pid(&pid7,img_error[1])-a;
//		}
//	else vel_y=new_common_pid(&pid7,img_error[1]);
//		if (fabs_(img_error[0])<=10) 
//	   {float a=1.2;
//		if(new_common_pid(&pid6,img_error[0])>0)
//			vel_x=new_common_pid(&pid6,img_error[0])+a;
//		else
//			vel_x=new_common_pid(&pid6,img_error[0])-a;
//		}
//	else vel_x=new_common_pid(&pid6,img_error[0]);
vel_y=new_common_pid(&pid7,img_error[1]);

 vel_x=new_common_pid(&pid6,img_error[0]);
	//jiux=vel_x;
	//slow_down(img_error[0],img_error[1]);
//	vel_x=nolinar_error(1.5,img_error[0]);
//	vel_y=nolinar_error(1.5,img_error[1]);
	vel_x=0.7*vel_x+0.3*vel_p[0];
	vel_y=0.7*vel_y+0.3*vel_p[1];
	Car.ex_vel_x=limit(vel_x,12);
	Car.ex_vel_y=limit(vel_y,12);
	vel_p[0]=vel_x;vel_p[1]=vel_y;
	
	
//下面的没看懂
//	img_error[0] -= IMU.encc_x/54.76f * pit0_time/1000.0f;
//	img_error[1] -= IMU.encc_y/54.76f * pit0_time/1000.0f;
//	vel_x =  (1.7,img_error[0]);
//	vel_y = nolinar_error(1.8,img_error[1]);

}
void approach_img_back()
{
	
	static float vel_p[2]={0};
	//异常值舍去
	if(abs(img_error[0]) > 40 || abs(img_error[1]) > 40)  return;
	
	float vel_x,vel_y;
//	if (fabs_(img_error[0])<=1.5)
//	{img_error[0]=0;
//	}
//	if (fabs_(img_error[1])<=1.5)
//	{img_error[1]=0;
//	} 
	
	vel_x=new_common_pid(&pid8,cor_img);
	if(Car.car_y>=10)	
		vel_y=-8;
	else
		vel_y=-5;
	//jiux=vel_x;
	//slow_down(img_error[0],img_error[1]);
	vel_x=0.7*vel_x+0.3*vel_p[0];
	vel_y=0.7*vel_y+0.3*vel_p[1];
	Car.ex_vel_x=limit(vel_x,6);
	Car.ex_vel_y=limit(vel_y,15);
	vel_p[0]=vel_x;vel_p[1]=vel_y;

}

void openart_non(uint8_t data)
{
	//通信协议
	uint8_t head[2] = {0xeb,0x90};
	uint8_t tail[2] = {0x0d,0x0a};
	//数据队列
	static uint8_t frame[100];
	static uint8_t last_data = 0;
	static uint8_t count = 0;
	static uint8_t receive_type = 0;
	static uint32_t start_time = 0;
	uint8_t tem_buff[max_pot][2]={0};
	static uint8_t tem_buff_p[max_pot][2]={0};
	
	if(data == head[1] && last_data == head[0])
	{
		receive_type = 1;	
	}
	else if(receive_type == 1)
	{
		frame[count++] = data;
		
	}
	//接收到帧尾或超时结束
	if(receive_type == 1 && data == tail[1] && last_data == tail[0])
	{
		//检查
		if(count%2!=0)
		{
			//printf("receice_error_404\r\n");
			return;
		}

		//数据写入暂存数组
		for(uint8_t i=0,j=0;i<count-2;i=i+4,j++)
		{
			
			//x
			tem_buff[j][0] = frame[i]*100+frame[i+1];
			//y
			tem_buff[j][1] = frame[i+2]*100+frame[i+3];
		
		}
		
		//lcd_showuint16(1,1,get_time_ms() - start_time);
		//全部重置
		//与有框图进行比较
		
		start_time = 0;
		receive_type = 0;
		last_data = 0;
		for(int i=0;i<count;i++)  frame[i] = 0;
		count = 0;
	}
    last_data = data;
} 
//void openart_scan(uint8_t data)//扫描
//{
//	//通信协议
//	uint8_t head[2] = {0xeb,0x90};
//	uint8_t tail[2] = {0x0d,0x0a};
//	//数据队列
//	static uint8_t frame[100];
//	static uint8_t last_data = 0;
//	static uint8_t count = 0;
//	static uint8_t countx = 0;
//	static uint8_t receive_type = 0;
//	static uint32_t start_time = 0;
//	
//	if(data == head[1] && last_data == head[0])
//	{
//		
//		start_time = 0;
//		receive_type = 0;
//		last_data = 0;
//		for(int i=0;i<count;i++)  frame[i] = 0;
//		scan_num=countx;
//		count = 0;
//		countx=0;
//		receive_type = 1;	
//	}
//	else if(receive_type == 1)
//	{
//		frame[count++] = data;
//		
//	}
//	//接收到帧尾或超时结束
//	if(receive_type == 1 && data == tail[1] && last_data == tail[0])
//	{
//		//检查
//		if(count%2!=0)
//		{
//			//printf("receice_error_404\r\n");
//			return;
//		}
//		char testx[25];
//		//数据写入坐标数组
//		for(uint8_t i=0,j=0;i<count-2;i=i+4,j++)
//		{
//			
//			//x
//			scan_temp[j][0] = (float)(frame[i]%10*100+frame[i+1]);
//			if(frame[i]>=10)scan_temp[j][0]=-scan_temp[j][0];
//			//
//			//sprintf(testx,"%f ",scan_temp[j][0]);
//			//wireless_uart_send_string(testx);
//			scan_temp[j][0]+=Car.car_x;//改
//			//y
//			scan_temp[j][1] = (float)(frame[i+2]%10*100+frame[i+3]);
//			if(frame[i+2]>=10)scan_temp[j][1]=-scan_temp[j][1];
//			//
//			//sprintf(testx,"%f\n",scan_temp[j][1]);
//			//wireless_uart_send_string(testx);
//			scan_temp[j][1]+=Car.car_y;//改    
//			countx++;
//			if(scan_temp[j][0]<-1.0||scan_temp[j][0]>701.0||scan_temp[j][1]<-1.0||scan_temp[j][1]>501.00) //正式比赛改 attention
//				countx--,j--,error_count++;
//		}
//		//wireless_uart_send_string("\n");
//		//lcd_showuint16(1,1,get_time_ms() - start_time);
//		//全部重置
//		start_time = 0;
//		receive_type = 0;
//		last_data = 0;
//		for(int i=0;i<count;i++)  frame[i] = 0;
//		scan_num=countx;
//		count = 0;
//		countx=0;
//		int flag_jw=0;
//		for(int i=0;i<scan_num;i++)
//		{
//		for(int j=0;j<map_sum;j++)
//		{
//			if(((fabs(scan_temp[i][0]-map_read[j][0])<30)&&(fabs(scan_temp[i][1]-map_read[j][1])<30)))
//			{
//				flag_jw=1;
//			}
//				
//		}
//		if (flag_jw==0)
//		{
//			if ((700>=scan_temp[i][0])&&(500>=scan_temp[i][1]))        //attention 跟场地有关
//			{
//			for (int m = map_sum - 1; m >= (map_num+1); m--)
//				{
//				map_read[m+1][0] = map_read[m][0];
//				map_read[m+1][1] = map_read[m][1];
//				}
//				map_read[map_num+1][0]=scan_temp[i][0];
//				map_read[map_num+1][1]=scan_temp[i][1];
//				map_sum++;}
//		}
//	}
//	}
//    last_data = data;
//	
//}
void openart_dis_angle(uint8 data)
{
		//通信协议
	uint8_t head1[2] = {0xab,0x99};
	uint8_t tail[2] = {0x0d,0x0a};
	//数据队列
	static uint8_t error[8];
	static uint8_t last_data = 0;
	static uint8_t count = 0;
	static uint8_t receive_type = 0;
	static uint8_t num = 0;
	if(data == head1[1] && last_data == head1[0])
	{
		receive_type = 1;	
		
	}
	else if(receive_type == 1)
	{
		error[count++] = data;
	}
	//接收到帧尾或超时结束
	if(receive_type == 1 && data == tail[1] && last_data == tail[0])
	{
		//检查
		if(count%2!=0)
		{
			//printf("receice_error_404\r\n");
			return;
		}
		
		//if(work_state==8)
		//{//数据写入数组
		//x
		img_error[0] = (float)(error[0]%10*100+error[1])/10.f;
		if(error[0] >= 10) img_error[0] = -img_error[0];
		//y
		img_error[1] = (float)(error[2]%10*100+error[3])/10.f;
		if(error[2] >= 10) img_error[1] = -img_error[1];
			
//		cor_angle = (float)(error[4]%10*100+error[5])/10.f;
//		if(error[4] >= 10) cor_angle = -cor_angle;
//		}
		receive_type = 0;
		last_data = 0;
		for(int i=0;i<count;i++)  error[i] = 0;
		count = 0;
		if(flag8)
		{
		Car.ex_vel_x=limit(new_common_pid(&pid9,img_error[0]),10);
		}
		//}
		if(fabs(img_error[0])<1.0&&work_state==8)
			{	
			flag7++;			
//			
//			real_angle=0.7*cor_angle+0.3*angle_p;
//			angle_p=real_angle;gpio_set_level(B11,0);
			}
		
	}
	 last_data = data;
}

void openart_jiaodu(uint8 data)
{
		//通信协议
	uint8_t head3[2] = {0xaa,0xbb};
	uint8_t tail[2] = {0x0d,0x0a};
	//数据队列
	static uint8_t error[8];
	static uint8_t last_data = 0;
	static uint8_t count = 0;
	static uint8_t receive_type = 0;
	static uint8_t num = 0;
	if(data == head3[1] && last_data == head3[0])
	{
		receive_type = 1;	
	}
	else if(receive_type == 1)
	{
		error[count++] = data;
	}
	//接收到帧尾或超时结束
	if(receive_type == 1 && data == tail[1] && last_data == tail[0])
	{
		//检查
		if(count%2!=0)
		{
			//printf("receice_error_404\r\n");
			return;
		}
	//	gpio_set_level(B11,1);
		if(work_state==8)
		{//数据写入数组
		cor_angle = (float)(error[0]%10*100+error[1])/10.f;
		if(error[0] >= 10) cor_angle = -cor_angle;
		}
		receive_type = 0;
		last_data = 0;
		for(int i=0;i<count;i++)  error[i] = 0;
		count = 0;		
	    real_angle=cor_angle;
		//0.7*cor_angle+0.3*angle_p;
	    angle_p=real_angle;
	}
	 last_data = data;
}
void openart_scan(uint8_t data)//扫描
{
	//通信协议
	uint8_t head[2] = {0xeb,0x90};
	uint8_t tail[2] = {0x0d,0x0a};
	//数据队列
	static uint8_t frame[100];
	static uint8_t last_data = 0;
	static uint8_t count = 0;
	static uint8_t countx = 0;
	static uint8_t receive_type = 0;
	static uint32_t start_time = 0;
	
	if(data == head[1] && last_data == head[0])
	{
		
		start_time = 0;
		receive_type = 0;
		last_data = 0;
		for(int i=0;i<count;i++)  frame[i] = 0;
		scan_num=countx;
		count = 0;
		countx=0;
		receive_type = 1;	
	}
	else if(receive_type == 1)
	{
		frame[count++] = data;
		
	}
	//接收到帧尾或超时结束
	if(receive_type == 1 && data == tail[1] && last_data == tail[0])
	{
		//检查
		if(count%2!=0)
		{
			//printf("receice_error_404\r\n");
			return;
		}
		char testx[25];
		//数据写入坐标数组
		for(uint8_t i=0,j=0;i<count-2;i=i+4,j++)
		{
			
			//x
			scan_temp[j][0] = (float)(frame[i]%10*100+frame[i+1]);
			if(frame[i]>=10)scan_temp[j][0]=-scan_temp[j][0];
			//
			//sprintf(testx,"%f ",scan_temp[j][0]);
			//wireless_uart_send_string(testx);
			scan_temp[j][0]+=Car.car_x;//改
			//y
			scan_temp[j][1] = (float)(frame[i+2]%10*100+frame[i+3]);
			if(frame[i+2]>=10)scan_temp[j][1]=-scan_temp[j][1];
			//
			//sprintf(testx,"%f\n",scan_temp[j][1]);
			//wireless_uart_send_string(testx);
			scan_temp[j][1]+=Car.car_y;//改    
			countx++;
			if(scan_temp[j][0]<5.0||scan_temp[j][0]>695.0||scan_temp[j][1]<5.0||scan_temp[j][1]>495.00) //正式比赛改        attention
				countx--,j--,error_count++;
		}
		//全部重置
		start_time = 0;
		receive_type = 0;
		last_data = 0;
		for(int i=0;i<count;i++)  frame[i] = 0;
		scan_num=countx;
		count = 0;
		countx=0;
		
//		sprintf(testx,"num:%d\n",scan_num);
//		wireless_uart_send_string(testx);
		
		
		if(work_state!=9||(work_state==9&&scan_stop_flag))//增加扫图停下找框
		{
			int flag_jw=0;
			for(int i=0;i<scan_num;i++)
			{
				for(int j=map_num;j<map_sum;j++)//j=0改到j=map_num   增加剪枝
				{
					//if(((fabs(scan_temp[i][0]-(float)map_read[j][0])<30.0)&&(fabs(scan_temp[i][1]-(float)map_read[j][1])<30.0)))//60
						if(((fabs(scan_temp[i][0]-map_read[j][0])<40.0)&&(fabs(scan_temp[i][1]-map_read[j][1])<40.0)))
					{
						flag_jw=1;
						break;//zean!!!!!!!!!!!!!!!剪枝
					}
					
				}
				if (flag_jw==0)
				{
				//	sprintf(testx,"state:%d x:%d y:%d  ac：%d %d\n",work_state,(int)scan_temp[i][0],(int)scan_temp[i][1],(int)Car.car_x,(int)Car.car_y);
				//	wireless_uart_send_string(testx);
					if(work_state!=9)zean_flag++;
					if(map_sum!=map_num)
					{
						if(work_state==9)
						{
							for (int m = (int)map_sum - 1; m >= (int)map_num; m--)
							{
								map_read[m+1][0] = map_read[m][0];
								map_read[m+1][1] = map_read[m][1];
							}
							map_read[map_num][0]=(scan_temp[i][0]);
							map_read[map_num][1]=(scan_temp[i][1]);
							map_sum++;
								
							
						}	
						else
						{
							for (int m = (int)map_sum - 1; m >= (int)(map_num+1); m--)
							{
								map_read[m+1][0] = map_read[m][0];
								map_read[m+1][1] = map_read[m][1];
							}
							map_read[map_num+1][0]=(scan_temp[i][0]);
							map_read[map_num+1][1]=(scan_temp[i][1]);
							//map_read[map_num+1][0]=((int)scan_temp[i][0]+10+10)/20*20-10;
							//map_read[map_num+1][1]=((int)scan_temp[i][1]+10+10)/20*20-10;
							map_sum++;
						}
						
						
					}
					else
					{
						//map_read[map_num][0]=((int)scan_temp[i][0]+10+10)/20*20-10;
						//map_read[map_num][1]=((int)scan_temp[i][1]+10+10)/20*20-10;
						map_read[map_num][0]=(scan_temp[i][0]);
						map_read[map_num][1]=(scan_temp[i][1]);
						map_sum++;
					
					}
						
					
				}
				flag_jw=0;//zean,结尾没置零！！！！！
			}
		}
		else if(work_state==9&&scan_get_flag==0)
		{
			if(scan_num!=0)
			{
				scan_num_time++;
				if(scan_num_time==1)
				{
					scan_get_flag=1;
				}
			
			}
		}


	}
    last_data = data;
}
void slow_down(float x,float y)
{   if((float)sqrt((double)(x*x + y))<=1)
	{img_error[0]=0;
	img_error[1]=0;}
	else if(((float)sqrt((double)(x*x + y*y))>1)&&((float)sqrt((double)(x*x + y*y))<=3))
	{img_error[0]=0.5*x;
	img_error[1]=0.5*y;}
	else
	{img_error[0]=x;
	img_error[1]=y;}
}
float nolinar_error(float k,float error)
	{
	//非线性
	int8_t k1 = 1;
	if(error < 0)
	{		
		k1= -k1;
		k = -k;
	}
	error = log(error*k1+1)*k;
	return error;
}