/**
  ******************************************************************************
  * @file    Timer.h
  * @brief   10ms 周期定时器模块头文件（STM32F103C8T6 循迹小车）
  * @note    使用 TIM3 实现每 10ms 触发一次，目标代码在 Timer.c 的 Timer_10ms_Task() 中填写
  ******************************************************************************
  */

#ifndef __TIMER_H
#define __TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  初始化 10ms 定时器（TIM4）
  * @param  None
  * @retval None
  * @note   调用后每 10ms 进入一次中断，并执行 Timer_10ms_Task()；使用 TIM3/TIM3_IRQn，兼容 LD/MD 等型号
  */
void Timer_Init(void);

/**
  * @brief  每 10ms 被调用一次的任务函数（在 Timer.c 中实现，可在此函数内填写目标代码）
  */
void Timer_10ms_Task(void);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H */
