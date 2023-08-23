#include "encoder.h"


//������״̬  dis��¼����  vel��¼�����������ٶ�   ��ʱû��
_Encoder encoder1 = {.dis=0,.vel=0},encoder2={.dis=0,.vel=0},\
            encoder3={.dis=0,.vel=0},encoder4={.dis=0,.vel=0};
bool ready_enc=0;
//������ʱ
int16 encoder_data_1 = 0;
int16 encoder_data_2 = 0;
int16 encoder_data_3 = 0;
int16 encoder_data_4 = 0;
uint32 time_rem_1=0;
uint32 time_rem_last=0;
uint32 time_pass=0;
			
void encoder_init()
{
    encoder_dir_init(ENCODER_1, ENCODER_1_LSB, ENCODER_1_DIR);                  // ��ʼ��������ģ�������� ���������ģʽ
    encoder_dir_init(ENCODER_2, ENCODER_2_LSB, ENCODER_2_DIR);                  // ��ʼ��������ģ�������� ���������ģʽ
    encoder_dir_init(ENCODER_3, ENCODER_3_LSB, ENCODER_3_DIR);                  // ��ʼ��������ģ�������� ���������ģʽ
    encoder_dir_init(ENCODER_4, ENCODER_4_LSB, ENCODER_4_DIR);                  // ��ʼ��������ģ�������� ���������ģʽ  
}

//��λʱ���������
void get_wheel_vel(void){
	
	//��������ʱ����
	
	static float last_enc_value[4] = {0,0,0,0};//�ϴ�enc ���ڵ�ͨ
	//��ȡ��������������ͼ�����
	static int32 cnt=1;
	cnt++;
	if (cnt==100){gpio_set_level(BEEP_PIN,1);}
	if (cnt==110){gpio_set_level(BEEP_PIN,0);}
	if (cnt>100)//ȥ����ʼ����������ֵ
	{   
		ready_enc=1;
		encoder_data_1 = -encoder_get_count(ENCODER_4);                              // ��ȡ����������
		encoder_data_2 = encoder_get_count(ENCODER_3); 
		encoder_data_3 = encoder_get_count(ENCODER_2);  
		encoder_data_4 = -encoder_get_count(ENCODER_1);
	}	
	else 
	{
		encoder_data_1 =0;                              // ��ȡ����������
		encoder_data_2 =0; 
		encoder_data_3 =0;  
		encoder_data_4 =0;
	}
	encoder_clear_count(ENCODER_1); 
    encoder_clear_count(ENCODER_2);       
    encoder_clear_count(ENCODER_3);  
    encoder_clear_count(ENCODER_4);   
	//���ÿ���������������ʱ���� ��Ǳ��bug
//    time_pass=time_rem_1-time_rem_last;
//	time_rem_last=time_rem_1;
	//�����ٶ� ����λʱ��������
	encoder1.vel=(float)(encoder_data_1*k_enc+(1-k_enc)*last_enc_value[0]);
	encoder2.vel=(float)(encoder_data_2*k_enc+(1-k_enc)*last_enc_value[1]);
	encoder3.vel=(float)(encoder_data_3*k_enc+(1-k_enc)*last_enc_value[2]);
	encoder4.vel=(float)(encoder_data_4*k_enc+(1-k_enc)*last_enc_value[3]);
	motor1.vel_ac=encoder1.vel/(10);  //5.20 �Ƿ����ж�ʱ���й�
	motor2.vel_ac=encoder2.vel/(10);
	motor3.vel_ac=encoder3.vel/(10);
	motor4.vel_ac=encoder4.vel/(10);
	
	last_enc_value[0]=encoder1.vel;
	last_enc_value[1]=encoder2.vel;
	last_enc_value[2]=encoder3.vel;
	last_enc_value[3]=encoder4.vel;
//  �������
    motor1.dis=(encoder1.vel*0.00828)*50/35.4;            //M����ֱ��6.3cm��������70��������������30��19.782/(1024*2.333)
	motor2.dis=(encoder2.vel*0.00828)*50/35.4;
	motor3.dis=(encoder3.vel*0.00828)*50/35.4;
	motor4.dis=(encoder4.vel*0.00828)*50/35.4;
	
}