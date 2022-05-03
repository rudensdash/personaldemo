#ifndef __SPI_H__
#define __SPI_H__

#include "STC15F2K60S2.h"

sbit SS = P1^2;
sbit MOSI = P1^3;
sbit MISO = P1^4;
sbit SCLK = P1^5;

typedef unsigned char BYTE;
typedef unsigned int WORD;

void Spi_init();

unsigned char spi_recircle(unsigned char spi_t);


#endif