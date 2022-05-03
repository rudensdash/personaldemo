
#ifndef __LCD_H_
#define __LCD_H_

//#define LCD1602_4PINS  // 区分4总线和8总线的LCD1602.

#include <reg52.h>
#ifndef DataPort
#define DataPort P0
#endif
/**************************************
PIN口定义
***************************************/
sbit RS=P2^6;
sbit RW=P2^5;
sbit EN=P2^7;

#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint
#define uint unsigned int
#endif

// LCD 功能部分

// LCD Write CMD: RS = 0
void LCD_Write_CMD(char value);


// LCD Write Data : RS = 1
void LCD_Write_Data(char value);

// LCD init
void LCDInit();


// delay
void delay1ms(uint c);
void delay1us(uint c);

#endif