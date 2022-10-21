#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"
#include "adc_config.h"
// ADC 编号选择
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx                          ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1

// ADC GPIO宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_GPIO_CLK3                 RCC_APB2Periph_GPIOA
//#define    ADC_PORT                      GPIOC
//#define    ADC_PIN                       GPIO_Pin_1

#define    ADC_PORT                      GPIOA
#define    ADC_PIN                       GPIO_Pin_2
// ADC 通道宏定义                                                        
//#define    ADC_CHANNEL                   ADC_Channel_11
#define    ADC_CHANNEL                   ADC_Channel_2

// ADC 中断相关宏定义
#define    ADC_IRQ                       ADC1_2_IRQn
#define    ADC_IRQHandler                ADC1_2_IRQHandler

//#define    ADC_IRQ                       ADC3_IRQn
//#define    ADC_IRQHandler                ADC3_IRQHandler

#define ADCAM_PORT GPIOC
#define ADCFM_PORT GPIOA
#define ADCBASE_PORT GPIOA

#define ADCAM_PORT_PIN GPIO_Pin_4
#define ADCFM_PORT_PIN GPIO_Pin_3
#define ADCBASE_PORT_PIN GPIO_Pin_2

#define NPT 1024

typedef struct {
	float Td; // 两个采样点之间间隔时间 Tconv + 1/DIV_TIM4(单位us) * unit_us
	float Tconv; // 转换时间 Tconv = 采样周期 + 12.5 / ADC频率(该项目为12M)
	double current_freq_; // 当前采样率 数字量
	uint8_t SampleCycles; // 采样周期设定值
	uint16_t VALUE_TIM4; // 定时器重载值 
	uint16_t DIV_TIM4; // 定时器4分频数
	enum Freq_t current_freq; // 当前采样率 枚举量
}SampleParameter_t;


void ADCx_Init(void);
void ADC3_Init(void);
void ADC2_Init(void);

void ADCx_GPIO_Config(void);


void ADC1_TriggertimeSet(SampleParameter_t* Sampler,uint16_t value, uint16_t DIV);
void ADC1_SetSampleCycles(SampleParameter_t* Sampler ,uint8_t cycles);
uint8_t ADC1_SampleShift(SampleParameter_t* Sampler, float freq );

extern uint16_t y_input1[NPT];  // ADC 采集数据
extern uint16_t y_input2[NPT];
extern uint16_t y_input3[NPT]; 

extern int modulate_flag; // 为 0 AM, 为 1 FM ,为 3 未调制

extern uint8_t presetValue;

extern uint8_t Adc_convert_over;


#endif /* __ADC_H */

