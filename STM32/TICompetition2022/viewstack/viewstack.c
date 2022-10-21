#include "viewstack.h"
#include "math.h"

_showStack_t screen;
uint16_t Had_Drawn[320] = {0};
uint16_t ArgWindowsWidth = 0;

const float cur_ampMultiple_FM_2[4]	= {2.6660, 8.40000, 25.80000, 25.80000};

// 栈初始化
void stack_init(){
	int i;
	screen.Size = 0;
	screen.push = screen_stack_push;
	screen.pop = screen_stack_pop;
	for(i = 0; i < MAX_STACK_DEPTH; i++){
		screen.container[i] = 0;
	} // 清空栈指针
}
// 栈push
void screen_stack_push(ScreenPicture_t UI){
	if(screen.Size < MAX_STACK_DEPTH)
		screen.container[screen.Size++] = &(*UI);
}

// 栈pop 伪删除
void screen_stack_pop(){
	if(screen.Size > 1){
		screen.container[screen.Size--] = 0; 
	}
}
// 显示波形函数 后面新增显示函数都要求 void xxx(WaveInfo_t* wave) 这种类型才能被push入栈 当然也可以直接调用
void Show_WavePicture(WaveInfo_t* wave){
	uint16_t x;
	//uint16_t distance, distance_Y = 0, distance_X = 0;
	//uint16_t i;
	float ADC_ConvertedValueLocal;
	const uint16_t down_border = 240; // 下边界为0  向上增长
	uint16_t cur_Y, dst_Y, had_drawn_cur_Y; //当前Y起始点
		
	// 画中点线
	
	// 画波形
	cur_Y = 0;
	had_drawn_cur_Y = 0; 
	for(x = 0; x < 320 - ArgWindowsWidth; x++){ // ArgWindowsWidth 当有参数窗口出现，占用屏幕靠右的位置，波形窗口缩短
		//ADC_ConvertedValueLocal = 1.20 * (y_input[x] / (1.20 / 3.3 * 4096));
		if(strcmp(wave->name,"AM" ) == 0){
			ADC_ConvertedValueLocal = (float)y_input1[x] * 3.3 / 4096; // 获取电压
			//ADC_ConvertedValueLocal /= 4;
		}else if(strcmp(wave->name,"FM" ) == 0){
			ADC_ConvertedValueLocal = (float)y_input2[x] * 3.3 / 4096; // 获取电压
			//ADC_ConvertedValueLocal /= fabs(1.5 - ADC_ConvertedValueLocal) / cur_ampMultiple_FM_2[currentGear] + 1.5;
		}else if(strcmp(wave->name,"BASE" ) == 0){
			ADC_ConvertedValueLocal = (float)y_input3[x] * 3.3 / 4096; // 获取电压
		}
		
		dst_Y = down_border - ADC_ConvertedValueLocal / 3.3 * 240; // 换算点对应电压值
		// 清屏
		LCD_SetTextColor(WHITE);
		My_DrawLine_Vertical(x, had_drawn_cur_Y, Had_Drawn[x]); // 以背景色重画上次遗留波形
		
		// 画方格 
		/*
		LCD_SetTextColor(GREEN);
		My_DrawLine_Vertical(distance_X, 0, 240);	
		if(distance_X < 320) distance_X += distance;
		else if(distance_X >= 320) distance_X = 0; // 重画
		My_DrawLine_Horizental(distance_Y, 0 , 320);
		if(distance_Y < 240) distance_Y += distance; 
		else if(distance_Y >= 240) distance_Y = 0; */
		
		//画波形
		LCD_SetTextColor(RED);
		My_DrawLine_Vertical(x, cur_Y, dst_Y);
		
		cur_Y = dst_Y;
		had_drawn_cur_Y = Had_Drawn[x]; // 已经画过的起始点Y
		Had_Drawn[x] = dst_Y; // 已经画过电压值Y的点数组更新
	}
	Show_Parameter(&(*wave));
	
}
// 显示参数函数 做了自适应显示，默认已经在显示波形中调用
// 当ArgWindowsWidth有值时显示 >= 120
void Show_Parameter(WaveInfo_t* wave){
	uint16_t i, d;
	
	char dispbuff[100];
	char waveclass[20];
	d = 16;
	if(ArgWindowsWidth > 0){
		//LCD_SetTextColor(CYAN);
		ILI9341_Clear(320 - ArgWindowsWidth, 0, 0 + ArgWindowsWidth, 240);
		LCD_SetTextColor(BLACK);
		sprintf(dispbuff, "峰峰值:%.2fmV", wave->vpp);
		ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 0, dispbuff);

		if(modulate_flag != 0){
			sprintf(dispbuff, "载波频率:%.2fMHz", Carrier_Wave_Freq); // F_C 载波信号
			ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 20, dispbuff);
		}
		
		
		//sprintf(dispbuff, "DCOFF:%.1fV", wave->dc_offset);
		//ILI9341_DispString_EN(320 - ArgWindowsWidth + 10, 60, dispbuff);
		
//		sprintf(dispbuff, "THD:%.2f%%", wave->THD * 100);
//		ILI9341_DispString_EN(320 - ArgWindowsWidth + 10, 80, dispbuff);
		
		if(screen.top == Show_FFT_Picture){ // 只有在FFT界面 显示谐波
			for(i = 0; i < 8; i++){
				sprintf(dispbuff, "谐波%d:%ld", i+1 , wave->U[i]);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + d, dispbuff);
				d += 16;
			}
		}else{
			if(modulate_flag == 0){
				sprintf(dispbuff, "调幅度:%.2f", wave->Ma);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + 16, dispbuff);
			}else if(modulate_flag == 1){
				sprintf(dispbuff, "调频度:%.2f", wave->Delta_fm / wave->freq);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + 16, dispbuff);
				sprintf(dispbuff, "最大频偏:%.2fHz", wave->Delta_fm);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + 32, dispbuff);
			}
			
			if(modulate_flag != 3){
				sprintf(dispbuff, "调制波频:%.2fKHz", wave->freq / 1000);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + 48, dispbuff);
			}
		}
		if(modulate_flag != 3 )
			sprintf(dispbuff, "调制类型: %s", wave->name);
		else
			sprintf(dispbuff, "调制类型: 未调制");
		ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 80 + 64, dispbuff);
		
		switch(wave->Class){
			case Sine: // 正弦波
				strcpy(waveclass, "正弦波");
				break;
			case Square: // 方波
				strcpy(waveclass, "方波");
				sprintf(dispbuff, "占空比:%.2f%%", wave->duty / 100);
				ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 40, dispbuff);
				break;
			case Triangle: // 三角波
				strcpy(waveclass, "三角波");
				break;
			case Sawtooth: // 锯齿波
				strcpy(waveclass, "锯齿波");
				break;
			case Noise: // 噪声
				strcpy(waveclass, "噪声");
				break;
			case Trapezoid: // 梯形波
				strcpy(waveclass, "梯形波");
				break;
			case Unknown: // 未知
				strcpy(waveclass, "未识别");
				break;
		}
		if(strcmp(wave->name, "BASE") != 0){ // 只有解调波形做识别
			sprintf(dispbuff, "波形:%s ", waveclass);
			ILI9341_DispString_EN_CH(320 - ArgWindowsWidth + 10, 224, dispbuff);
		}
	
	}
}

void info_window(char* message){
	ILI9341_Clear(0,0,320,240);	/* 清屏 */
	
}

/*
	显示频谱
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
		temp = (240 - temp); // 求补码
		LCD_SetTextColor(RED);
		ILI9341_DrawRectangle(x+1, temp , (320 - ArgWindowsWidth) / 16 , 240 - temp  , 1);
		if(i < 7) i++;

	}
	LCD_SetTextColor(BLACK);
	Show_Parameter(&(*wave));
}

// 封装的画竖线
void My_DrawLine_Vertical(uint16_t x, uint16_t start_y, uint16_t end_y){ // 画竖线
	
	ILI9341_DrawLine(x , start_y, x , end_y);

}
// 封装的画横线
void My_DrawLine_Horizental(uint16_t y, uint16_t start_x, uint16_t end_x){ // 画横线
	
	ILI9341_DrawLine(start_x , y, end_x , y);

}




