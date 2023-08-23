#include "imu.h"
IMU_ IMU; 
float show_debug=0;
float show_debug1=0;
 
float fabs_(float a){
	return (float)(fabs((double) a));
}

void imu_init()
{
	IMU.acc_x = 0;
	IMU.acc_y = 0;
	IMU.acc_z = 0;
	//���ٶ�
	IMU.gyro_x = 0;
	IMU.gyro_y = 0;
	IMU.gyro_z = 0;
	//�Ƕ�
	IMU.Pitch = 0;
	IMU.Yaw = 0;
	IMU.Roll = 0;
	//��������
	IMU.ex_yaw = 0;
	//����λ��
	IMU.now_x = 0;
	IMU.now_y = 0;
	//�ǶȻ�pid����14
	//float kpid_out[3] = {0.79f,0.f,3.f};   //ת�Ƕ�ģʽʱ��Ĳ���  �޷�Ϊ3  k_x_y 5 
	float kpid_out[3] = {1.5,0,18}; //0.06 ���񻹲��� k_x_y 5 ʱ�� �޷�Ϊ3  0.95 5     1msʱΪ1.25��0��65
	float kpid_in[3] = {0.0f,0.f,0.f};
	IMU.kpid_in[0] = kpid_in[0];
	IMU.kpid_in[1] = kpid_in[1];
	IMU.kpid_in[2] = kpid_in[2];
	IMU.kpid_out[0] = kpid_out[0];
	IMU.kpid_out[1] = kpid_out[1];
	IMU.kpid_out[2] = kpid_out[2];
	//imu�Լ�
	for(uint16_t i = 0;i<1000;i++)
	{
		imu660ra_get_gyro();     
		imu660ra_get_acc(); 
		//icm20602_get_gyro();
		//icm20602_get_acc();
		IMU.gyro_bias += imu660ra_gyro_z;
		IMU.acc_x_bias += imu660ra_acc_x;
		IMU.acc_y_bias += imu660ra_acc_y;
//		IMU.gyro_bias += icm20602_gyro_z;
//		IMU.acc_x_bias += icm20602_acc_x;
//		IMU.acc_y_bias += icm20602_acc_y;
		system_delay_ms(2);
		
	}
    IMU.gyro_bias /= 1000.f;
	//IMU.gyro_bias += 0.27f;//�����  ��ʱע��
	IMU.acc_x_bias /= 1000.f;
	IMU.acc_y_bias /= 1000.f;

}
void imu_init_20602()
{
	IMU.acc_x = 0;
	IMU.acc_y = 0;
	IMU.acc_z = 0;
	//���ٶ�
	IMU.gyro_x = 0;
	IMU.gyro_y = 0;
	IMU.gyro_z = 0;
	//�Ƕ�
	IMU.Pitch = 0;
	IMU.Yaw = 0;
	IMU.Roll = 0;
	//��������
	IMU.ex_yaw = 0;
	//����λ��
	IMU.now_x = 0;
	IMU.now_y = 0;
	//�ǶȻ�pid����14
	//float kpid_out[3] = {0.79f,0.f,3.f};   //ת�Ƕ�ģʽʱ��Ĳ���  �޷�Ϊ3  k_x_y 5 
	float kpid_out[3] = {1.5,0,18}; //0.06 ���񻹲��� k_x_y 5 ʱ�� �޷�Ϊ3  0.95 5     1msʱΪ1.25��0��65
	float kpid_in[3] = {0.0f,0.f,0.f};
	IMU.kpid_in[0] = kpid_in[0];
	IMU.kpid_in[1] = kpid_in[1];
	IMU.kpid_in[2] = kpid_in[2];
	IMU.kpid_out[0] = kpid_out[0];
	IMU.kpid_out[1] = kpid_out[1];
	IMU.kpid_out[2] = kpid_out[2];
	//imu�Լ�
	for(uint16_t i = 0;i<1000;i++)
	{
		//imu660ra_get_gyro();     
		//imu660ra_get_acc(); 
		icm20602_get_gyro();
		icm20602_get_acc();
//		IMU.gyro_bias += imu660ra_gyro_z;
//		IMU.acc_x_bias += imu660ra_acc_x;
//		IMU.acc_y_bias += imu660ra_acc_y;
		IMU.gyro_bias += icm20602_gyro_z;
		IMU.acc_x_bias += icm20602_acc_x;
		IMU.acc_y_bias += icm20602_acc_y;
		system_delay_ms(2);
		
	}
    IMU.gyro_bias /= 1000.f;
	//IMU.gyro_bias += 0.27f;//�����  ��ʱע��
	IMU.acc_x_bias /= 1000.f;
	IMU.acc_y_bias /= 1000.f;

}

void cal_rollAngle()
{
	//ȡֵ���ʱ��
	static uint32 dt_ = 0;
	int32 dt;
	static int16 gyro_z_p = 0;
	static int16 acc_y_p = 0;
	//��������ȡֵ���
//	dt = timer_get(GPT_TIM_1) ;
//	timer_clear(GPT_TIM_1);
	imu660ra_get_gyro();
	//icm20602_get_gyro();
	
	
	imu660ra_get_acc();
	IMU.gyro_z = -(imu660ra_gyro_z - IMU.gyro_bias);     //M:21.85   H:1.769
	//IMU.gyro_z = -(icm20602_gyro_z - IMU.gyro_bias);     //M:21.85   H:1.769
	
	IMU.acc_y = imu660ra_acc_y - IMU.acc_y_bias;
	//��ͨ�˲�
	float alpha = 0.7;
	//IMU.acc_y = IMU.acc_y*alpha + (1-alpha)*acc_y_p;
	IMU.gyro_z = IMU.gyro_z*alpha + (1-alpha)*gyro_z_p;
	gyro_z_p = IMU.gyro_z;
	//acc_y_p = IMU.acc_y;
	//IMU.gyro_z = kalmanFilter_filter(kf+5,IMU.gyro_z);
	IMU.gyro_z = imu660ra_gyro_transition(IMU.gyro_z);
	//IMU.gyro_z = icm20602_gyro_transition(IMU.gyro_z);
	
	//IMU.acc_y = imu660ra_acc_transition(IMU.acc_y);
	//���ֳ��Ƕ�
	IMU.Yaw +=  -IMU.gyro_z*0.005;           //5.20 �Ƿ��ж�ʱ��Ļ���
	//IMU.now_y += IMU.acc_y*0.001;
	if(IMU.Yaw >= 360) 	IMU.Yaw -= 360;
	if(IMU.Yaw < 0 )   IMU.Yaw = 360 + IMU.Yaw;
	Car.car_yaw=IMU.Yaw;
//	if (Car.car_yaw<300){show_debug1=Car.car_yaw;}
//	else{show_debug1=Car.car_yaw-360;}
}
void cal_rollAngle_20602()
{
	//ȡֵ���ʱ��
	static uint32 dt_ = 0;
	int32 dt;
	static int16 gyro_z_p = 0;
	static int16 acc_y_p = 0;
	//��������ȡֵ���
//	dt = timer_get(GPT_TIM_1) ;
//	timer_clear(GPT_TIM_1);
	//imu660ra_get_gyro();
	icm20602_get_gyro();
	
	
	//imu660ra_get_acc();
	icm20602_get_acc();
	//IMU.gyro_z = -(imu660ra_gyro_z - IMU.gyro_bias);     //M:21.85   H:1.769
	IMU.gyro_z = -(icm20602_gyro_z - IMU.gyro_bias);     //M:21.85   H:1.769
	
	IMU.acc_y = icm20602_acc_y - IMU.acc_y_bias;
	//��ͨ�˲�
	float alpha = 0.7;
	IMU.acc_y = IMU.acc_y*alpha + (1-alpha)*acc_y_p;
	IMU.gyro_z = IMU.gyro_z*alpha + (1-alpha)*gyro_z_p;
	gyro_z_p = IMU.gyro_z;
	acc_y_p = IMU.acc_y;
	//IMU.gyro_z = kalmanFilter_filter(kf+5,IMU.gyro_z);
	//IMU.gyro_z = imu660ra_gyro_transition(IMU.gyro_z);
	IMU.gyro_z = icm20602_gyro_transition(IMU.gyro_z);
	
	IMU.acc_y = icm20602_acc_transition(IMU.acc_y);
	//���ֳ��Ƕ�
	IMU.Yaw +=  IMU.gyro_z*0.005;           //5.20 �Ƿ��ж�ʱ��Ļ���
	//IMU.now_y += IMU.acc_y*0.001;
	if(IMU.Yaw >= 360) 	IMU.Yaw -= 360;
	if(IMU.Yaw < 0 )   IMU.Yaw = 360 + IMU.Yaw;
	Car.car_yaw=IMU.Yaw;
//	if (Car.car_yaw<300){show_debug1=Car.car_yaw;}
//	else{show_debug1=Car.car_yaw-360;}
}
float turn_Yaw()
{   
	//�⻷λ��ʽ
	if(IMU.ex_yaw >= 360) 	IMU.ex_yaw -= 360;
	if(IMU.ex_yaw < 0 )   IMU.ex_yaw = 360 + IMU.ex_yaw;
	float *kpid_out = IMU.kpid_out;
	//�ڻ�����ʽ
	float *kpid_in = IMU.kpid_in;
	static float error_p_out = 0.f;
//	static float error_p_p_out = 0.f;
	static float d_out = 0.f;
	static float d_out_p = 0.f;
	static float pid_value = 0.f;
	float error = 0.f;
	float dpid = 0.f;
	//����������ת����
	IMU.ex_yaw = ((int16)(IMU.ex_yaw*10) % 3600)/10.0f;
	error = (IMU.Yaw - IMU.ex_yaw);
	if(fabs_(360 - fabs_(IMU.Yaw - IMU.ex_yaw)) >= fabs_(IMU.Yaw - IMU.ex_yaw))
		error = -(IMU.Yaw - IMU.ex_yaw);
	else{
		error = ((error/fabs_(error)) * (360 - fabs_(IMU.Yaw - IMU.ex_yaw)));
	}

	//d_out=error - error_p_out;
	//�⻷λ��ʽ   D�ӵ�ͨ  ϵ����������0.7
	float output = kpid_out[0] * error + kpid_out[2] *(error-error_p_out);
	error_p_out = error;
	d_out_p=(1* (d_out)+0*(d_out_p));
    pid_value=output;
	//��������
//	//�ڻ�����ʽ
//	dpid = kpid_in[0] * (error - error_p_in1) + 
//				 kpid_in[1] * error + 
//				 kpid_in[2] * (error - 2*error_p_in1 + error_p_in2);
//	error_p_in2 = error_p_in1;
//	error_p_in1 = error;
	//���ٻ���

//	if(fabs(error) <= 10.f)
//	{
//		pid_value += dpid;
//	}
//	else if(fabs(error) <= 15.f)
//	{
//		pid_value += 0.6*dpid;
//	}
//	else
//	{
//		pid_value += 0.1*dpid;
//	}
	//�����޷�
//	float k = 6;
//	if((error >=0 && pid_value >= k*error) || (error < 0 && pid_value <= k*error)) 
//	{		
//		pid_value = k*error;
//	}
   	return limit(pid_value,20);
}
