#ifndef __AUXR1__H__
#define __AUXR1__H__
#include "STC15F2K60S2.h"

typedef enum {
  // default CCPÔÚP1.2/ECI,P1.1/CCP,P1.0/CCP1,P3.7/CCP2
  CCP_LOCATION_NUM2 =
      0x1, // CCPÔÚP3.4/ECI_2,P3.5/CCP0_2,P3.6/CCP1_2,P3.7/CCP2_2
  CCP_LOCATION_NUM3 =
      0x20 // CCPÔÚ?P2.4/?CI_3,P2.5/CCP?_3,P2.6/CCP1_3,P2.7/CCP2_3?
} ccp_setting_t;

typedef enum {
  // defaut : ON P3.0RXD P3.1TXD
  UART_LOCATION_NUM2 = 0x40, // p3.6 RXD,p3.7 TXD
  UART_LOCATION_NUM3 = 0x80  // p1.6 RXD,p1.7 TXD
} uart_setting_t;

typedef enum {
  // defaut: P1.2 SS ,P1.3 MOSI, P1.4 MISO, P1.5 SCLK
  SPI_LOCATION_NUM2 = 0x04, // P2.4 SS P2.3 MOSI P2.2MISO P2.1 SCLK
  SPI_LOCATION_NUM3 = 0X08  // P5.4 SS P4.0 MOSI P4.1MISO P4.3 SCLK
} spi_setting_t;

void uart_location_set(uart_setting_t loc);
void spi_location_set(spi_setting_t loc);
void ccp_location_set(ccp_setting_t loc);

#endif /*