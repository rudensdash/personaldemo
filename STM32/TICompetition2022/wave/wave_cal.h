#ifndef __WAVE_CAL__H
#define __WAVE_CAL__H

#include "wave_cal.h"
#include "stm32f10x.h"
#include "bsp_adc.h"
#include "my_fft.h"

#define Filter_Num 5 // 5������Ϊһ���˲�

typedef enum{
	Sine = 0,
	Square,
	Triangle,
	Sawtooth,
	Noise,
	Trapezoid,
	Unknown
}WaveClass_t;


typedef struct{
    float vpp; // ���ֵ
    float duty;	// ռ�ձ�
    float freq; // ����Ƶ��
	float cycles; // ���� ��ʱû��
    float dc_offset;
    float Ma; // ������
	float Mf; // ��Ƶ��
	float Delta_fm; // ���Ƶƫ
	float THD; // ����ʧ���
	WaveClass_t Class; // ��������
	long U[8]; // �洢г������
	float U_Normalized[8]; // г��������һ��ֵ
	uint16_t Rising_time;
	uint16_t Falling_time;
	char flat_top;  //�����Ƿ�ƽ��
	char* name;
}WaveInfo_t;

extern float Carrier_Wave_Freq;

/*
    @brief ���㺯�����ֵ����Сֵ�����ֵ �洢�� wave�ṹ����
    @param wave ���νṹ��
    @param SampleBuffer �����洢����
    @param Size �����洢�����С
    @return vpp - ���ֵ
*/

float Wave_GetFreq(WaveInfo_t* wave , SampleParameter_t* sampler, uint16_t* SampleBuffer, uint16_t Size);

float wave_filter_mid_aver(float* vpp_arr, int len);

void wave_doFilter(uint16_t* SampleBuffer);




#endif
