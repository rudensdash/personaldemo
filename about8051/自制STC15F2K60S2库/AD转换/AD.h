#ifndef __AD__H
#define __AD__H
#include "STC15F2K60S2.h"
#include <intrins.h>

typedef void (*adc_mode_cb_t)(void);
typedef unsigned char adc_channel_t;

// parameters
#define VCC 3.3

// 设置AD启动参数
#define ADControl(x) (ADC_CONTR = (x))
// 设置需要打开的采样引脚
#define ADPinOn(x) (P1ASF = x)
// 设置数据需要存储的方式
#define ADRJset(x) ((x) ? (CLK_DIV |= 0x20,storeflag = 1) : (CLK_DIV &= 0xdf, storeflag = 0))

#define ADC_START 0x08
#define ADC_FLAG 0x10
#define ADC_POWER 0x80

#define ADC_INTTERRUPT_SWITCH EADC
#define MAIN_INTERRUPT_SWITCH EA

#define ADC_SPEED_540 0x00 // 540个时钟周期转换一次数据
#define ADC_SPEED_360 0x20 // 360个时钟周期转换一次数据
#define ADC_SPEED_180 0x40 // 180个时钟周期转换一次数据
#define ADC_SPEED_90 0x60  // 90个时钟周期转换一次数据

typedef enum {
  POne0 = 0x01, // P1.0口采样
  POne1 = 0x02, // P1.1口采样
  POne2 = 0x04, // P1.2口采样
  POne3 = 0x08, // P1.3口采样
  POne4 = 0x10, // P1.4口采样
  POne5 = 0x20, // P1.5口采样
  POne6 = 0x40, // P1.6口采样
  POne7 = 0x80, // P1.7口采样
  ALL = 0xff,   // 全开
} ADC_Pins;

typedef enum { BIGHEAD = 0, LITTLEHEAD = 1 } adc_storemode;

extern unsigned int result;
extern unsigned char ch;
extern double vin;

/**
  * @brief  这是一个初始化AD模块的例子，演示初始化AD模块的最小步骤。
  */
void AD_init_default();

/**
  * @brief  启动AD转换
  */
void AD_start();

/**
  * @brief  内部软延时函数，用于匹配时序，不需要用户调用。
  * 
  * @param i 定时时间
  */
void AD_Delay(unsigned int i);

/**
  * @brief  AD模块中断注册函数，自定义一个无返回值，无参数的函数，将函数作为形参传入。
  * 
  * @param func 处理函数
  */
void AD_callback_function_register(adc_mode_cb_t func);

/**
 * @brief 读取转换完成标志
 * 
 * @return bit 1 为完成 0 为没有转换完成
 */
bit AD_read_transover_flag();

/**
 * @brief 打开模数转换中断
 * 
 */
void AD_open_interrupt();

/**
  * @brief  清零数据缓冲区 
  * 
  */
void AD_buffer_clear();

/**
  * @brief  设置采样通道 
  * 
  * @param num 0~8的一个数 代表通道0 到 通道8 ，默认为0
  */
void AD_set_channel(adc_channel_t num);

/**
  * @brief  设置采样的转换速度，单位是时钟周期，设置的时候请使用已经定义好的速度宏，格式
  *         是ADC_SPEED_XXX
  * 
  * @param spd 速度
  */
void AD_set_speed(unsigned char spd);

/**
  * @brief  计算电压 返回电压值
  * 
  * @param result 返回转换的电压值
  * @return double 
  */
double CalVoltage(unsigned int result);

#endif