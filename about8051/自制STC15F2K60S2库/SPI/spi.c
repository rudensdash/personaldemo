#include "spi.h"


void Spi_init(){
	SPDAT = 0;
	SPCTL = 0xD0;
}

static void spi_transoverflag_clear(){
	SPSTAT |= 0x80;
}

unsigned char spi_recircle(unsigned char post_data){
	SPDAT = post_data;
	while((SPSTAT & 0x80) == 0);
	spi_transoverflag_clear();
	return SPDAT;
}