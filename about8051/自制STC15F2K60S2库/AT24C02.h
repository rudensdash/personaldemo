#ifndef __AT24C02_H_
#define __AT24C02_H_
#include "STC15F2K60S2.h"
#include "INTRINS.H"
sbit SCL = P2 ^ 6;              //AT24C02的时钟
sbit SDA = P2 ^ 7;              //AT24C02的数据

unsigned char AT24C02_Read(unsigned char add);
void AT24C02_Write(unsigned char add, unsigned char val);


#endif