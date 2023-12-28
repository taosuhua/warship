#ifndef __SYSTICK_H
#define __SYSTICK_H
#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(u32 nus);
void Delay_ms(u16 nms);
void Delay_s(unsigned int ms);
void TimingDelay_Decrement(void);





#endif
