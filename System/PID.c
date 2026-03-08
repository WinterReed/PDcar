#include "stm32f10x.h"                  // Device header


typedef struct{
	float kp;
	float ki;
	float kd;
	float integral;
	float prev_error;
	float max_integral;
} PID_TypeDef;

int error_get(int *LL,int *L,int *R,int *RR)	//此函数用于判断偏向
{
	int error=0;
	if (*LL&&*RR)	return 0;	//外侧左右都压线，直走
	if (!*L&&!*R) return 0;		//内测左右都没压线，直走
	if (*LL) error-=40;			//极左
	if (*L) error-=20;			//偏左
	if (*R) error+=20;			//偏右
	if (*RR) error+=40;			//极右
	return error;
	
}
	

float PID_get(PID_TypeDef*pid, float sensor, float dt)
{
	
	pid->integral+=(0-sensor)*dt;
	
	if (pid->integral > pid->max_integral)
		pid->integral = pid->max_integral;
	else if (pid->integral < -pid->max_integral)
		pid->integral = -pid->max_integral;
	
	float d =((0-sensor-pid->prev_error)/dt);
	float output= -(pid->kp*sensor)+(pid->ki*pid->integral)+(pid->kd*d);
	
	return output;	
}
