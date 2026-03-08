#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"
#include "Mode.h"
#include "Track.h"
#include "Sensor.h"
#include "Timer.h"
#include "OLED.h"



/***
引脚连接：A0-1接L298N的ENA（左）、ENB（右），A7接小开关，A9-12接L298N的IN1、2（左轮），IN3、4（右轮）
		B3-6接4个反射式红外传感器的DO口(顺序自左到右)
***/

int main ()
{	
	OLED_Init();
	OLED_ShowString(1,1,"DaiJi");
	Delay_s(3);			//待机两秒

	Motor_Init ();		//初始化Motor和PWM（因为PWM初始化被集成在了Motor初始化中）
	Mode_Init ();		//初始化Mode模块
	Sensor_Init();		//初始化传感器
	
	Timer_Init();
	
	while (1)
	{
		
		
	}
}
