#ifndef _pid_h
#define _pid_h
#include "zf_common_headfile.h"


#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
//限幅 
#define MINMAX(input, low, upper) MIN(MAX(input, low), upper)

typedef struct {
    float kp;    //P
    float ki;    //I
    float kd;    //D
    float i_max; //i p d限幅
    float p_max; 
    float d_max; 

    float low_pass;//暂时未用

    float out_p;
    float out_i;
    float out_d;
    
    float error;
    float pre_error;
    float pre_pre_error;
} _Pid_param_t;
extern _Pid_param_t pid1,pid2,pid3,pid4,pid5,pid6,pid7,pid8,pid9;
float common_pid(_Pid_param_t *pid, float error);
float new_common_pid(_Pid_param_t *pid, float error);
float increment_pid(_Pid_param_t *pid, float error);
void pid_adjust(_Pid_param_t *pid, float p,float i,float d);
float limit(float a,float b);
#endif