#ifndef __AD__H
#define __AD__H
#include "STC15F2K60S2.h"
#include <intrins.h>

typedef void (*adc_mode_cb_t)(void);
typedef unsigned char adc_channel_t;

// parameters
#define VCC 3.3

// ����AD��������
#define ADControl(x) (ADC_CONTR = (x))
// ������Ҫ�򿪵Ĳ�������
#define ADPinOn(x) (P1ASF = x)
// ����������Ҫ�洢�ķ�ʽ
#define ADRJset(x) ((x) ? (CLK_DIV |= 0x20,storeflag = 1) : (CLK_DIV &= 0xdf, storeflag = 0))

#define ADC_START 0x08
#define ADC_FLAG 0x10
#define ADC_POWER 0x80

#define ADC_INTTERRUPT_SWITCH EADC
#define MAIN_INTERRUPT_SWITCH EA

#define ADC_SPEED_540 0x00 // 540��ʱ������ת��һ������
#define ADC_SPEED_360 0x20 // 360��ʱ������ת��һ������
#define ADC_SPEED_180 0x40 // 180��ʱ������ת��һ������
#define ADC_SPEED_90 0x60  // 90��ʱ������ת��һ������

typedef enum {
  POne0 = 0x01, // P1.0�ڲ���
  POne1 = 0x02, // P1.1�ڲ���
  POne2 = 0x04, // P1.2�ڲ���
  POne3 = 0x08, // P1.3�ڲ���
  POne4 = 0x10, // P1.4�ڲ���
  POne5 = 0x20, // P1.5�ڲ���
  POne6 = 0x40, // P1.6�ڲ���
  POne7 = 0x80, // P1.7�ڲ���
  ALL = 0xff,   // ȫ��
} ADC_Pins;

typedef enum { BIGHEAD = 0, LITTLEHEAD = 1 } adc_storemode;

extern unsigned int result;
extern unsigned char ch;
extern double vin;

/**
  * @brief  ����һ����ʼ��ADģ������ӣ���ʾ��ʼ��ADģ�����С���衣
  */
void AD_init_default();

/**
  * @brief  ����ADת��
  */
void AD_start();

/**
  * @brief  �ڲ�����ʱ����������ƥ��ʱ�򣬲���Ҫ�û����á�
  * 
  * @param i ��ʱʱ��
  */
void AD_Delay(unsigned int i);

/**
  * @brief  ADģ���ж�ע�ắ�����Զ���һ���޷���ֵ���޲����ĺ�������������Ϊ�βδ��롣
  * 
  * @param func ������
  */
void AD_callback_function_register(adc_mode_cb_t func);

/**
 * @brief ��ȡת����ɱ�־
 * 
 * @return bit 1 Ϊ��� 0 Ϊû��ת�����
 */
bit AD_read_transover_flag();

/**
 * @brief ��ģ��ת���ж�
 * 
 */
void AD_open_interrupt();

/**
  * @brief  �������ݻ����� 
  * 
  */
void AD_buffer_clear();

/**
  * @brief  ���ò���ͨ�� 
  * 
  * @param num 0~8��һ���� ����ͨ��0 �� ͨ��8 ��Ĭ��Ϊ0
  */
void AD_set_channel(adc_channel_t num);

/**
  * @brief  ���ò�����ת���ٶȣ���λ��ʱ�����ڣ����õ�ʱ����ʹ���Ѿ�����õ��ٶȺ꣬��ʽ
  *         ��ADC_SPEED_XXX
  * 
  * @param spd �ٶ�
  */
void AD_set_speed(unsigned char spd);

/**
  * @brief  �����ѹ ���ص�ѹֵ
  * 
  * @param result ����ת���ĵ�ѹֵ
  * @return double 
  */
double CalVoltage(unsigned int result);

#endif