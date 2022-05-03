#ifndef __TIMER_H
#define __TIMER_H

#include "STC15F2K60S2.H"

typedef unsigned char BYTE;
typedef unsigned int WORD;

#define TRUE 1
#define FALSE 0

// --- TCON --- 位寻址宏定义
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
    TIMER0 = 0, // 定时器0
    TIMER1 = 1  // 定时器1
} timer_num_t;

typedef enum {
    TWORKMODE_16BIT_AUTOREFILL  = 0x00, // 16位自动重装载模式
    TWORKMODE_16BIT_NOREFILL    = 0X01, // 16位手动重装载
    TWORKMODE_8BIT_AUTOREFILL   = 0X02, // 8位自动重装载
    TWORKMODE_STANDBY           = 0X03  // 当工作在定时器1时不计数, 工作在定时器0时为16位不可屏蔽中断自动重装定时器. 
}timer_mode_t;

typedef struct {
    BYTE LBuffer;               // 定时器初值低8位
    BYTE HBuffer;               // 定时器初值高8位
    timer_mode_t work_mode;     // 定时器工作模式
    BYTE Gate;                  // 定时器启动方式 0 为软启动 1 为硬启动
    BYTE CountMode;             // 如果为 1 使定时器工作在计数模式
}timer_config_t;

/**
  * @brief   定时器0初始化函数, 这只是一个Demo, 可以参考这个函数的初始化步骤。
  */
void Timer0Init();

/**
  * @brief   根据传入的定时器配置来初始化定时器。
  * 
  * @param timer     定时器编号
  * @param config    定时器配置
  */
void TimerInit(timer_num_t timer, timer_config_t config);

/**
 * @brief 定时器模式串口工作回调函数注册, 如果让顶实际工作在计数模式, 则自行写中断函数并注释掉对应定时器原有中断函数.
 * 
 * @param timer 定时器编号 定时器0或者定时器1
 * @param Func  用于注册成中断处理函数的函数
 * @param opnum 中断时间的计数, 该值取决于你设置的定时器初值, 公式是 你想触发程序的时间 / 你设定的定时器溢出时间
 *              比如想 1 秒执行一次中断函数, 而定时器溢出时间为 1ms 那么 opnum = 1000ms / 1ms = 1000
 */
void Timer_interrupt_handler_register(timer_num_t timer, timer_interrupt_cb_t Func, unsigned int opnum);

/**
  * @brief  重新装填指定定时器初值。  
  * 
  * @param timer 
  */
void Refill(timer_num_t timer);

#endif