#ifndef __SX1278_H
#define __SX1278_H
void Spi_Init(void);
void SX1278LoraInit(void);
void SX1278LoraTx(unsigned char *pucBuf, unsigned char ucSize);
unsigned char SX1278LoraRx(unsigned char *pucBuf);
#endif