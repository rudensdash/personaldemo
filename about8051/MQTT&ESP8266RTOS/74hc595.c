#include "74hc595.h"
unsigned char code LED[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char code pos[] = {0x00,0x01,0x02,0x04,0x08};
unsigned total = 0;
unsigned int count = 0;
bit TimerOverflow = 0;


void push_data(unsigned char code_tmp){
	unsigned char i;
	for(i = 0; i < 8; i++){
		if(code_tmp & 0x80){
			DS = 1;
		}else{
			DS = 0;
		}
		SH = 1;
		code_tmp <<= 1;
		SH = 0;
	}

}


void set_view(unsigned char code_pos){
	SH = 0;
	OE = 0;
	push_data(code_pos);
}

void pos_show(unsigned char pos , unsigned char context){
	set_view(0);
	set_view(pos);
	push_data(context);
	OE = 0;
	ST = 0;
	ST = 1;
}

// Timer Delay 
#ifdef TIMER_DELAY

void TimerInit(){
	TMOD &= 0xF0; //Selected Timer0 ,Working in mode 0.
	TH0 = 0xFC;
	TL0 = 0x18;
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}


void Timer_delay() interrupt 1 {
	count++;
	if(count == 1000){
		count = 0;
		total++;
		if(total > 9999)
			total = 0;
		TimerOverflow = 1;
	}
}


#endif
