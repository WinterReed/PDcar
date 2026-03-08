#include "stm32f10x.h"                  // Device header

void PWM_Init (void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//打开时钟APB1的通用定时器TIM2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//打开时钟APB2的GPIOA
	
	GPIO_InitTypeDef GPIO_InitStructure;						//初始化A0、A1口输出PWM波，它们要连接ENA和ENB
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  |  GPIO_Pin_1;	//PIN0是CH1口，PIN1是CH2口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//初始化TIM2
	TIM_InternalClockConfig(TIM2);						
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;					//此处ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;				//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);						//先初始化结构体里所有内容，全部设置为默认值
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;				//再重置需要用的参数
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;		
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStruct.TIM_Pulse = 0;								//CCR   高电平占空比：CCR/（ARR+1），此处赋初始值0，不通电
	TIM_OC1Init(TIM2,&TIM_OCInitStruct);						//OC1就是TIM2的CH1口，即PIN0
	TIM_OC2Init(TIM2,&TIM_OCInitStruct);						//同理，OC2就是CH2口，即PIN1
	
	TIM_Cmd(TIM2, ENABLE);			//开启TIM2计时器
}


