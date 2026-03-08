#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"



void Mode_Init (void)					//初始化模式切换模块
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//打开APB2时钟的GPIOA	
	
	GPIO_InitTypeDef GPIO_InitStructure;					//构建结构体，用作GPIO初始化的实参		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//上拉输入，按钮接地，按下接地为低电平，松开上拉为高电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				//初始化pin7口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//频率50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//完成对A7口的初始化
	
}

int Mode_Check (void)				//判断模式按钮是否被按下
{
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==1)	//判断A7口通电情况决定开环闭环, 有电走圆
	{
			return 1;		
	}
	else 
	{
			return 0;
	}
}

