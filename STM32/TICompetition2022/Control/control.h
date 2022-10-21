#ifndef __CONTROL__H
#define __CONTROL__H

#include "stm32f10x.h"
#include "bsp_adc.h"

#define ControlPort GPIOB
#define ControlPin1 GPIO_Pin_12
#define ControlPin2 GPIO_Pin_13
#define ControlPin3 GPIO_Pin_14
#define ControlPin4 GPIO_Pin_15

#define AM_Amp_Control_Low ControlPin1
#define AM_Amp_Control_High ControlPin2

#define FM_Amp_Control_Low ControlPin3
#define FM_Amp_Control_High ControlPin4

#define OUTPUTPort GPIOC
#define OUTPUT_Control_Low GPIO_Pin_8
#define OUTPUT_Control_High GPIO_Pin_9

#define ControlPortClockCmd RCC_APB2PeriphClockCmd
#define ControlPortClock RCC_APB2Periph_GPIOB
#define OUTPUTPortClock RCC_APB2Periph_GPIOC

// 变量

#define OUT_AM 0
#define OUT_FM 1
#define OUT_BASE 2



// 初始化

void Control_gpio_init(void);

int Shift_Amp_Status(uint8_t Status);

int Set_OutPut(int Source);

// 切换档位

#endif


// --------  end line  -----------------

