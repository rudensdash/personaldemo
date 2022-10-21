#include "wave_cal.h"
#include "bsp_usart.h"
#include "string.h"

#define DELTA_PRESET_NUM 9
float Delta_fm_preset_vpp[DELTA_PRESET_NUM];
float Delta_fm_preset_f[DELTA_PRESET_NUM];



void wave_filter(WaveInfo_t* wave, uint16_t* SampleBuffer, uint16_t Size){
	
//#define Up  ((wave->dc_offset + 0.1) / 3.3 * 4096)
//#define Down ((wave->dc_offset - 0.1) / 3.3 * 4096)
	int i;
	int aver;
	for(i = 0; i < Size - 2; i+=3){
		aver = (SampleBuffer[i] + SampleBuffer[i + 1] + SampleBuffer[i + 2]) / 3;
		SampleBuffer[i] = aver;
		SampleBuffer[i + 1] = aver;
		SampleBuffer[i + 2] = aver;
	}
	
}



float wave_filter_mid_aver(float* vpp_arr, int len){
	int i;
	float max, min,sum = 0;
	max = min = vpp_arr[0];
	for( i = 0; i < len; i++){
		if(vpp_arr[i] > max) max = vpp_arr[i];
		if(vpp_arr[i] < min) min = vpp_arr[i];
		sum += vpp_arr[i];
	}
	return (sum - max - min) / (len - 2);
}

// ��ȡƵ��
float Wave_GetFreq(WaveInfo_t* wave , SampleParameter_t* sampler, uint16_t* SampleBuffer, uint16_t Size)
{
	char flag=0;
	int i,n;
	float s0,s1=0,duty_s0, duty_s1,f_max,f_min;
	float Up_Unit_Time, Down_Unit_Time,temp_Ma; 
	char Rising_record_allowed,Falling_record_allowed;
	int max,min;
	int t0=0,t1=0;
	//float f;
	float t;
//������ֵ��UP���ޣ�DOMN����
#define Up  ((wave->dc_offset + 0.1) / 3.3 * 4096)
#define Down ((wave->dc_offset - 0.1) / 3.3 * 4096)
	max=min=SampleBuffer[0];//���ֵ��ʼ�� 
    n=0;//���ش���
	if(SampleBuffer[0]>Up)flag=1;else if(SampleBuffer[0]<Down)flag=0;else flag=-1;//�жϳ�ʼ��ѹ���
	s1=0;//�ߵ�ƽʱ���ۼ�ֵ
	s0=0;//�͵�ƽʱ���ۼ�ֵ
	t0=0;//�½���ʱ����
	t1=0;//������ʱ����
	Up_Unit_Time = 0;
	Down_Unit_Time = 0;
	wave->Rising_time = 0;
	wave->Falling_time = 0;
	wave->flat_top = 0;  // ƽ������ ���ڼ������β������ǲ�
	Rising_record_allowed = 0;
	Falling_record_allowed = 0;
	//wave_filter(&(*wave), &(*SampleBuffer), Size);
	//printf("Vol: %.1f ", (float)SampleBuffer[0] / 4096.0 * 3.3);
//	if(strcmp(wave->name,"BASE") == 0){
//		PRINT(plotter, "%.1f", (float)SampleBuffer[0] / 4096.0 * 3.3);
//	}
	
	for(i=1;i<Size;i++)
	{
		if(Falling_record_allowed && (SampleBuffer[i] < Up && SampleBuffer[i] > Down)){
			wave->Falling_time++;
			
		}
		
		if(Rising_record_allowed && (SampleBuffer[i] < Up && SampleBuffer[i] > Down)){
			wave->Rising_time++;
		}
		
		//printf("Vol: %.1f ", (float)SampleBuffer[i] / 4096.0 * 3.3);
		//PRINT(plotter, "%.1f", (float)SampleBuffer[i] / 4096.0 * 3.3);
//		if(strcmp(wave->name,"BASE") == 0){
//			PRINT(plotter, "%.1f", (float)SampleBuffer[i] / 4096.0 * 3.3);
//		}
	  if(SampleBuffer[i]>Up)
		{
			if((SampleBuffer[i + 5] == SampleBuffer[i + 7]) && (SampleBuffer[i + 7] == SampleBuffer[i + 10]))
				wave->flat_top = 1;
			Rising_record_allowed = 0;
			if(SampleBuffer[i + 1] < Up) // �½��ؼ�����ʼ
				Falling_record_allowed = 1;
			else
				Falling_record_allowed = 0;
			if(flag!=1) // �ж��Ƿ��������� �� ���� 0 Ϊ������ -1 Ϊ����
			{
				if(flag==0) //���Ϊ-1�����ʾ���ǵ�һ�����أ������ۼ�
				{
				 if(n)s0+=i-t0; //�����������أ��͵�ƽʱ��Ϊ��ǰֵ���½���
				 t1=i;//���������صı���ֵ (ʱ���)
				 if(n > 1){
					Down_Unit_Time = i-t0;
				 }
                 n++;//������+1                                                            
				}
				flag=1;//��־Ϊ�ߵ�ƽ
				//printf("s0=%d,s1=%d,t0=%d,t1=%d,n=%d,i=%d,flag=%d\r\n",s0,s1,t0,t1,n,i,flag);
			}
			if(max<SampleBuffer[i])max=SampleBuffer[i];//���������ֵ 
		}
		else  if(SampleBuffer[i]<Down)
		{
			Falling_record_allowed = 0;
			if(SampleBuffer[i + 1] > Down)
				Rising_record_allowed = 1;
			else
				Rising_record_allowed = 0;
			
			if(flag!=0) // �ж��Ƿ����½��� �� ���� �½���Ϊ 1 ���� -1
			{
				if(flag==1) //���Ϊ-1�����ʾ���ǵ�һ�����أ������ۼ�
				{
				 if(n)s1+=i-t1; //�������½��أ��ߵ�ƽʱ��Ϊ��ǰֵ��������
				 t0=i;//�����½��ı���ֵ
				 if(n > 1){
					Up_Unit_Time = i-t1;
				 }
				 
				 
				 n++;//������+1
				}
				flag=0;//��־Ϊ�͵�ƽ
				//printf("s0=%d,s1=%d,t0=%d,t1=%d,n=%d,i=%d,flag=%d\r\n",s0,s1,t0,t1,n,i,flag);
			}
			if(min>SampleBuffer[i])min=SampleBuffer[i];//��������Сֵ 	
		}
		
		
	}
	duty_s0 = s0;
	duty_s1 = s1;
	printf("\r\n\r N:%d \r\n\r", n);
	/*
		* β�����ڲ��� ʹ��������
	*/

		if(SampleBuffer[0] < Down && SampleBuffer[NPT - 1] < Down) { 
			duty_s0 += Down_Unit_Time;
			n += 1;

		}else if(SampleBuffer[0] > Up && SampleBuffer[NPT - 1] > Up){
			duty_s1 += Up_Unit_Time;
			n += 1;	

		}
	
    wave->duty=duty_s1*10000/(duty_s1+duty_s0);//��λС��
	
	t=(duty_s1+duty_s0)*2.00/(n - 1);
	printf("Up_Unit_Time:%.2f,Down_Unit_Time:%.2f ", Up_Unit_Time, Down_Unit_Time);
	printf("\r\n\r");
	printf("%s,���ڣ�%.2f,��ձ�:%.2f \r\n\r",wave->name,t ,(wave->duty)/100.0);
	
	//printf("�ߵ�ƽʱ��: %.2f, �͵�ƽʱ��:%.2f\r\n\r", duty_s1, duty_s0);
	wave->cycles = t;
	wave->freq = sampler->current_freq_ / t;
	wave->vpp = (max - min) * 3280 / 4096; // mV

	printf("%s,vpp: %.2f\r\n\r", wave->name, wave->vpp);
//	wave->Ma = (float)(max - min) / (float)(max + min);
//		printf("Ma: %.2f\r\n", wave->Ma);
		
//	if(wave->Ma > 0.1 && wave->Ma < 0.2){
//		wave->Ma += 0.05;
//	}if(wave->Ma > 0.2 && wave->Ma < 0.3){
//		wave->Ma += 0.1;
//	} else if (wave->Ma > 0.3 && wave->Ma < 0.4){
//		wave->Ma += 0.12;
//	} else if(wave->Ma > 0.4 && wave->Ma < 0.5){
//		wave->Ma += 0.2;
//	} else if(wave->Ma > 0.5 && wave->Ma < 0.6){
//		wave->Ma += 0.3;
//	} else if(wave->Ma > 0.6 && wave->Ma < 0.7){
//		wave->Ma += 0.35;
//	}else if(wave->Ma > 0.7 && wave->Ma < 0.8){
//		wave->Ma += 0.31;
//	}else if(wave->Ma > 0.8 && wave->Ma < 0.95){
//		wave->Ma += 0.41;
//	}
				
	//wave->Ma = temp_Ma;
	printf("AM������: %.2f%\r\n\r", wave->Ma * 100);
	
	
//		printf("max: %d\r\n\r", max);
//	f_max = 1200 * ((float)max / (1200.0 / 3280.0 * 4096));
//	f_min = 1200 * ((float)min / (1200.0 / 3280.0 * 4096));
//	wave->vpp = f_max - f_min;
	
	return t;
	
}
