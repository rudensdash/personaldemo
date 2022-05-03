#include "uart.h"


bit TransfersStates = 1;


void uart_init(){
	AUXR = 0x40;
	AUXR1 |= 0x80;
	TMOD = 0x00;
	SCON = 0x50;
	TL1 = (65536 - (SYSTEMCLOCK / 4 / BAUD));
	TH1 = (65536 - (SYSTEMCLOCK / 4/ BAUD) >> 8);
	TR1 = 1;
}

void uart_interrupt_enable(){
	EA = 1;
	ES = 1;
}

void uart_send_data(const unsigned char *string){
	unsigned int i = 0;
    TransfersStates = 1;
	while(string[i] != '\0'){
		SBUF = string[i++];
		while(!TI);
		TI = 0;
	}
	TransfersStates = 0;
}

	
#ifndef TIMER_DELAY
void Delay(unsigned int count){
	unsigned int i;
	while(count--)
		for(i = 0; i < 256; i++);
}
#endif
