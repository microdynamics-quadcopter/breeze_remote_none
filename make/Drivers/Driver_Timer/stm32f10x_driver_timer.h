#ifndef __STM32F10X_DRIVER_TIMER_H__
#define __STM32F10X_DRIVER_TIMER_H__

#include "stm32f10x.h"

#define TIME4_Preiod  1000

extern u16 flag10Hz,flag50Hz,flag80Hz,flag100Hz;

void TIM4_Init(char clock,int Preiod);//用于监测系统
void TIM3_Init(char clock,int Preiod);//定时器3的初始化
void TimerNVIC_Configuration(void);//定时器中断向量表配置

#endif
