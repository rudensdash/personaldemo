#ifndef __RS485__H
#define __RS485__H

#include "STC15F2K60S2.h"
#include "intrins.h"

sbit RS485DIR = P3 ^ 5;

#define MODE RS485DIR

enum state{ // RECV ���� EMIT ����
	RECV = 0, EMIT
};

/*
	RS485 ��Ҫ��ϴ���ʹ�ã���Ҫ�ȳ�ʼ�����ڡ�
*/

void RS485_send_data(char *string); //RS485 ��������
void RS485_init(); // RS485 ��ʼ��
#endif