#ifndef __MY_FFT__H
#define __MY_FFT__H

#include "bsp_adc.h"
#include "math.h"

#include "string.h"

extern u32 lBufInArray[NPT];
extern u32 lBufOutArray[NPT];
extern u32 lBufMagArray[NPT];

typedef struct {
	float resolution;  // 频率分辨率
	float harmonic;	   // 谐波点数
}fft_info_t;

extern fft_info_t fft_inf;

//#define C_FFT_NOT_DSP  // 定义该宏 则使用 C 语言FFT算法 如果不定义 则使用DSP

#if !defined(C_FFT_NOT_DSP)

// 使用 DSP 库需要包含的头文件

#include "stm32_dsp.h"
//#include "table_fft.h"


void FFT(void);




#endif


#endif
