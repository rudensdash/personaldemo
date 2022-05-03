#ifndef __RS485__H
#define __RS485__H

#include "STC15F2K60S2.h"
#include "intrins.h"

sbit RS485DIR = P3 ^ 5;

#define MODE RS485DIR

enum state{ // RECV 接收 EMIT 发送
	RECV = 0, EMIT
};

/*
	RS485 需要配合串口使用，需要先初始化串口。
*/

void RS485_send_data(char *string); //RS485 发送数据
void RS485_init(); // RS485 初始化
#endif