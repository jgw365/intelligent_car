#ifndef _imu_h
#define _imu_h
#include "zf_common_headfile.h"
void cal_rollAngle();
//IMU初始化
void imu_init();
void imu_init_20602();
void cal_rollAngle_20602();
float fabs_(float a);
typedef struct 
{
	//加速度计计算加速度
	float  acc_x;
	float  acc_y;
	float  acc_z;
	//角速度
	float gyro_x;
	float gyro_y;
	float gyro_z;
	//角度
	float Pitch;
	float Yaw;
	float Roll;
	float ex_yaw;   //期望方向
	float enc_update;
	//位置
	float now_x;
	float now_y;
	//自检
	float gyro_bias;  //陀螺仪零偏
	float acc_x_bias;  //加速度计x轴零偏
	float acc_y_bias;  //加速度计y轴零偏
	//角度环pid
	float kpid_in[3];
	float kpid_out[3];
	float kpid_out1[3];
	//需要跑的距离
	float ex_x;
	float ex_y;

}IMU_;
extern IMU_ IMU;
float turn_Yaw();
extern float show_debug;
extern float show_debug1;

#endif

////欧拉角
//typedef struct{
//	float euler_ang_le[3];
//}Vector3f;

////四元数
//typedef struct{
//	float q0;
//	float q1;
//	float q2;
//	float q3;
//}quat_;