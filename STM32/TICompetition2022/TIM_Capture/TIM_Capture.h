#ifndef TIM_CAPTURE_H__
#define TIM_CAPTURE_H__

#include "stm32f10x.h"

extern int Sample_flag;



void EventTimer_Init(uint16_t arr, uint16_t psc);
#endif 



