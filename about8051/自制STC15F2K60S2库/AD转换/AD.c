#include "AD.h"

unsigned int result = 0;
unsigned char ch = 0;
unsigned char speed;
double vin       = 0;
bit storeflag    = 0;

adc_mode_cb_t adc_interrupt_callback_func;

void test_adc_interrupt_func(){
	vin = 0;
	// Calculate voltage for input
	vin = CalVoltage(result);
}

void AD_init_default(){
	ADPinOn(POne0); 	// 设置打开外部检测AD引脚
	//ADRJset(BIGHEAD);
    AD_buffer_clear();	// 清除缓冲区
    AD_set_speed(ADC_SPEED_360); // 设置速度
    AD_callback_function_register(test_adc_interrupt_func); //设置中断函数
    AD_open_interrupt(); // 打开中断
    AD_Delay(2);
    AD_start(); // 打开AD转换
}

static void AD_start(){
    ADControl( ADC_START | ADC_POWER | speed | ch );
}

void AD_open_interrupt(){
	MAIN_INTERRUPT_SWITCH = 1;
    ADC_INTTERRUPT_SWITCH = 1;
}

void AD_buffer_clear(){
	result = 0;
	ADC_RES = 0;
	ADC_RESL = 0;
}

bit AD_read_transover_flag(){
	if(ADC_CONTR & ADC_FLAG)
        return 1;
    return 0;
}

void AD_set_channel(adc_channel_t num){
	ch = num;
}

void AD_set_speed(unsigned char spd) { speed = spd; }

void AD_callback_function_register(adc_mode_cb_t func){
	adc_interrupt_callback_func = func;
}

double CalVoltage(unsigned int result){
	return ((result * VCC) / 1024);
}

void AD_get_result(adc_storemode mode){
	switch(mode){
		case BIGHEAD:
			result = ADC_RES; 
			result <<= 2;
			result += (ADC_RESL & 0x03);
            break;
        case LITTLEHEAD:
            result = ADC_RESL;
            result <<= 8;
			result += ADC_RES;
            break;
    }
}

void AD_isr() interrupt 5 {
	ADControl(ADC_CONTR & (!ADC_FLAG)); // clear "interrupt" flag
	// Get the result from buffer
	if(storeflag){
		AD_get_result(LITTLEHEAD);
	}else{
		AD_get_result(BIGHEAD);
	}

    adc_interrupt_callback_func();

    AD_start();
}

static void AD_Delay(unsigned int i){
	unsigned int j;
	while(i--){
		for(j = 5000;j > 0; j--);
	}
}
