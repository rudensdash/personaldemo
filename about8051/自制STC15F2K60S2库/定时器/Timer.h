#ifndef __TIMER_H
#define __TIMER_H

#include "STC15F2K60S2.H"

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define TRUE 1
#define FALSE 0

// --- TCON --- λѰַ�궨��
#define TIMER_OVERFLOW_TIMER1 TF1
#define TIMER_OVERFLOW_TIMER0 TF0

#define TIMER_LAUNCH_SWITCH_TIMER1 TR1
#define TIMER_LAUNCH_SWITCH_TIMER0 TR0

#define TIMER_WORKMODE_STATUS TMOD

#define TIMER_INTERRUPT_SWITCH_TIMER0 ET0
#define TIMER_INTERRUPT_SWITCH_TIMER1 ET1
#define MAIN_INTERRUPT_SWITCH EA

#define TIMER_BUFFER_LOW_TIMER0 TL0
#define TIMER_BUFFER_HIGH_TIMER0 TH0
#define TIMER_BUFFER_LOW_TIMER1 TL1
#define TIMER_BUFFER_HIGH_TIMER1 TH1

typedef void (*timer_interrupt_cb_t)(void);

typedef enum {
    TIMER0 = 0, // ��ʱ��0
    TIMER1 = 1  // ��ʱ��1
} timer_num_t;

typedef enum {
    TWORKMODE_16BIT_AUTOREFILL  = 0x00, // 16λ�Զ���װ��ģʽ
    TWORKMODE_16BIT_NOREFILL    = 0X01, // 16λ�ֶ���װ��
    TWORKMODE_8BIT_AUTOREFILL   = 0X02, // 8λ�Զ���װ��
    TWORKMODE_STANDBY           = 0X03  // �������ڶ�ʱ��1ʱ������, �����ڶ�ʱ��0ʱΪ16λ���������ж��Զ���װ��ʱ��. 
}timer_mode_t;

typedef struct {
    BYTE LBuffer;               // ��ʱ����ֵ��8λ
    BYTE HBuffer;               // ��ʱ����ֵ��8λ
    timer_mode_t work_mode;     // ��ʱ������ģʽ
    BYTE Gate;                  // ��ʱ��������ʽ 0 Ϊ������ 1 ΪӲ����
    BYTE CountMode;             // ���Ϊ 1 ʹ��ʱ�������ڼ���ģʽ
}timer_config_t;

/**
  * @brief   ��ʱ��0��ʼ������, ��ֻ��һ��Demo, ���Բο���������ĳ�ʼ�����衣
  */
void Timer0Init();

/**
  * @brief   ���ݴ���Ķ�ʱ����������ʼ����ʱ����
  * 
  * @param timer     ��ʱ�����
  * @param config    ��ʱ������
  */
void TimerInit(timer_num_t timer, timer_config_t config);

/**
 * @brief ��ʱ��ģʽ���ڹ����ص�����ע��, ����ö�ʵ�ʹ����ڼ���ģʽ, ������д�жϺ�����ע�͵���Ӧ��ʱ��ԭ���жϺ���.
 * 
 * @param timer ��ʱ����� ��ʱ��0���߶�ʱ��1
 * @param Func  ����ע����жϴ������ĺ���
 * @param opnum �ж�ʱ��ļ���, ��ֵȡ���������õĶ�ʱ����ֵ, ��ʽ�� ���봥�������ʱ�� / ���趨�Ķ�ʱ�����ʱ��
 *              ������ 1 ��ִ��һ���жϺ���, ����ʱ�����ʱ��Ϊ 1ms ��ô opnum = 1000ms / 1ms = 1000
 */
void Timer_interrupt_handler_register(timer_num_t timer, timer_interrupt_cb_t Func, unsigned int opnum);

/**
  * @brief  ����װ��ָ����ʱ����ֵ��  
  * 
  * @param timer 
  */
void Refill(timer_num_t timer);

#endif