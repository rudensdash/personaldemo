#include "uart.h"
#include "STC15F2K60S2.h"
#include <intrins.h>
bit TransfersStates = 1;
unsigned int count = 0;
uart_interrupt_cb_t uart_on_send_complete = on_uart_send_complete_default;
uart_interrupt_cb_t uart_on_recv_complete = on_uart_recv_complete_default;
uart_interrupt_cb_t uart_on_default = on_uart_interrupt_default;
char Buffer[20];

void on_uart_send_complete_default(void) { TI = 0; }
void on_uart_recv_complete_default(void) { RI = 0;}
void on_uart_interrupt_default(void) { RI = 0; }

void uart_init(){
	TMOD = 0x00;
	SCON = 0x50;
	AUXR = 0x40;
	TL1 = (65536 - (SYSTEMCLOCK / 4 / BAUD));
	TH1 = (65536 - (SYSTEMCLOCK / 4/ BAUD) >> 8);
	TR1 = 1;
}

void uart_interrupt_enable(){
	EA = 1;
	ES = 1;
}

void uart_interrupt_handler_register(uart_interrupt_cb_t on_send, 
										uart_interrupt_cb_t on_recv,
										uart_interrupt_cb_t _default){
	uart_on_send_complete = on_send;
	uart_on_recv_complete = on_recv;
    uart_on_default       = _default;
}

void uart_send_data(char *string){
	unsigned int i = 0;
    TransfersStates = 1;
	while(string[i] != '\0'){
		SBUF = string[i++];
		while(!TI);
		TI = 0;
	}
	TransfersStates = 0;
	
}


void uart_interrupt() interrupt 4{
	if(TI){
        uart_on_send_complete();
		TI = 0;
    }

	if(RI){
		uart_on_recv_complete();
        RI = 0;
    }

    uart_on_default();
}
