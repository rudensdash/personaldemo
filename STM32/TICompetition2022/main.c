
//#define DEBUG
#define PRINT_DEBUG // �����������д��ڵ������ ע�͵���ȡ��

#if !defined(DEBUG)
/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   LCD��ʾӢ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_usart.h" 
#include <stdio.h>
#include "math.h"
#include "TIM_Capture.h"
#include "wave_cal.h"
#include "adc_config.h"
#include "bsp_led.h"
#include "my_fft.h"
#include "viewstack.h"
#include "control.h"


extern __IO uint16_t ADC_ConvertedValue;

__inline void fft_get_resolution(SampleParameter_t* Sampler);
__inline void fft_get_harmonic(WaveInfo_t* Wave);
int check_vpp(WaveInfo_t* FM);
static void Delay ( __IO uint32_t nCount );
void Printf_Charater(void)   ;
long Get_MaxCenterFreq(int n);
void Key_Scan(void);

float Carrier_Wave_Freq;

uint16_t y_input1[NPT];  // ADC �ɼ�����
uint16_t y_input2[NPT];
uint16_t y_input3[NPT]; 

#define SAMPLE_NUM 5
float vpp_sum_AM[SAMPLE_NUM];
float vpp_sum_FM[SAMPLE_NUM];
float vpp_sum_BASE[SAMPLE_NUM];
int modulate_flag = 3; // Ĭ��Ϊδ�����ź�
int sample_count = 0;
uint8_t currentGear = 0;
//float cur_ampMultiple_AM[4] = {3.00, 5.10, 9.00, 15.00};
float AM_ampMultiple = 4; // AM�Ŵ�1.5��
float cur_ampMultiple_FM[4]	= {2.6660, 8.40000, 25.80000, 25.80000};
#define VPP_PRESET_NUM 23

// ���ֵ���
float vpp_preset[VPP_PRESET_NUM] = {180,252,280,340,750, 800, 850, 900 ,950, 1000, 1050, 1100, 1180, 1300, 1390, 1500, 1600, 1670, 1780, 1909, 1990, 2230, 2385}; 

float vpp_preset_p[VPP_PRESET_NUM] = {200,288,300,380,800, 850, 900, 950 ,1000, 1050, 1100, 1150, 1250, 1350, 1450, 1550, 1650, 1750, 1850, 2000, 2100, 2350, 2450};
	
//3k 3 - 18k
const float freq_depth_preset_vpp[] = {33, 42, 52, 62, 77, 82, 92, 102, 112, 118, 122, 144, 155, 166, 176, 
										203}; // 3k
const float freq_depth_preset_p[] = {3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000, 12000, 13000, 14000, 15000, 16000, 17000,
									18000}; // 3k
// 4k 4 - 24k
const float freq_depth_preset_vpp_4[] = {39, 51, 61, 70, 75, 90, 100, 108, 117, 122, 142, 150, 160, 171, 201,
										212, 226, 240, 251, 264, 276};
const float freq_depth_preset_p_4[] = {4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000, 12000, 13000, 14000, 15000, 16000, 17000, 18000,
										19000, 20000, 21000, 22000, 23000, 24000};
// 5k 5 - 30k
const float freq_depth_preset_vpp_5[] = {50, 60, 65.5, 78.5, 82, 96, 99, 114, 120, 137, 147, 152, 166, 174, 207,
										 218, 230, 243, 253, 265, 276, 288, 292, 296, 301, 314};
const float freq_depth_preset_p_5[] = {5000, 6000, 7000, 8000, 9000, 10000, 11000, 12000, 13000, 14000, 15000,  16000, 17000, 18000, 19000,
										20000, 21000, 22000, 23000, 24000, 25000, 26000, 27000, 28000, 29000, 30000};
// 6k 6 - 36k
const float freq_depth_preset_vpp_6[] = {52, 60, 70, 75, 81, 91, 102, 112, 119, 128, 136, 150, 153, 161, 207,  
										 219, 230, 241, 252, 261, 271, 283, 288, 291, 295, 302, 314, 327, 339, 352, 364};
const float freq_depth_preset_p_6[] = {6000, 7000, 8000, 9000, 10000, 11000, 12000, 13000, 14000, 15000, 16000, 17000, 18000, 19000, 20000,
										21000, 22000, 23000, 24000, 25000, 26000, 27000, 28000, 29000, 30000, 31000, 32000, 33000, 34000,
										35000, 36000};
// 7k 7 - 42k
const float freq_depth_preset_vpp_7[] = {62, 70, 77, 83, 92, 102, 111, 122, 131, 141, 150, 161, 168, 174, 207,
										217, 227, 237, 247, 257, 267, 277, 287, 290, 292, 296, 301, 313, 325,
										338, 349, 362, 372, 384, 394, 402};
const float freq_depth_preset_p_7[] = {7000, 8000, 9000, 10000, 11000, 12000, 13000, 14000, 15000, 16000, 17000, 18000, 19000, 20000, 21000,
										22000, 23000, 24000, 25000, 26000, 27000, 28000, 29000, 30000, 31000, 32000, 33000, 34000, 35000, 
										36000, 37000, 38000, 39000, 40000, 41000, 42000};
// 8k 8 - 48k
const float freq_depth_preset_vpp_8[] = {66, 74, 80, 85, 96, 105, 115, 120, 133, 141, 151, 160, 167, 175, 206, 
										216, 226, 238, 248, 256, 265, 274, 283, 288, 291, 295, 297, 305, 315, 
										328, 339, 352, 362, 374, 384, 392, 400, 405, 410, 415, 418};
const float freq_depth_preset_p_8[] = {8000, 9000, 10000, 11000, 12000, 13000, 14000, 15000, 16000, 17000, 18000, 19000, 20000, 21000, 22000,
										23000, 24000, 25000, 26000, 27000, 28000, 29000, 30000, 31000, 32000, 33000, 34000, 35000, 36000,
										37000, 38000, 39000, 40000, 41000, 42000, 43000, 44000, 45000, 46000, 47000, 48000};
// 9k 9 - 54k
const float freq_depth_preset_vpp_9[] = {71, 77.5, 82.5, 88.5, 99, 109, 116, 121, 135, 146, 154, 161, 169, 175, 206,
										216, 225, 235, 244, 254, 263, 272, 279, 288, 290, 292, 295, 299, 308, 
										319, 329, 340, 350, 361, 372, 382, 392, 399, 403, 407, 412, 415, 419, 
										423, 475, 484};
const float freq_depth_preset_p_9[] = {9000, 10000, 11000, 12000, 13000, 14000, 15000, 16000, 17000, 18000, 19000, 20000, 21000, 22000,23000,
										24000, 25000, 26000, 27000, 28000, 29000, 30000, 31000, 32000, 33000, 34000, 35000, 36000, 37000,
										38000, 39000, 40000, 41000, 42000, 43000, 44000, 45000, 46000, 47000, 48000, 49000, 50000, 51000,
										52000, 53000, 54000};
// 10k 10 - 60k									
const float freq_depth_preset_vpp_10[] = {71, 77, 84, 91, 101, 110, 118, 121, 135, 143, 150, 158, 165, 172, 
										204, 215, 222, 232, 240, 249, 260, 271, 278, 287, 290, 293, 295, 297, 298, 306,
										314, 324, 337, 346, 354, 366, 376, 383, 390, 393, 398, 401, 405, 409, 412, 416,
										421, 424, 488, 503};
const float freq_depth_preset_p_10[] = {10000, 11000, 12000, 13000, 14000, 15000, 16000, 17000, 18000, 19000, 20000, 21000, 22000,23000,
										24000, 25000, 26000, 27000, 28000, 29000, 30000, 31000, 32000, 33000, 34000, 35000, 36000, 37000,
										38000, 39000, 40000, 41000, 42000, 43000, 44000, 45000, 46000, 47000, 48000, 49000, 50000, 51000,
										52000, 53000, 54000, 55000, 56000, 57000, 58000, 59000, 60000};

float Delta_f_vpp;	
float times;
	
#if defined(PRINT_DEBUG)
int debug_i;
#endif

uint16_t i_;  // ѭ����
 
#define PSC_ 16483.516483 // ��Ƶϵ��

/*
	* ���²������� ���²ɼ��� �� ���ε���ز���
	* @param sampler �ɼ����ṹ��
	* @param wave ���νṹ��
*/
void Update_parameters(SampleParameter_t* sampler, WaveInfo_t* wave){
	#if defined(PRINT_DEBUG)
	printf("1Td: %.1fus, duty: %.1f\r\n\r", sampler->Td, wave->duty / 100);
	#endif
	sampler->Tconv = (12.5 + sampler->SampleCycles + 0.5 ) / 12;
	sampler->Td = (sampler->VALUE_TIM4 ) * (1 / (72.0 / sampler->DIV_TIM4 + 1)) + sampler->Tconv; // ����������
	#if defined(PRINT_DEBUG)
	printf("2Td: %.1fus, duty: %.1f\r\n\r", sampler->Td, wave->duty / 100);
	#endif
	
	if(strcmp(wave->name,"AM" ) == 0){
		Wave_GetFreq(&(*wave), &(*sampler) ,y_input1, NPT);
		vpp_sum_AM[sample_count] = wave->vpp;
	}else if(strcmp(wave->name,"FM" ) == 0){
		Wave_GetFreq(&(*wave), &(*sampler) ,y_input2, NPT);
		vpp_sum_FM[sample_count] = wave->vpp;
	}else if(strcmp(wave->name,"BASE" ) == 0){
		
		Wave_GetFreq(&(*wave), &(*sampler) ,y_input3, NPT);
		vpp_sum_BASE[sample_count] = wave->vpp;
	}
	
	
	#if defined(PRINT_DEBUG)
	printf("%s, f:%f, currentsamplerate:%f\r\n\r",wave->name,wave->freq, sampler->current_freq_);
	#endif
}


// ���㲨��ʧ��� �������ֱֵ�Ӵ���wave�ṹ���У����ýṹ���ڶ�Ӧ��Ա��ò���
void Wave_Cal_THD( WaveInfo_t* wave ){
	int i;
	// �������г������
	wave->U[0] = Get_MaxCenterFreq((int)(fft_inf.harmonic + 0.5)); // ����
	wave->U[1] = Get_MaxCenterFreq((int)(fft_inf.harmonic * 2 + 0.5));
	wave->U[2] = Get_MaxCenterFreq((int)(fft_inf.harmonic * 3 + 0.5));
	wave->U[3] = Get_MaxCenterFreq((int)(fft_inf.harmonic * 4 + 0.5));
	wave->U[4] = Get_MaxCenterFreq((int)(fft_inf.harmonic * 5 + 0.5));
	wave->U[5] = Get_MaxCenterFreq((int)(fft_inf.harmonic * 6 + 0.5));
	wave->U[6] = Get_MaxCenterFreq((int)(fft_inf.harmonic * 7 + 0.5));   
	wave->U[7] = Get_MaxCenterFreq((int)(fft_inf.harmonic * 8 + 0.5));
	
	#if defined(PRINT_DEBUG)
	for(debug_i = 0; debug_i < 8 ;debug_i++){
		printf("U%d = %ld \r\n\r", debug_i + 1, wave->U[debug_i]);
	}
	#endif
	
	wave->THD = 0;
	for(i = 1; i < 8; i++){
		wave->THD += (float)(wave->U[i] * wave->U[i]); // ����г��ƽ�����ۼ�
	}
	wave->THD = sqrt(wave->THD);
	wave->THD = wave->THD / wave->U[0]; 
	#if defined(PRINT_DEBUG)
	printf("THD: %0.2f\r\n", wave->THD * 100);
	#endif
}

#if 1
// ʶ���� ʶ����ֱ�Ӵ���wave->Class
void Wave_Identify(WaveInfo_t* wave){
	int i,count = 0;
	char noise_flag = 0;
	 //�洢��һ�����ֵ
	for(i = 0; i < 7; i++){
		wave->U_Normalized[i] = (float)wave->U[i] / wave->U[0]; 
		printf(" %d:%.2f ",i+1, wave->U_Normalized[i]);
	}
	printf("\r\n\r");
	
	for(i = 1; i < 7; i++){
		if(wave->U_Normalized[i] > 0.06){
			count++;
			if(wave->U_Normalized[i] > 1000.0){
				noise_flag = 1; 
			}
		}
			
	}
	
	wave->Class = Unknown;
	#if defined(PRINT_DEBUG)
	printf("COUNT:%d", count);
	#endif
	if(count > 0){
		if(noise_flag){
			wave->Class = Noise;
		}else if(count >= 5){ 
			printf("\r\n\r%.2f, %.2f\r\n\r",wave->U_Normalized[4],wave->U_Normalized[6]);
			wave->Class = (wave->U_Normalized[4] >= 0.1 && wave->U_Normalized[6] <= 0.4 
														&& ((wave->Falling_time >= 4) || (wave->Rising_time >= 4))) ? Sawtooth : Square;
		}else if(count == 4){
			wave->Class = (wave->U_Normalized[4] >= 0.1 && wave->U_Normalized[6] <= 0.4 
														&& ((wave->Falling_time >= 4) || (wave->Rising_time >= 4))) ? Sawtooth : Square;
		}else if(count == 3){
			if((wave->U_Normalized[2] >= 0.2) && (wave->U_Normalized[2] <= 0.45) && 
													(wave->U_Normalized[4] > 0.1) && (wave->U_Normalized[6] >= 0.09)){
				wave->Class = Square;
			}
		}else if(count == 2){
			wave->Class = Sine;
		}else if(count == 1){
			if(wave->U_Normalized[2] >= 0.05 && wave->U_Normalized[2] <= 0.15)
				wave->Class = Triangle;
		}
		
	}else{
		#if defined(PRINT_DEBUG)
		printf("Sine.\r\n\r"); // ���Ҳ� ��г������
		#endif
		wave->Class = Sine;
	}
	
}
#endif





__inline void ADC1_Sample(){ // AM �ɼ�
	//Sample_flag = 0;
	//ADCx_Init(); 
	ADCx_Init();
	for(i_ = 0; i_ < NPT; i_++) {
		while(Sample_flag == 0);
		Sample_flag = 0;
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
		y_input1[i_] = ADC_GetConversionValue(ADC1);
	}
	
}

__inline void ADC2_Sample(){ // FM �ɼ�
	ADC2_Init();
	for(i_ = 0; i_ < NPT; i_++) {
		while(Sample_flag == 0);
		Sample_flag = 0;
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
		while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));
		y_input2[i_] = ADC_GetConversionValue(ADC2);
	}
}

__inline void ADC3_Sample(){ // ����Ƶ�ʲɼ�
	ADC3_Init();
	for(i_ = 0; i_ < NPT; i_++) {
		while(Sample_flag == 0);
		Sample_flag = 0;
		ADC_SoftwareStartConvCmd(ADC3, ENABLE);
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC));
		y_input3[i_] = ADC_GetConversionValue(ADC3);
	}
}

uint8_t FM_ensure_flag = 1;

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main ( void )
{
	uint8_t state; // ״̬��״̬��
	WaveInfo_t wave; // ���������Ϣ�ṹ��
	WaveInfo_t wave_AM; // ���������Ϣ�ṹ��
	WaveInfo_t wave_FM; // ���������Ϣ�ṹ��
	WaveInfo_t* cur_wave;
	float temp__;
	float temp_cur_vpp;
	//char shift_flag = 0;
	SampleParameter_t SampleArgs; // ���������ṹ��
	ILI9341_Init();         //LCD ��ʼ��
	USART_Config();			//���ڳ�ʼ��
	stack_init();			//��ʾջ��ʼ��
	ADCx_Init();			//ADC��ʼ��
	ADC2_Init();
	ADC3_Init();
	Control_gpio_init();
	LED_GPIO_Config();		//LED�Ƴ�ʼ��
	wave.dc_offset = 1.5; 	//ֱ��ƫ���� �����źŷ��������������ź�ƫ��������
	wave_AM.dc_offset = 1.5;
	wave_FM.dc_offset = 1.5;
	wave.name = "BASE";
	wave_AM.name = "AM";
	wave_FM.name = "FM";
	ArgWindowsWidth = 180;
	screen.push(Show_WavePicture); // Ĭ������Ļջѹ�벨����ʾ���� ����д��������������ͨ�����push������ջ
	// ���ջ���Ϊ5 ���޸�

 //����0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
 //���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��			
 //���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����  
	ILI9341_GramScan ( 3 );	
	LCD_SetTextColor(BLACK);
	ILI9341_Clear(0,0,320,240);	/* ��������ʾȫ�� */
	
	state = 0;
	LED_flash(_BLUE);
	while ( 1 )
	{
		
		//Key_Scan();
		switch(state){
			case 0: // Ĭ�ϳ�ʼ�� ��500KHZ������ 

				ADC1_TriggertimeSet(&SampleArgs, 7, 72);
				
				ADC1_SetSampleCycles(&SampleArgs , 7);
				//SampleArgs.current_freq_ = 496551.72413793;
				SampleArgs.current_freq_ = 125000.0;
				//SampleArgs.current_freq_ = 159292.035398;
				SampleArgs.current_freq = FREQ496KHZ;
				currentGear = 2;
				Shift_Amp_Status(currentGear);
				state = 1;	
				
				break;
			case 1: // ����ת��
				
				EventTimer_Init(SampleArgs.VALUE_TIM4, SampleArgs.DIV_TIM4 - 1);
				
				ADC1_Sample();
				TIM_Cmd(TIM4, DISABLE);
				
				EventTimer_Init(SampleArgs.VALUE_TIM4, SampleArgs.DIV_TIM4 - 1);
				ADC2_Sample();
				TIM_Cmd(TIM4, DISABLE);
				
				EventTimer_Init(SampleArgs.VALUE_TIM4, SampleArgs.DIV_TIM4 - 1);
				ADC3_Sample();
				TIM_Cmd(TIM4, DISABLE);
				
				
				state = 2;
				
				break;
			case 2: // �������� Ƶ�ʵ�
				
				Update_parameters(&SampleArgs, &wave);
				Update_parameters(&SampleArgs, &wave_AM);
				Update_parameters(&SampleArgs, &wave_FM);
				//LED_flash(_BLUE);
				printf("FM_S: %d\r\n", currentGear);
				if(wave_AM.vpp > 200){ // ��AM���� AM����
					modulate_flag = 0;
					cur_wave = &wave_AM; // 
				}else{
					 //wave_FM.vpp = wave_FM.vpp / cur_ampMultiple_FM[currentGear];
					printf("�л�ǰ�Ŵ���www: %d\r\n", currentGear);
					if(check_vpp(&wave_FM) && FM_ensure_flag){
						state = 1;
						break;
					}else{
						if(FM_ensure_flag == 0){
							FM_ensure_flag = 1;
							modulate_flag = 3; // ���ز� �޵��� 
							cur_wave = &wave;
						}else{
							modulate_flag = 1; // ����AM���� �����ز� ��FM
							cur_wave = &wave_FM;
						}
					}		
				}
				//modulate_flag = 1;
				ILI9341_Clear(0, 0, 128 , 32);
				ILI9341_DispString_EN_CH(0, 0, "��ʼ�ɼ�");
				
				if( sample_count < SAMPLE_NUM){
					sample_count++;
					state = 1;
					break;
				}
				
				sample_count = 0;
				 
				wave.vpp = wave_filter_mid_aver(vpp_sum_BASE, SAMPLE_NUM);
				wave_AM.vpp = wave_filter_mid_aver(vpp_sum_AM, SAMPLE_NUM) / AM_ampMultiple;
				wave_FM.vpp = wave_filter_mid_aver(vpp_sum_FM, SAMPLE_NUM) / cur_ampMultiple_FM[currentGear];
				//wave_FM.vpp = wave_filter_mid_aver(vpp_sum_FM, SAMPLE_NUM) / 3.0;
				//wave_AM.Ma = wave_AM.vpp / 715.000;
				wave_AM.Ma = wave_AM.vpp / 630.000;
				//printf("�ռ�����Ma: %.2f", wave_AM.Ma);
				if((wave_AM.Ma >= 0.700) && (wave_AM.Ma <= 0.84)) wave_AM.Ma += 0.05;
				else if(wave_AM.Ma >= 0.80)wave_AM.Ma += 0.1;
				if(wave_AM.Ma > 1.00)wave_AM.Ma = 1.00;
				//printf("�ռ�����Ma2: %.2f", wave_AM.Ma);
				
				
				temp_cur_vpp = cur_wave->vpp + (cur_wave->freq / 3000.0);
				//temp_cur_vpp = cur_wave->vpp;
				printf("curwave, vpp:%.2f\r\n\r",cur_wave->vpp);
				for(i_ = 0; i_ < VPP_PRESET_NUM - 1; i_++){
					if(temp_cur_vpp > vpp_preset[i_]){
						printf("cmp2, FMvpp: %.2f, dstVpp:%.2f\r\n",Delta_f_vpp ,vpp_preset_p[i_ + 1]);
						if(vpp_preset[i_ + 1] != 0 ){
							cur_wave->vpp = vpp_preset_p[i_] + ((temp_cur_vpp - vpp_preset[i_]) / (vpp_preset[i_ + 1] - vpp_preset[i_]) * 
													(vpp_preset_p[i_ + 1] - vpp_preset_p[i_]) );
						}
						
					}
				}
				
				Carrier_Wave_Freq = wave.freq * PSC_;  // �ز�Ƶ�ʲ��� 0.5MHZ
				
				Carrier_Wave_Freq /= 1000000;
				//LED_flash(_RED);
				/*
					�������� 0.5MHZ
				*/
				temp__ = ((int)(Carrier_Wave_Freq) + 0.5);
				if(Carrier_Wave_Freq < temp__){
					if((Carrier_Wave_Freq - (int)(Carrier_Wave_Freq) > (temp__ - Carrier_Wave_Freq))){
						Carrier_Wave_Freq = temp__; // ���� x.5 �� x ����
					}else{
						Carrier_Wave_Freq = (int)(Carrier_Wave_Freq);
					}
				}else if(Carrier_Wave_Freq > temp__){
					if((((int)(Carrier_Wave_Freq) + 1.0 ) - Carrier_Wave_Freq > (Carrier_Wave_Freq - temp__))){
						Carrier_Wave_Freq = temp__; // ���� x.5 �� x + 1 ����
					}else{
						Carrier_Wave_Freq = (int)(Carrier_Wave_Freq) + 1.0;
					}
				}
				
				if(modulate_flag == 1){ // �����FM���� �������Ƶƫ
					times = Carrier_Wave_Freq / 10;
					if(times > 1.5 && times < 1.8) times += 0.05;
					else if(times >= 1.8 && times <= 2.2) times -= 0.05;
					else if(times > 2.2 && times < 2.5) times -= 0.1;
					else if(times >= 2.5 && times <= 2.85) times -= 0.35;
					else if(times > 2.85 ) times -= 0.6;
					printf("Times, %.2f, FMvpp: %.2f\r\n", times, wave_FM.vpp);
					//times = 1;
					Delta_f_vpp = wave_FM.vpp * times;
						if( wave_FM.freq > 2500 && wave_FM.freq < 3500 ){
								for(i_ = 0; i_ < (sizeof(freq_depth_preset_vpp) / sizeof(float) ) -1; i_++){
									if(Delta_f_vpp > freq_depth_preset_vpp[i_]){
										printf("cmp, FMvpp: %.2f, dltFp:%.2f\r\n",Delta_f_vpp ,freq_depth_preset_p[i_]);
										if(freq_depth_preset_vpp[i_ + 1] != 0){
										wave_FM.Delta_fm = 	freq_depth_preset_p[i_] + (Delta_f_vpp - freq_depth_preset_vpp[i_]) 
																	/ (freq_depth_preset_vpp[i_ + 1] - freq_depth_preset_vpp[i_])
																	* (freq_depth_preset_p[i_ + 1] - freq_depth_preset_p[i_]);	
										
									}
									
								}
							}
						
						}else if( wave_FM.freq > 3500 && wave_FM.freq < 4500){
							for(i_ = 0; i_ < (sizeof(freq_depth_preset_vpp_4) / sizeof(float) ) -1; i_++){
									if(Delta_f_vpp > freq_depth_preset_vpp_4[i_]){
										printf("cmp, FMvpp: %.2f, dltFp4:%.2f\r\n",Delta_f_vpp ,freq_depth_preset_p_4[i_]);
										if(freq_depth_preset_vpp_4[i_ + 1] != 0){
										wave_FM.Delta_fm = 	freq_depth_preset_p_4[i_] + (Delta_f_vpp - freq_depth_preset_vpp_4[i_]) 
																	/ (freq_depth_preset_vpp_4[i_ + 1] - freq_depth_preset_vpp_4[i_])
																	* (freq_depth_preset_p_4[i_ + 1] - freq_depth_preset_p_4[i_]);	
										
									}
									
								}
							}
						}else if(wave_FM.freq > 4500 && wave_FM.freq < 5500){
							for(i_ = 0; i_ < (sizeof(freq_depth_preset_vpp_5) / sizeof(float) ) -1; i_++){
									if(Delta_f_vpp > freq_depth_preset_vpp_5[i_]){
										printf("cmp, FMvpp: %.2f, dltFp5:%.2f\r\n",Delta_f_vpp ,freq_depth_preset_p_5[i_]);
										if(freq_depth_preset_vpp_5[i_ + 1] != 0){
										wave_FM.Delta_fm = 	freq_depth_preset_p_5[i_] + (Delta_f_vpp - freq_depth_preset_vpp_5[i_]) 
																	/ (freq_depth_preset_vpp_5[i_ + 1] - freq_depth_preset_vpp_5[i_])
																	* (freq_depth_preset_p_5[i_ + 1] - freq_depth_preset_p_5[i_]);	
										
									}
									
								}
							}
						}else if(wave_FM.freq > 5500 && wave_FM.freq < 6500){
							for(i_ = 0; i_ < (sizeof(freq_depth_preset_vpp_6) / sizeof(float) ) -1; i_++){
									if(Delta_f_vpp > freq_depth_preset_vpp_6[i_]){
										printf("cmp, FMvpp: %.2f, dltFp6:%.2f\r\n",Delta_f_vpp ,freq_depth_preset_p_6[i_]);
										if(freq_depth_preset_vpp_6[i_ + 1] != 0){
										wave_FM.Delta_fm = 	freq_depth_preset_p_6[i_] + (Delta_f_vpp - freq_depth_preset_vpp_6[i_]) 
																	/ (freq_depth_preset_vpp_6[i_ + 1] - freq_depth_preset_vpp_6[i_])
																	* (freq_depth_preset_p_6[i_ + 1] - freq_depth_preset_p_6[i_]);	
										
									}
									
								}
							}
					}else if(wave_FM.freq > 6500 && wave_FM.freq < 7000){
							for(i_ = 0; i_ < (sizeof(freq_depth_preset_vpp_7) / sizeof(float) ) -1; i_++){
									if(Delta_f_vpp > freq_depth_preset_vpp_7[i_]){
										printf("cmp, FMvpp: %.2f, dltFp7:%.2f\r\n",Delta_f_vpp ,freq_depth_preset_p_7[i_]);
										if(freq_depth_preset_vpp_7[i_ + 1] != 0){
										wave_FM.Delta_fm = 	freq_depth_preset_p_7[i_] + (Delta_f_vpp - freq_depth_preset_vpp_7[i_]) 
																	/ (freq_depth_preset_vpp_7[i_ + 1] - freq_depth_preset_vpp_7[i_])
																	* (freq_depth_preset_p_7[i_ + 1] - freq_depth_preset_p_7[i_]);	
										
									}
									
								}
							}
					}else if(wave_FM.freq > 7500 && wave_FM.freq < 8500){
							for(i_ = 0; i_ < (sizeof(freq_depth_preset_vpp_8) / sizeof(float) ) -1; i_++){
									if(Delta_f_vpp > freq_depth_preset_vpp_8[i_]){
										printf("cmp, FMvpp: %.2f, dltFp8:%.2f\r\n",Delta_f_vpp ,freq_depth_preset_p_8[i_]);
										if(freq_depth_preset_vpp_8[i_ + 1] != 0){
										wave_FM.Delta_fm = 	freq_depth_preset_p_8[i_] + (Delta_f_vpp - freq_depth_preset_vpp_8[i_]) 
																	/ (freq_depth_preset_vpp_8[i_ + 1] - freq_depth_preset_vpp_8[i_])
																	* (freq_depth_preset_p_8[i_ + 1] - freq_depth_preset_p_8[i_]);	
										
									}
									
								}
							}
					}else if(wave_FM.freq > 8500 && wave_FM.freq < 9500){
							for(i_ = 0; i_ < (sizeof(freq_depth_preset_vpp_9) / sizeof(float) ) -1; i_++){
									if(Delta_f_vpp > freq_depth_preset_vpp_9[i_]){
										printf("cmp, FMvpp: %.2f, dltFp9:%.2f\r\n",Delta_f_vpp ,freq_depth_preset_p_9[i_]);
										if(freq_depth_preset_vpp_9[i_ + 1] != 0){
										wave_FM.Delta_fm = 	freq_depth_preset_p_9[i_] + (Delta_f_vpp - freq_depth_preset_vpp_9[i_]) 
																	/ (freq_depth_preset_vpp_9[i_ + 1] - freq_depth_preset_vpp_9[i_])
																	* (freq_depth_preset_p_9[i_ + 1] - freq_depth_preset_p_9[i_]);	
										
									}
									
								}
							}
					}else if(wave_FM.freq > 9500 && wave_FM.freq < 10500){
							for(i_ = 0; i_ < (sizeof(freq_depth_preset_vpp_10) / sizeof(float) ) -1; i_++){
									if(Delta_f_vpp > freq_depth_preset_vpp_10[i_]){
										printf("cmp, FMvpp: %.2f, dltFp9:%.2f\r\n",Delta_f_vpp ,freq_depth_preset_p_10[i_]);
										if(freq_depth_preset_vpp_10[i_ + 1] != 0){
										wave_FM.Delta_fm = 	freq_depth_preset_p_10[i_] + (Delta_f_vpp - freq_depth_preset_vpp_10[i_]) 
																	/ (freq_depth_preset_vpp_10[i_ + 1] - freq_depth_preset_vpp_10[i_])
																	* (freq_depth_preset_p_10[i_ + 1] - freq_depth_preset_p_10[i_]);	
										
									}
									
								}
							}
						}
				
				}
				printf("Times, FMDelt:%.2f\r\n", wave_FM.Delta_fm);
				//wave_FM.Delta_fm = 3000 + ((wave_FM.vpp - 30) / 10.0 * 1000);  //��ʽ�� 
				

				
				if(modulate_flag == 3)
					state = 7;  
				else
					state = 4;
				break;
			case 3: // �������
				#if 0  // ������
				shift_flag = ADC1_SampleShift(&SampleArgs, wave.freq);
				if(shift_flag > 0){ // ��������ʸ��� ���²ɼ�
					state = 1; 
				} else{
					state = 4;
				}
				#endif
				state = 4;	
				break;
			case 4: // FFT����
				//LED_flash(_YELLOW);
				FFT(); 
				fft_get_resolution(&SampleArgs);  // ����ֱ���
				if(modulate_flag == 0){
					fft_get_harmonic(&wave_AM);  // ����г������
					printf("FFT, AM\r\n\r");
				}else if(modulate_flag == 1){
					fft_get_harmonic(&wave_FM);
					printf("FFT, FM\r\n\r");
				}
				
				#if 0
				#if defined(PRINT_DEBUG)
				printf("Resolution: %.2f, harmonic: %.2f\r\n\r", fft_inf.resolution, fft_inf.harmonic);
				for(debug_i = 0; debug_i < NPT; debug_i++)
					printf("Mag%d:%d ", debug_i, lBufMagArray[debug_i]);
				printf("\r\n\r");
				#endif 
				#endif
				state = 5;
				break;
			case 5: // ����ʧ���THD
				//LED_flash(_CYAN);
				if(modulate_flag == 0){
					Wave_Cal_THD(&wave_AM);  // ����г������
				}else if(modulate_flag == 1){
					Wave_Cal_THD(&wave_FM);
				}
				state = 6;
				break;
			case 6: // �������ʶ��
				if(modulate_flag == 0){
					 Wave_Identify(&wave_AM);  // ����г������
				}else if(modulate_flag == 1){
					 Wave_Identify(&wave_FM);
				}
				state = 7;
				break;
			case 7: // ˢ����ʾ
				#if defined(PRINT_DEBUG)
				printf("StackSize:%d\r\n\r", screen.Size);
				#endif
				screen.top = screen.container[screen.Size - 1];
				/*
					ʹ��ջ���ݽṹ��ջ��������ʾ
					�������ĺô��ǵ���������������(���ܵ����)
					���Է�����з�����һ������
				*/ 
				if(modulate_flag == 0){
					screen.top(&wave_AM); 
					Set_OutPut(OUT_AM);
				}else if(modulate_flag == 1){
					screen.top(&wave_FM); 
					Set_OutPut(OUT_FM);
				}else if(modulate_flag == 3){
					screen.top(&wave); 
					Set_OutPut(OUT_BASE);
				}
				
				
				ILI9341_Clear(0, 0, 128 , 32);
				ILI9341_DispString_EN_CH(0, 0, "�ɼ����");
				state = 0; // ˢ������ٴ�����ת��
				break;
			
				

		}
		
	}
	
	
}
//#if 0
//void Key_Scan(void){
//	static uint16_t i;
//	// ��ʱ���밴����� ���Ʋ�����ʾ K1��һ�ο���������ʾ �ٰ��ر���ʾ����
//		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) != RESET){
//				i = 0;
//				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET){
//					if(i < 60000)i++;
//							
//					if(i == 100)
//					{
//						LED_flash(_YELLOW); // �Ƶ���˸������/�ز�������
//					}
//					
//					if(i == 20000)LED_flash(_GREEN);   // �̵���˸���� ����г��Ƶ����ʾ����
//				}
//				
//				if(i < 20000){
//					ILI9341_Clear(320 - ArgWindowsWidth, 0, 0 + ArgWindowsWidth, 240);
//					if(ArgWindowsWidth)
//						ArgWindowsWidth = 0;
//					else 
//						ArgWindowsWidth = 120; // ������ʾ���ڿ�� �߶�Ϊ����Y��
//				}
//				if(i > 18000){
//					for(i = 0; i < screen.Size; i++){ // ����Ƿ����иú���
//						if(screen.container[i] == Show_FFT_Picture)break;
//					}
//					if(i == screen.Size){
//						screen.push(Show_FFT_Picture);
//					}else{
//						printf("�Ѿ����˸ú��� %d\r\n\r", i);
//					}
//					
//				}
//	
//			}
//		// ���ؼ�
//		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) != RESET){
//			i = 0;
//			while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == SET);
//			screen.pop();
//			LED_flash(_RED);
//			ILI9341_Clear(0, 0, 320 - ArgWindowsWidth, 240);
//		}
//}
//#endif 


int check_vpp(WaveInfo_t* FM){
	
		// 1100 3200
	
	if(!((FM->vpp > 700) && (FM->vpp <= 3200))){
		if(currentGear > 0 && FM->vpp > 3200){
			currentGear--;
		}else{
			FM_ensure_flag = 0;
		}
	
		
		Shift_Amp_Status(currentGear);
		ILI9341_Clear(0, 0, 128 , 32);
		ILI9341_DispString_EN_CH(0, 0, "FM����");
		return 1;
	}
	
	return 0;
}



// �������� �����ɶ���ͬʱ���⺯�����ÿ�֧  __inline
__inline void fft_get_resolution(SampleParameter_t* Sampler){
	fft_inf.resolution = Sampler->current_freq_ / NPT ; // �����ʳ��Բ�������
}

// ��ȡг������
__inline void fft_get_harmonic(WaveInfo_t* Wave){
	fft_inf.harmonic = Wave->freq / fft_inf.resolution;
}

// ��ȡ�������Ƶ��
long Get_MaxCenterFreq(int n){
	long u[5];
	long max = 0;
	int i;
	u[0] = lBufMagArray[n];
	u[1] = lBufMagArray[n + 1];
	u[2] = lBufMagArray[n + 2];
	u[3] = lBufMagArray[n + 3];
	u[4] = lBufMagArray[n - 1];
	
	for(i = 0; i < 5; i++){
		if(max <= u[i])
			max = u[i];
	}
	return max;
}

/**
  * @brief  ����ʱ����
  * @param  nCount ����ʱ����ֵ
  * @retval ��
  */	
static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}





#else 

#include "stm32f10x.h"
#include <stdio.h>
#include "math.h"

#include "bsp_led.h"

#include "control.h"


float Carrier_Wave_Freq;

uint16_t y_input1[NPT];  // ADC �ɼ�����
uint16_t y_input2[NPT];
uint16_t y_input3[NPT]; 
int modulate_flag = 3; // Ĭ��Ϊδ�����ź�

int main(){
	Control_gpio_init();
	LED_flash(_BLUE);
	while(1){
		Shift_Amp_Status(0);
	}
}

#endif


