#ifndef __UART_H
#define __UART_H

#include "STC15F2K60S2.h"
#include <intrins.h>
#define BAUD 115200 // baud rate
#define SYSTEMCLOCK 11059200L // system clock frequency

void uart_init();

void uart_send_data(const unsigned char *string);

void Delay(unsigned int count);
void uart_interrupt_enable();

#endif