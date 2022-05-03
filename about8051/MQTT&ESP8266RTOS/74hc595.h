#ifndef __DIGITAL_LED
#define __DIGITAL_LED
#include "STC15F2K60S2.h"
#include <intrins.h>
#define TIMER_DELAY
sbit SH = P4 ^ 3;
sbit DS = P4 ^ 0;
sbit ST = P4 ^ 2;
sbit OE = P4 ^ 1;

extern unsigned char code LED[];
//unsigned char code Seq[] = {0xc0,0xa4,0xc0,0xc0,0xf8};
extern unsigned char code pos[];

extern bit TimerOverflow;

extern unsigned total;

void push_data(unsigned char code_tmp);
void set_view(unsigned char code_pos);
void pos_show(unsigned char pos , unsigned char context);
void TimerInit();
#endif
