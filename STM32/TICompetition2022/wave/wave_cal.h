#ifndef __WAVE_CAL__H
#define __WAVE_CAL__H

#include "wave_cal.h"
#include "stm32f10x.h"
#include "bsp_adc.h"
#include "my_fft.h"

#define Filter_Num 5 // 5个数据为一组滤波

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
    float vpp; // 峰峰值
    float duty;	// 占空比
    float freq; // 波形频率
	float cycles; // 周期 暂时没用
    float dc_offset;
    float Ma; // 调幅度
	float Mf; // 调频度
	float Delta_fm; // 最大频偏
	float THD; // 波形失真度
	WaveClass_t Class; // 波形类型
	long U[8]; // 存储谐波分量
	float U_Normalized[8]; // 谐波分量归一化值
	uint16_t Rising_time;
	uint16_t Falling_time;
	char flat_top;  //顶部是否平滑
	char* name;
}WaveInfo_t;

extern float Carrier_Wave_Freq;

/*
    @brief 计算函数最大值、最小值、峰峰值 存储到 wave结构体中
    @param wave 波形结构体
    @param SampleBuffer 采样存储数组
    @param Size 采样存储数组大小
    @return vpp - 峰峰值
*/

float Wave_GetFreq(WaveInfo_t* wave , SampleParameter_t* sampler, uint16_t* SampleBuffer, uint16_t Size);

float wave_filter_mid_aver(float* vpp_arr, int len);

void wave_doFilter(uint16_t* SampleBuffer);




#endif
