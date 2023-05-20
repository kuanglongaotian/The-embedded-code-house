#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"                  // Device header

//正点原子定时器驱动函数库

void TIM4_Int_Init_Pwm(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
 
#endif
