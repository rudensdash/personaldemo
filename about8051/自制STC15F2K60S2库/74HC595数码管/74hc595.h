#ifndef __DIGITAL_LED
#define __DIGITAL_LED
#include "STC15F2K60S2.h"
#include <intrins.h>
#define TIMER_DELAY
// ���ݵ�Ƭ�����Ӷ�������
sbit SH = P4 ^ 3;
sbit DS = P4 ^ 0;
sbit ST = P4 ^ 2;
sbit OE = P4 ^ 1;

extern unsigned char code LED[]; // �������ܵ���ֵ 1 �� F
extern unsigned char code pos[]; // ��������λѡֵ, �� 0 ��Ϊռλ����(������).


enum show_position { 
  POS_FIRST = 0x01,   // �������ҵ�һ�������
  POS_SECOND = 0x02, 
  POS_THIRD = 0x04, 
  POS_LAST = 0x08
};

/**
  * @brief  ������ʾλ�ò���ʾ����.
  * 
  * @param pos      ��ʾλ��
  * @param context  ��ʾ����
  */
void pos_show(unsigned char pos , unsigned char context);

/**
 * @brief ������ʾ������һ���Ѿ���������ݵ����������ʾ��
 * 
 * @param buf ��Ҫ��ʾ�����ݣ������ң���ʾλ�ã����������С� 
 */
void show(unsigned char buf[4]);

/**
  * @brief  ��ʹ�ö�ʱ����Ҫ���øú������г�ʼ��.
  */
void TimerInit();
#endif
