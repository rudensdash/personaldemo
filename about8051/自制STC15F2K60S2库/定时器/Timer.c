#include "Timer.h"

unsigned int count = 0;		// 定时器0计数初值
unsigned int count1 = 0;	// 定时器1计数初值

unsigned int OPNUM1 = 1000;
unsigned int OPNUM  = 1000;

timer_interrupt_cb_t timer0_callback_function;
timer_interrupt_cb_t timer1_callback_function;

BYTE rfL0, rfH0, rfL1, rfH1;

void timer_interrupt_handler(void) { P00 = ~P00; } // 中断处理函数

void Timer0Init(){
    timer_config_t config; // 配置定时器
    config.work_mode = TWORKMODE_16BIT_AUTOREFILL;
    config.Gate      = 0;
    config.CountMode = 0;
    config.LBuffer   = 0x18;
    config.HBuffer   = 0xFC;
	// 注册定时器中断处理函数
    Timer_interrupt_handler_register(TIMER0, timer_interrupt_handler, 1000);
	// 启动定时器
    TimerInit(TIMER0, config);
}

void TimerInit(timer_num_t timer, timer_config_t config){
    MAIN_INTERRUPT_SWITCH = 1;
    switch (timer) {
    	case TIMER0:
			TIMER_WORKMODE_STATUS &= 0xF0; // 清空低位.
			if (config.Gate) TIMER_WORKMODE_STATUS |= 0x08;
			if (config.CountMode) TIMER_WORKMODE_STATUS |= 0x04;
			TIMER_WORKMODE_STATUS 		 |= config.work_mode;
			TIMER_BUFFER_LOW_TIMER0       = config.LBuffer;
			TIMER_BUFFER_HIGH_TIMER0      = config.HBuffer;
			TIMER_INTERRUPT_SWITCH_TIMER0 = 1;
			rfL0 = config.LBuffer;
			rfH0 = config.HBuffer;
			TIMER_LAUNCH_SWITCH_TIMER0    = 1;
			break;
    	case TIMER1:
			TIMER_WORKMODE_STATUS &= 0x0F; // 清空高位
			if (config.Gate) TIMER_WORKMODE_STATUS |= 0x80;
			if (config.CountMode) TIMER_WORKMODE_STATUS |= 0x40;
			TIMER_WORKMODE_STATUS   |= (config.work_mode << 4);
			TIMER_BUFFER_LOW_TIMER1  = config.LBuffer;
			TIMER_BUFFER_HIGH_TIMER1 = config.HBuffer;
			TIMER_INTERRUPT_SWITCH_TIMER1 = 1;
			rfL1 = config.LBuffer;
			rfH1 = config.HBuffer;
			TIMER_LAUNCH_SWITCH_TIMER1    = 1;
			break;
    }
}

void Refill(timer_num_t timer){
	switch(timer){
		case TIMER0:
			TIMER_BUFFER_LOW_TIMER0 = rfL0;
			TIMER_BUFFER_HIGH_TIMER0 = rfH0;
			break;
		case TIMER1:
			TIMER_BUFFER_LOW_TIMER1 = rfL1;
			TIMER_BUFFER_HIGH_TIMER1 = rfH1;
			break;
    }
}

void Timer_interrupt_handler_register(timer_num_t timer, timer_interrupt_cb_t Func, unsigned int opnum){
	switch(timer){
		case TIMER0:
			timer0_callback_function = Func;
			OPNUM = opnum;
            break;
        case TIMER1:
            timer1_callback_function = Func;
			OPNUM1 = opnum;
            break;
        }
}

void Timer0Interrupt() interrupt 1{
	count++;
	if(count == OPNUM){
		count = 0;
        timer0_callback_function();
    }
	Refill(TIMER0);
}

void Timer1Interrupt() interrupt 3{
	count1++;
	if(count1 == OPNUM1){
		count1 = 0;
        timer1_callback_function();
    }
	Refill(TIMER1);
}