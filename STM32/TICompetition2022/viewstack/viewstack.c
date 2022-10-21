#include "viewstack.h"
#include "math.h"

_showStack_t screen;
uint16_t Had_Drawn[320] = {0};
uint16_t ArgWindowsWidth = 0;

const float cur_ampMultiple_FM_2[4]	= {2.6660, 8.40000, 25.80000, 25.80000};

// ջ��ʼ��
void stack_init(){
	int i;
	screen.Size = 0;
	screen.push = screen_stack_push;
	screen.pop = screen_stack_pop;
	for(i = 0; i < MAX_STACK_DEPTH; i++){
		screen.container[i] = 0;
	} // ���ջָ��
}
// ջpush
void screen_stack_push(ScreenPicture_t UI){
	if(screen.Size < MAX_STACK_DEPTH)
		screen.container[screen.Size++] = &(*UI);
}

// ջpop αɾ��
void screen_stack_pop(){
	if(screen.Size > 1){
		screen.container[screen.Size--] = 0; 
	}
}
// ��ʾ���κ��� ����������ʾ������Ҫ�� void xxx(WaveInfo_t* wave) �������Ͳ��ܱ�push��ջ ��ȻҲ����ֱ�ӵ���
void Show_WavePicture(WaveInfo_t* wave){
	uint16_t x;
	//uint16_t distance, distance_Y = 0, distance_X = 0;
	//uint16_t i;
	float ADC_ConvertedValueLocal;
	const uint16_t down_border = 240; // �±߽�Ϊ0  ��������
	uint16_t cur_Y, dst_Y, had_drawn_cur_Y; //��ǰY��ʼ��
		
	// ���е���
	
	// ������
	cur_Y = 0;
	had_drawn_cur_Y = 0; 
	for(x = 0; x < 320 - ArgWindowsWidth; x++){ // ArgWindowsWidth ���в������ڳ��֣�ռ����Ļ���ҵ�λ�ã����δ�������
		//ADC_ConvertedValueLocal = 1.20 * (y_input[x] / (1.20 / 3.3 * 4096));
		if(strcmp(wave->name,"AM" ) == 0){
			ADC_ConvertedValueLocal = (float)y_input1[x] * 3.3 / 4096; // ��ȡ��ѹ
			//ADC_ConvertedValueLocal /= 4;
		}else if(strcmp(wave->name,"FM" ) == 0){
			ADC_ConvertedValueLocal = (float)y_input2[x] * 3.3 / 4096; // ��ȡ��ѹ
			//ADC_ConvertedValueLocal /= fabs(1.5 - ADC_ConvertedValueLocal) / cur_ampMultiple_FM_2[currentGear] + 1.5;
		}else if(strcmp(wave->name,"BASE" ) == 0){
			ADC_ConvertedValueLocal = (float)y_input3[x] * 3.3 / 4096; // ��ȡ��ѹ
		}
		
		dst_Y = down_border - ADC_ConvertedValueLocal / 3.3 * 240; // ������Ӧ��ѹֵ
		// ����
		LCD_SetTextColor(WHITE);
		My_DrawLine_Vertical(x, had_drawn_cur_Y, Had_Drawn[x]); // �Ա���ɫ�ػ��ϴ���������
		
		// ������ 
		/*
		LCD_SetTextColor(GREEN);
		My_DrawLine_Vertical(distance_X, 0, 240);	
		if(distance_X < 320) distance_X += distance;
		else if(distance_X >= 320) distance_X = 0; // �ػ�
		My_DrawLine_Horizental(distance_Y, 0 , 320);
		if(distance_Y < 240) distance_Y += distance; 
		else if(distance_Y >= 240) distance_Y = 0; */
		
		//������
		LCD_SetTextColor(RED);
		My_DrawLine_Vertical(x, cur_Y, dst_Y);
		
		cur_Y = dst_Y;
		had_drawn_cur_Y = Had_Drawn[x]; // �Ѿ���������ʼ��Y
		Had_Drawn[x] = dst_Y; // �Ѿ�������ѹֵY�ĵ��������
	}
	Show_Parameter(&(*wave));
	
}
// ��ʾ�������� ��������Ӧ��ʾ��Ĭ���Ѿ�����ʾ�����е���
// ��ArgWindowsWidth��ֵʱ��ʾ >= 120
void Show_Parameter(WaveInfo_t* wave){
	uint16_t i, d;
	
	char dispbuff[100];
	char waveclass[20];
	d = 16;
	if(ArgWindowsWidth > 0){
		//LCD_SetTextColor(CYAN);
		ILI9341_Clear(320 - ArgWindowsWidth, 0, 0 + ArgWindowsWidth, 240);
		LCD_SetTextColor(BLACK);
		sprintf(dispbuff, "���ֵ:%.2fmV", wave->vpp);
		ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 0, dispbuff);

		if(modulate_flag != 0){
			sprintf(dispbuff, "�ز�Ƶ��:%.2fMHz", Carrier_Wave_Freq); // F_C �ز��ź�
			ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 20, dispbuff);
		}
		
		
		//sprintf(dispbuff, "DCOFF:%.1fV", wave->dc_offset);
		//ILI9341_DispString_EN(320 - ArgWindowsWidth + 10, 60, dispbuff);
		
//		sprintf(dispbuff, "THD:%.2f%%", wave->THD * 100);
//		ILI9341_DispString_EN(320 - ArgWindowsWidth + 10, 80, dispbuff);
		
		if(screen.top == Show_FFT_Picture){ // ֻ����FFT���� ��ʾг��
			for(i = 0; i < 8; i++){
				sprintf(dispbuff, "г��%d:%ld", i+1 , wave->U[i]);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + d, dispbuff);
				d += 16;
			}
		}else{
			if(modulate_flag == 0){
				sprintf(dispbuff, "������:%.2f", wave->Ma);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + 16, dispbuff);
			}else if(modulate_flag == 1){
				sprintf(dispbuff, "��Ƶ��:%.2f", wave->Delta_fm / wave->freq);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + 16, dispbuff);
				sprintf(dispbuff, "���Ƶƫ:%.2fHz", wave->Delta_fm);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + 32, dispbuff);
			}
			
			if(modulate_flag != 3){
				sprintf(dispbuff, "���Ʋ�Ƶ:%.2fKHz", wave->freq / 1000);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + 48, dispbuff);
			}
		}
		if(modulate_flag != 3 )
			sprintf(dispbuff, "��������: %s", wave->name);
		else
			sprintf(dispbuff, "��������: δ����");
		ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + 64, dispbuff);
		
		switch(wave->Class){
			case Sine: // ���Ҳ�
				strcpy(waveclass, "���Ҳ�");
				break;
			case Square: // ����
				strcpy(waveclass, "����");
				sprintf(dispbuff, "ռ�ձ�:%.2f%%", wave->duty / 100);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 40, dispbuff);
				break;
			case Triangle: // ���ǲ�
				strcpy(waveclass, "���ǲ�");
				break;
			case Sawtooth: // ��ݲ�
				strcpy(waveclass, "��ݲ�");
				break;
			case Noise: // ����
				strcpy(waveclass, "����");
				break;
			case Trapezoid: // ���β�
				strcpy(waveclass, "���β�");
				break;
			case Unknown: // δ֪
				strcpy(waveclass, "δʶ��");
				break;
		}
		if(strcmp(wave->name, "BASE") != 0){ // ֻ�н��������ʶ��
			sprintf(dispbuff, "����:%s ", waveclass);
			ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 224, dispbuff);
		}
	
	}
}

void info_window(char* message){
	ILI9341_Clear(0,0,320,240);	/* ���� */
	
}

/*
	��ʾƵ��
*/
void Show_FFT_Picture(WaveInfo_t* wave){
	uint16_t x,i;
	uint16_t temp = 0;
	float f_temp = 0;
	ILI9341_Clear(0, 0, 320, 240);
	i = 0;
	for(x = 0; x < 320 - ArgWindowsWidth; x+= (320 - ArgWindowsWidth) / 8){
		f_temp = (float)wave->U[i] / 4096.0 * 240.0;
		temp = (uint16_t)f_temp;
		temp = (240 - temp); // ����
		LCD_SetTextColor(RED);
		ILI9341_DrawRectangle(x+1, temp , (320 - ArgWindowsWidth) / 16 , 240 - temp  , 1);
		if(i < 7) i++;

	}
	LCD_SetTextColor(BLACK);
	Show_Parameter(&(*wave));
}

// ��װ�Ļ�����
void My_DrawLine_Vertical(uint16_t x, uint16_t start_y, uint16_t end_y){ // ������
	
	ILI9341_DrawLine(x , start_y, x , end_y);

}
// ��װ�Ļ�����
void My_DrawLine_Horizental(uint16_t y, uint16_t start_x, uint16_t end_x){ // ������
	
	ILI9341_DrawLine(start_x , y, end_x , y);

}




