#ifndef __MY_FFT__H
#define __MY_FFT__H

#include "bsp_adc.h"
#include "math.h"

#include "string.h"

extern u32 lBufInArray[NPT];
extern u32 lBufOutArray[NPT];
extern u32 lBufMagArray[NPT];

typedef struct {
	float resolution;  // Ƶ�ʷֱ���
	float harmonic;	   // г������
}fft_info_t;

extern fft_info_t fft_inf;

//#define C_FFT_NOT_DSP  // ����ú� ��ʹ�� C ����FFT�㷨 ��������� ��ʹ��DSP

#if !defined(C_FFT_NOT_DSP)

// ʹ�� DSP ����Ҫ������ͷ�ļ�

#include "stm32_dsp.h"
//#include "table_fft.h"


void FFT(void);




#endif


#endif
