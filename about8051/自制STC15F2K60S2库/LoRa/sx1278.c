#include "STC15F2K60S2.H"
sbit CS = P2^4;
sbit TR = P2^5;
// SPI 初始化
void Spi_Init(void)
{
	SPCTL = 0xD1; // 忽略 SS，允许 SPI，主机模式，16 分频
	AUXR1 |= 4; // 选择引脚 P21~P24
}
// SPI 读写：读时地址字节的最高位为 0，写时为 1，读时 ucData 无效
unsigned char Spi_Rw(unsigned char ucAddr, unsigned char ucData)
{
	CS = 0;
	SPDAT = ucAddr; // 发送地址
	while((SPSTAT & 0x80) == 0); // 等待发送完成
	SPSTAT |= 0x80; // 清除 SPIF
	SPDAT = ucData; // 发送数据
	while((SPSTAT & 0x80) == 0); // 等待发送完成
	SPSTAT |= 0x80; // 清除 SPIF
	CS = 1;
	return SPDAT; // 返回接收数据
}
// 设置射频频率（137~525MHz）
void SX1278LoRaSetRFFrequency(unsigned long freq)
{
	freq = (freq / 32) << 19;
	Spi_Rw(0x06|0x80, freq >> 16);
	Spi_Rw(0x07|0x80, (freq >> 8) & 0xFF);
	Spi_Rw(0x08|0x80, freq & 0xFF);
}
// 设置射频功率（2~20dBm）
void SX1278LoRaSetRFPower(unsigned char power)
{
	power -= 2;
	if(power > 15)
	{
		power -= 3;
		Spi_Rw(0x4D|0x80, 0x87); // 设置高功率
	}
	Spi_Rw(0x09|0x80, power|0x80); // 设置功率
}
// 设置信号带宽（0~9）
void SX1278LoRaSetBW(unsigned char bw)
{
	unsigned char ret;
	ret = Spi_Rw(0x1D, 0);
	ret &= 0x0F;
	ret |= bw<<4;
	Spi_Rw(0x1D|0x80, ret);
}
// 设置纠错编码率（1~4）
void SX1278LoRaSetCR(unsigned char cr)
{
	unsigned char ret;
	ret = Spi_Rw(0x1D, 0);
	ret &= 0xF1;
	ret |= cr<<1;
	Spi_Rw(0x1D|0x80, ret);
}
// LoRa 初始化
void SX1278LoraInit(void)
{
	Spi_Rw(0x01|0x80, 0); // 设置睡眠模式
	Spi_Rw(0x01|0x80, 0x80); // 设置 LoRa 模式
	Spi_Rw(0x01|0x80, 1); // 设置待机模式
	SX1278LoRaSetRFFrequency(430);// 设置射频频率（137~525MHz）
	SX1278LoRaSetRFPower(10); // 设置射频功率（2~20dBm）
	Spi_Rw(0x1E|0x80, 7<<4); // 设置扩频因子（7~12）128
	SX1278LoRaSetBW(7); // 设置信号带宽（0~9）
	SX1278LoRaSetCR(1); // 设置纠错编码率（1~4）
	TR = 0;
	Spi_Rw(0x01|0x80, 5); // 设置连续接收模式
}
// LoRa 发送: pucBuf-发送数据，ucSize-数据个数
void SX1278LoraTx(unsigned char *pucBuf, unsigned char ucSize)
{
	unsigned char i, ret;
	TR = 1;
	Spi_Rw(0x01|0x80, 1); // 设置待机模式
	ret = Spi_Rw(0x0E, 0); // 读取 FifoTxBaseAddr
	Spi_Rw(0x0D|0x80, ret); // 设置 FifoAddrPtr
	Spi_Rw(0x22|0x80, ucSize); // 设置 PayloadLength
	for(i=0; i<ucSize; i++) // 写数据到 FIFO
	Spi_Rw(0x00|0x80, pucBuf[i]);
	Spi_Rw(0x01|0x80, 3); // 设置发送模式
	do
		{
			ret = Spi_Rw(0x12, 0); // 读标志
		}while(ret & 8 == 0); // 等待发送完成
	Spi_Rw(0x12|0x80, 8); // 清除发送完成
	TR = 0;
	Spi_Rw(0x01|0x80, 5); // 设置连续接收模式
}
// Lora 接收: pucBuf-接收数据，返回值-数据个数
unsigned char SX1278LoraRx(unsigned char *pucBuf)
{
	unsigned char i, ret;
	ret = Spi_Rw(0x12, 0); // 读标志
	if(ret & 0x40) // 接收完成
{
	Spi_Rw(0x01|0x80, 1); // 设置待机模式
	Spi_Rw(0x12|0x80, 0x40); // 清除接收完成
	ret = Spi_Rw(0x10, 0); // 读取 FifoRxCurrentAddr
	Spi_Rw(0x0D|0x80, ret); // 设置 FifoAddrPtr
	ret = Spi_Rw(0x13, 0); // 读取 RxNbBytes
	for(i=0; i<ret; i++)
	pucBuf[i] = Spi_Rw(0, 0);// 从 FIFO 读数据
	Spi_Rw(0x01|0x80, 5); // 设置连续接收模式
}
	else
	ret = 0;
	return ret;
}