#ifndef _encoder_h
#define _encoder_h
#include "zf_common_headfile.h"

//四个方向编码器对应的引脚
#define ENCODER_1                       (QTIMER1_ENCODER1)
#define ENCODER_1_LSB                   (QTIMER1_ENCODER1_CH1_C0)
#define ENCODER_1_DIR                   (QTIMER1_ENCODER1_CH2_C1)

#define ENCODER_2                       (QTIMER1_ENCODER2)
#define ENCODER_2_LSB                   (QTIMER1_ENCODER2_CH1_C2)
#define ENCODER_2_DIR                   (QTIMER1_ENCODER2_CH2_C24)

#define ENCODER_3                       (QTIMER2_ENCODER1)
#define ENCODER_3_LSB                   (QTIMER2_ENCODER1_CH1_C3)
#define ENCODER_3_DIR                   (QTIMER2_ENCODER1_CH2_C25)

#define ENCODER_4                       (QTIMER3_ENCODER2)
#define ENCODER_4_LSB                   (QTIMER3_ENCODER2_CH1_B18)
#define ENCODER_4_DIR                   (QTIMER3_ENCODER2_CH2_B19)

//低通滤波系数
#define k_enc (0.8)  //原为0.8，测影响时改动为1
//编码器类
typedef struct{
	float vel;     //速度
	float dis;     //里程
}_Encoder;
extern _Encoder encoder1, encoder2, encoder3, encoder4;
void encoder_init(void);
void get_wheel_vel(void);
extern uint32 time_rem_1;
extern uint32 time_rem_last;
extern uint32 time_pass;
extern bool ready_enc;
#endif