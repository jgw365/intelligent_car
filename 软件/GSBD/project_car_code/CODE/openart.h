#ifndef __OPENART_H__
#define __OPENART_H__
#include "zf_common_headfile.h"
#define max_pot 35   //������
//zean
extern int place_all ; //ȫͼ�㣬����ǰ���� attention 
void openart_xy(uint8_t data);
void openart_error(uint8_t data);
void openart1_error(uint8_t data);
void openart2_receive(uint8_t data);
void openart_type(uint8_t data);
void openart_non(uint8_t data);
void approach_img();   //�ӽ�ͼƬ
void approach_img_back();
void slow_down(float x,float y); // ͼƬ��������
void openart_jiaodu(uint8_t data);
float nolinar_error(float k,float error);

extern float angle_p;
//��������
extern uint32 cnt_art;
extern float cor_angle;
extern uint32 cnt_art_p;
extern uint32 cnt_;
extern int16_t map_read[max_pot][2];
extern uint8 map_num;
extern uint8 map_sum;
extern float img_error[2];
extern float img_1_error[2];
extern float img_error_p[2];
extern uint8 start_correct_flag;
//extern uint8 error_flag;
//extern uint8 error1_flag;
extern uint8 object_type[max_pot];
extern uint8 reply_state;
//extern uint8_t find_flag;   //�Ƿ��ܿ����ı�־��ÿ��ʶ����һ����0
extern float correct_xkpid[3];
extern float correct_ykpid[3];
extern uint8_t two_con;
extern bool find_flag;
extern bool control_flag;
extern bool control_1_flag;
extern float jiux;
extern int cor_img;
extern int error_count;
extern float scan_temp[max_pot][2];
extern float scan_this[max_pot][2];
extern uint8 scan_num;

extern uint8 headx,tailx;
extern float swap_temp;


extern uint8 scan_this_num[max_pot];
void openart_scan(uint8_t data);//ɨ��
void openart_dis_angle(uint8 data);


#endif