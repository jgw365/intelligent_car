#ifndef _imu_h
#define _imu_h
#include "zf_common_headfile.h"
void cal_rollAngle();
//IMU��ʼ��
void imu_init();
void imu_init_20602();
void cal_rollAngle_20602();
float fabs_(float a);
typedef struct 
{
	//���ٶȼƼ�����ٶ�
	float  acc_x;
	float  acc_y;
	float  acc_z;
	//���ٶ�
	float gyro_x;
	float gyro_y;
	float gyro_z;
	//�Ƕ�
	float Pitch;
	float Yaw;
	float Roll;
	float ex_yaw;   //��������
	float enc_update;
	//λ��
	float now_x;
	float now_y;
	//�Լ�
	float gyro_bias;  //��������ƫ
	float acc_x_bias;  //���ٶȼ�x����ƫ
	float acc_y_bias;  //���ٶȼ�y����ƫ
	//�ǶȻ�pid
	float kpid_in[3];
	float kpid_out[3];
	float kpid_out1[3];
	//��Ҫ�ܵľ���
	float ex_x;
	float ex_y;

}IMU_;
extern IMU_ IMU;
float turn_Yaw();
extern float show_debug;
extern float show_debug1;

#endif

////ŷ����
//typedef struct{
//	float euler_ang_le[3];
//}Vector3f;

////��Ԫ��
//typedef struct{
//	float q0;
//	float q1;
//	float q2;
//	float q3;
//}quat_;