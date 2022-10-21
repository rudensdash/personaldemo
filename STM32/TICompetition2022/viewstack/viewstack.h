#ifndef __VIEWSTACK_
#define __VIEWSTACK_

#include "stm32f10x.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_adc.h"
#include "wave_cal.h"
#include  <stdio.h>
#include "string.h"

typedef void (*ScreenPicture_t)(WaveInfo_t* wave);
typedef void (*StackDELHanlder_t)(void);
typedef void (*StackINITHanlder_t)(void);
typedef void (*StackADDHandler_t)(ScreenPicture_t);

#define MAX_STACK_DEPTH 5

extern uint16_t ArgWindowsWidth;

typedef struct{

ScreenPicture_t top;
ScreenPicture_t container[MAX_STACK_DEPTH]; //���5 
StackINITHanlder_t init;
StackADDHandler_t push;
StackDELHanlder_t pop;
uint16_t Size;

}_showStack_t; // ��ʾ������ջ

typedef enum{
	GridSize1us = 0, // һ�����Ӵ���1us
	GridSize5us,
	GridSize10us,
	GridSize100us,
	GridSize1ms,
	GridSize5ms
}GirdSize_t; // դ���С����

extern _showStack_t screen;

void stack_init(void);
void screen_stack_push(ScreenPicture_t UI); // ��������ʾ���溯��ѹ��ջ
void screen_stack_pop(void); // ��ջ
void My_DrawLine_Vertical(uint16_t x, uint16_t start_y, uint16_t end_y);
void My_DrawLine_Horizental(uint16_t y, uint16_t start_x, uint16_t end_x);
void Show_WavePicture(WaveInfo_t* wave);
void Show_Parameter(WaveInfo_t* wave);

void Show_FFT_Picture(WaveInfo_t* wave);


#endif
