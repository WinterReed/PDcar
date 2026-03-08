#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void PWM_Init (void);			//函数声明


void Motor_Init(void)			//初始化，包括：方向初始化（Motor初始化时实现），速度初始化（PWM初始化时实现）
{
	PWM_Init();					//将PWM初始化集成到Motor初始化
	
	//初始化时钟APB2的GPIOA(已在PWM初始化时完成)
	
	GPIO_InitTypeDef GPIO_InitStructure;						//初始化A9、A10、A11、A12口，它们要连接IN1、IN2、IN3、IN4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9  |  GPIO_Pin_10 | GPIO_Pin_11  |  GPIO_Pin_12 ;
	//PIN9、10为IN1、2 ； PIN11、12为IN3、4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOA , GPIO_Pin_9  |  GPIO_Pin_10 | GPIO_Pin_11  |  GPIO_Pin_12, Bit_RESET);	
	//统一所有方向口都是低电平，确保初始为静止，同时避免电平不稳
	
	//GPIO_WriteBit(GPIOA , GPIO_Pin_9|GPIO_Pin_11,Bit_SET);		//默认两个轮子向前
	//GPIO_WriteBit(GPIOA , GPIO_Pin_10|GPIO_Pin_12,Bit_RESET);
}


void Motor_Left (int speed)		//控制左轮方向与转速
{
	if (speed>0)		//前进
	{
		GPIO_WriteBit(GPIOA , GPIO_Pin_9,Bit_SET);
		GPIO_WriteBit(GPIOA , GPIO_Pin_10,Bit_RESET);
		TIM_SetCompare1(TIM2,speed);	//调CCR		
	}	
	else if (speed<0)	//后退
	{
		GPIO_WriteBit(GPIOA , GPIO_Pin_9,Bit_RESET);
		GPIO_WriteBit(GPIOA , GPIO_Pin_10,Bit_SET);
		TIM_SetCompare1(TIM2,-speed);	//调CCR		
	}
	else 				//停转
		GPIO_WriteBit(GPIOA , GPIO_Pin_9 | GPIO_Pin_10 , Bit_RESET);	
}


void Motor_Right (int speed)		//控制右轮方向与转速
{
	if (speed>0)		//前进
	{
		GPIO_WriteBit(GPIOA , GPIO_Pin_11,Bit_SET);
		GPIO_WriteBit(GPIOA , GPIO_Pin_12,Bit_RESET);
		TIM_SetCompare2(TIM2,speed);	//调CCR		
	}	
	else if (speed<0)	//后退
	{
		GPIO_WriteBit(GPIOA , GPIO_Pin_11,Bit_RESET);
		GPIO_WriteBit(GPIOA , GPIO_Pin_12,Bit_SET);
		TIM_SetCompare2(TIM2,-speed);	//调CCR		
	}
	else 				//停转
		GPIO_WriteBit(GPIOA , GPIO_Pin_11 | GPIO_Pin_12 , Bit_RESET);	
}


