#ifndef __DIGITAL_LED
#define __DIGITAL_LED
#include "STC15F2K60S2.h"
#include <intrins.h>
#define TIMER_DELAY
// 根据单片机连接定义引脚
sbit SH = P4 ^ 3;
sbit DS = P4 ^ 0;
sbit ST = P4 ^ 2;
sbit OE = P4 ^ 1;

extern unsigned char code LED[]; // 存放数码管的数值 1 到 F
extern unsigned char code pos[]; // 存放数码管位选值, 第 0 号为占位数据(无意义).


enum show_position { 
  POS_FIRST = 0x01,   // 从左往右第一个数码管
  POS_SECOND = 0x02, 
  POS_THIRD = 0x04, 
  POS_LAST = 0x08
};

/**
  * @brief  设置显示位置并显示数据.
  * 
  * @param pos      显示位置
  * @param context  显示内容
  */
void pos_show(unsigned char pos , unsigned char context);

/**
 * @brief 集合显示，传入一个已经存放了数据的数组进行显示。
 * 
 * @param buf 需要显示的数据，从左到右（显示位置）存入数组中。 
 */
void show(unsigned char buf[4]);

/**
  * @brief  若使用定时器需要调用该函数进行初始化.
  */
void TimerInit();
#endif
