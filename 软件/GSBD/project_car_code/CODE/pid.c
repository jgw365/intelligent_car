#include "pid.h"
//�г����Ĳ���   100 15
_Pid_param_t pid1 = {.kp=300, .ki=9, .kd=0,.low_pass=1,.out_d=0,.out_i=0,.out_p=0,\
			.p_max=PWM_DUTY_MAX,.i_max=PWM_DUTY_MAX,.d_max=PWM_DUTY_MAX,.pre_error=0,.pre_pre_error=0};//��116,3 zew420,50   jw 550,16  315,48���� 420,50ying
_Pid_param_t pid2 = {.kp=300, .ki=9, .kd=0,.low_pass=1,.out_d=0,.out_i=0,.out_p=0,\
            .p_max=PWM_DUTY_MAX,.i_max=PWM_DUTY_MAX,.d_max=PWM_DUTY_MAX,.pre_error=0,.pre_pre_error=0};
_Pid_param_t pid3 = {.kp=300, .ki=9, .kd=0,.low_pass=1,.out_d=0,.out_i=0,.out_p=0,\
            .p_max=PWM_DUTY_MAX,.i_max=PWM_DUTY_MAX,.d_max=PWM_DUTY_MAX,.pre_error=0,.pre_pre_error=0};
_Pid_param_t pid4 = {.kp=300, .ki=9, .kd=0,.low_pass=1,.out_d=0,.out_i=0,.out_p=0,\
            .p_max=PWM_DUTY_MAX,.i_max=PWM_DUTY_MAX,.d_max=PWM_DUTY_MAX,.pre_error=0,.pre_pre_error=0};
_Pid_param_t pid5 = {.kp=0.9, .ki=0, .kd=16,.low_pass=1,.out_d=0,.out_i=0,.out_p=0,\
            .p_max=PWM_DUTY_MAX,.i_max=PWM_DUTY_MAX,.d_max=PWM_DUTY_MAX,.pre_error=0,.pre_pre_error=0};
//λ�û� ���� 3,100
_Pid_param_t pid6 = {.kp=1.7,.ki=0, .kd=8.0,.low_pass=1,.out_d=0,.out_i=0,.out_p=0,\
            .p_max=PWM_DUTY_MAX,.i_max=PWM_DUTY_MAX,.d_max=PWM_DUTY_MAX,.pre_error=0,.pre_pre_error=0};//1.7 8.0
_Pid_param_t pid7 = {.kp=1.5,.ki=0, .kd=8.0,.low_pass=1,.out_d=0,.out_i=0,.out_p=0,\
            .p_max=PWM_DUTY_MAX,.i_max=PWM_DUTY_MAX,.d_max=PWM_DUTY_MAX,.pre_error=0,.pre_pre_error=0};//1.5 8.0
_Pid_param_t pid8 = {.kp=0.5,.ki=0, .kd=0,.low_pass=1,.out_d=0,.out_i=0,.out_p=0,\
            .p_max=PWM_DUTY_MAX,.i_max=PWM_DUTY_MAX,.d_max=PWM_DUTY_MAX,.pre_error=0,.pre_pre_error=0};
_Pid_param_t pid9 = {.kp=1.4,.ki=0, .kd=8.0,.low_pass=1,.out_d=0,.out_i=0,.out_p=0,\
            .p_max=PWM_DUTY_MAX,.i_max=PWM_DUTY_MAX,.d_max=PWM_DUTY_MAX,.pre_error=0,.pre_pre_error=0};  //����
//����0.08  pid6 0.45 0.1
			//6.7  0.9 4.5
			// ����PID λ��  ����ջ�һ����ki  ûȥ�ܵ�ͨlowpassֵ 
float common_pid(_Pid_param_t *pid, float error) {
    pid->out_d = (error - pid->out_p);
    pid->out_p = error;
    pid->out_i += error;
    return limit(pid->kp * pid->out_p + pid->kd * pid->out_d,velocity_global_lim);// pid->ki * pid->out_i;// + pid->kd * pid->out_d;
}
float new_common_pid(_Pid_param_t *pid, float error) {
    pid->out_d = (error - pid->out_p);
    pid->out_p = error;
    pid->out_i += error;
	jiux=pid->kp * pid->out_p /*+ pid->kd * pid->out_d*/;
    return limit(pid->kp * pid->out_p + pid->kd * pid->out_d,20);// pid->ki * pid->out_i;// + pid->kd * pid->out_d;
}

// ����ʽPID
float increment_pid(_Pid_param_t *pid, float error) 
	{
    pid->out_p = pid->kp * (error - pid->pre_error);
    pid->out_i =(pid->ki) * error;
    pid->out_d =pid->kd*(error-2*pid->pre_error+pid->pre_pre_error);
    pid->pre_pre_error = pid->pre_error;
    pid->pre_error = error;
	return limit(pid->out_p + pid->out_i+pid->out_d ,30000);
}

void pid_adjust(_Pid_param_t *pid, float p,float i,float d)   //���κ���
{  
	pid->kp+=p;
	pid->ki+=i;
	pid->kd+=d;
	
}

//����Ϊ�ٶ�Ϊ10������
//�Ƕ�kΪ1  x kΪ1.7 y kΪ1.8      5cm�ڴﵽ10���ٶ� kΪ
float limit(float a,float b)
{
	if (a>=0)
    {
        if (a>=b)
        {
            a = b;
        }

    }
    else if (a<=0)
    {
        if (a<-b)
        {
            a = -b;
        }
    }
    return a;
}