#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"
#include "Sensor.h"
                
// 基础参数
static int base_speed = 35;     // 基础速度
static int max_speed = 65;      // 最大速度
static int min_speed = -65;      // 最小速度
static float dt = 0.01;

// PID参数
static float Kp = 4.3;         // 比例系数
static float Kd = 2.5;          // 微分系数

static float last_error = 0.0;
static int sign = 2;

// PID控制
void PID_Trace_Control(void)
{
    float error = Get_Error();
	
	 if (error == 10)	//保护机制检测：边缘碰线
		sign = 1 ;
	else if (error == -10)
		sign = -1;
	else if (error == 0)
		sign=0;

	if (sign==1 )// 保护机制
	{
		Motor_Left(30);
		Motor_Right(-30);
	}
	else if (sign == -1)
	{
		Motor_Left(-30);
		Motor_Right(30);
	}
	else if (error == 99)		//脱轨
	{
		if (last_error < 0)
			error = -4;
		else if (last_error > 0)
			error = 4;
		else error = 0;
	}
	else 					//正常循迹
	{
		float feedforward = 0.0;
		if (error<-5)
			feedforward = -10;
		else if (error>5)
			feedforward = 10;

		float derivative = (error - last_error) / dt;
		
		
		// 计算PID输出
		float output =  Kp * error + Kd * derivative;

		// 更新上次误差
		last_error = error;

		// 计算左右轮速度
		int left_speed = base_speed + feedforward + output;
		int right_speed = base_speed - feedforward - output;

		// 限速处理
		if(left_speed > max_speed) left_speed = max_speed;
		if(left_speed < min_speed) left_speed = min_speed;
		if(right_speed > max_speed) right_speed = max_speed;
		if(right_speed < min_speed) right_speed = min_speed;

		// 控制速度
		Motor_Left(left_speed);
		Motor_Right(right_speed);
	}
}
