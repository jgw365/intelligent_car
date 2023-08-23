#include "encoder.h"


//编码器状态  dis记录距离  vel记录编码器返回速度   暂时没用
_Encoder encoder1 = {.dis=0,.vel=0},encoder2={.dis=0,.vel=0},\
            encoder3={.dis=0,.vel=0},encoder4={.dis=0,.vel=0};
bool ready_enc=0;
//用来计时
int16 encoder_data_1 = 0;
int16 encoder_data_2 = 0;
int16 encoder_data_3 = 0;
int16 encoder_data_4 = 0;
uint32 time_rem_1=0;
uint32 time_rem_last=0;
uint32 time_pass=0;
			
void encoder_init()
{
    encoder_dir_init(ENCODER_1, ENCODER_1_LSB, ENCODER_1_DIR);                  // 初始化编码器模块与引脚 方向编码器模式
    encoder_dir_init(ENCODER_2, ENCODER_2_LSB, ENCODER_2_DIR);                  // 初始化编码器模块与引脚 方向编码器模式
    encoder_dir_init(ENCODER_3, ENCODER_3_LSB, ENCODER_3_DIR);                  // 初始化编码器模块与引脚 方向编码器模式
    encoder_dir_init(ENCODER_4, ENCODER_4_LSB, ENCODER_4_DIR);                  // 初始化编码器模块与引脚 方向编码器模式  
}

//单位时间的脉冲数
void get_wheel_vel(void){
	
	//编码器计时部分
	
	static float last_enc_value[4] = {0,0,0,0};//上次enc 用于低通
	//获取编码器脉冲计数和计数器
	static int32 cnt=1;
	cnt++;
	if (cnt==100){gpio_set_level(BEEP_PIN,1);}
	if (cnt==110){gpio_set_level(BEEP_PIN,0);}
	if (cnt>100)//去除初始波动编码器值
	{   
		ready_enc=1;
		encoder_data_1 = -encoder_get_count(ENCODER_4);                              // 获取编码器计数
		encoder_data_2 = encoder_get_count(ENCODER_3); 
		encoder_data_3 = encoder_get_count(ENCODER_2);  
		encoder_data_4 = -encoder_get_count(ENCODER_1);
	}	
	else 
	{
		encoder_data_1 =0;                              // 获取编码器计数
		encoder_data_2 =0; 
		encoder_data_3 =0;  
		encoder_data_4 =0;
	}
	encoder_clear_count(ENCODER_1); 
    encoder_clear_count(ENCODER_2);       
    encoder_clear_count(ENCODER_3);  
    encoder_clear_count(ENCODER_4);   
	//别处用可以用下面两行暂时代替 但潜在bug
//    time_pass=time_rem_1-time_rem_last;
//	time_rem_last=time_rem_1;
	//计算速度 即单位时间脉冲叔
	encoder1.vel=(float)(encoder_data_1*k_enc+(1-k_enc)*last_enc_value[0]);
	encoder2.vel=(float)(encoder_data_2*k_enc+(1-k_enc)*last_enc_value[1]);
	encoder3.vel=(float)(encoder_data_3*k_enc+(1-k_enc)*last_enc_value[2]);
	encoder4.vel=(float)(encoder_data_4*k_enc+(1-k_enc)*last_enc_value[3]);
	motor1.vel_ac=encoder1.vel/(10);  //5.20 是否与中断时间有关
	motor2.vel_ac=encoder2.vel/(10);
	motor3.vel_ac=encoder3.vel/(10);
	motor4.vel_ac=encoder4.vel/(10);
	
	last_enc_value[0]=encoder1.vel;
	last_enc_value[1]=encoder2.vel;
	last_enc_value[2]=encoder3.vel;
	last_enc_value[3]=encoder4.vel;
//  计算距离
    motor1.dis=(encoder1.vel*0.00828)*50/35.4;            //M车轮直径6.3cm，齿轮数70，编码器齿轮数30；19.782/(1024*2.333)
	motor2.dis=(encoder2.vel*0.00828)*50/35.4;
	motor3.dis=(encoder3.vel*0.00828)*50/35.4;
	motor4.dis=(encoder4.vel*0.00828)*50/35.4;
	
}