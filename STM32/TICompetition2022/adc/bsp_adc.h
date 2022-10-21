#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"
#include "adc_config.h"
// ADC ���ѡ��
// ������ ADC1/2�����ʹ��ADC3���ж���ص�Ҫ�ĳ�ADC3��
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADCx                          ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1

// ADC GPIO�궨��
// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_GPIO_CLK3                 RCC_APB2Periph_GPIOA
//#define    ADC_PORT                      GPIOC
//#define    ADC_PIN                       GPIO_Pin_1

#define    ADC_PORT                      GPIOA
#define    ADC_PIN                       GPIO_Pin_2
// ADC ͨ���궨��                                                        
//#define    ADC_CHANNEL                   ADC_Channel_11
#define    ADC_CHANNEL                   ADC_Channel_2

// ADC �ж���غ궨��
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
	float Td; // ����������֮����ʱ�� Tconv + 1/DIV_TIM4(��λus) * unit_us
	float Tconv; // ת��ʱ�� Tconv = �������� + 12.5 / ADCƵ��(����ĿΪ12M)
	double current_freq_; // ��ǰ������ ������
	uint8_t SampleCycles; // ���������趨ֵ
	uint16_t VALUE_TIM4; // ��ʱ������ֵ 
	uint16_t DIV_TIM4; // ��ʱ��4��Ƶ��
	enum Freq_t current_freq; // ��ǰ������ ö����
}SampleParameter_t;


void ADCx_Init(void);
void ADC3_Init(void);
void ADC2_Init(void);

void ADCx_GPIO_Config(void);


void ADC1_TriggertimeSet(SampleParameter_t* Sampler,uint16_t value, uint16_t DIV);
void ADC1_SetSampleCycles(SampleParameter_t* Sampler ,uint8_t cycles);
uint8_t ADC1_SampleShift(SampleParameter_t* Sampler, float freq );

extern uint16_t y_input1[NPT];  // ADC �ɼ�����
extern uint16_t y_input2[NPT];
extern uint16_t y_input3[NPT]; 

extern int modulate_flag; // Ϊ 0 AM, Ϊ 1 FM ,Ϊ 3 δ����

extern uint8_t presetValue;

extern uint8_t Adc_convert_over;


#endif /* __ADC_H */

