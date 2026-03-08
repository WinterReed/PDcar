/**
  ******************************************************************************
  * @file    Timer.c
  * @brief   10ms 周期定时器实现（TIM3，STM32F103C8T6）
  * @note    使用 TIM3 与 TIM3_IRQn，LD/MD 等型号均支持；PWM 已占 TIM2，故用 TIM3
  ******************************************************************************
  */

#include "Timer.h"
#include "stm32f10x.h"
#include "misc.h"
#include "Track.h"
#include "Mode.h"
#include "Motor.h"
#include "OLED.h"

/* TIM3 时钟在 APB1，定时器时钟 72MHz */
#define TIM3_PRESCALER    (72 - 1)     /* 72MHz / 72 = 1MHz */
#define TIM3_PERIOD       (10000 - 1)  /* 10000 计数 = 10ms */

/**
  * @brief  初始化 TIM3 为 10ms 周期，并开启更新中断
  */
void Timer_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = TIM3_PERIOD;
    TIM_TimeBaseStructure.TIM_Prescaler = TIM3_PRESCALER;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_InternalClockConfig(TIM3);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  /* TIM3_IRQn 在 LD/MD/HD 等型号均有定义 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief  每 10ms 执行一次：在此函数内填写目标代码
  * @note   ========== 此处可放目标代码 ==========
  */
int OLED , LO=9;
void Timer_10ms_Task(void)
{
	if (Mode_Check())	//开环走圆
		{	OLED = 0;
			Motor_Left (30);
			Motor_Right (36);	
			//警告：我以后要通电测试两个轮子的CCR确保轨迹半径和运行速度。2026.1.19
		}
		else			//闭环寻路
		{
			OLED = 1;
			PID_Trace_Control();
		}//警告：寻路系统尚未完工。2026.1.20
		if (OLED != LO) 
		{if (OLED==0)
			{OLED_Clear ();
			 OLED_ShowString (1,1,"KaiHuan");}
			else 
			{OLED_Clear ();
			OLED_ShowString (1,1,"BiHuan");}}
		LO = OLED;
}

/**
  * @brief  TIM3 中断服务函数：每 10ms 进入一次，调用 Timer_10ms_Task()
  */
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        Timer_10ms_Task();
    }
}
