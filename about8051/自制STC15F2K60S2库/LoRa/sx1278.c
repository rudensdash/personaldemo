#include "STC15F2K60S2.H"
sbit CS = P2^4;
sbit TR = P2^5;
// SPI ��ʼ��
void Spi_Init(void)
{
	SPCTL = 0xD1; // ���� SS������ SPI������ģʽ��16 ��Ƶ
	AUXR1 |= 4; // ѡ������ P21~P24
}
// SPI ��д����ʱ��ַ�ֽڵ����λΪ 0��дʱΪ 1����ʱ ucData ��Ч
unsigned char Spi_Rw(unsigned char ucAddr, unsigned char ucData)
{
	CS = 0;
	SPDAT = ucAddr; // ���͵�ַ
	while((SPSTAT & 0x80) == 0); // �ȴ��������
	SPSTAT |= 0x80; // ��� SPIF
	SPDAT = ucData; // ��������
	while((SPSTAT & 0x80) == 0); // �ȴ��������
	SPSTAT |= 0x80; // ��� SPIF
	CS = 1;
	return SPDAT; // ���ؽ�������
}
// ������ƵƵ�ʣ�137~525MHz��
void SX1278LoRaSetRFFrequency(unsigned long freq)
{
	freq = (freq / 32) << 19;
	Spi_Rw(0x06|0x80, freq >> 16);
	Spi_Rw(0x07|0x80, (freq >> 8) & 0xFF);
	Spi_Rw(0x08|0x80, freq & 0xFF);
}
// ������Ƶ���ʣ�2~20dBm��
void SX1278LoRaSetRFPower(unsigned char power)
{
	power -= 2;
	if(power > 15)
	{
		power -= 3;
		Spi_Rw(0x4D|0x80, 0x87); // ���ø߹���
	}
	Spi_Rw(0x09|0x80, power|0x80); // ���ù���
}
// �����źŴ���0~9��
void SX1278LoRaSetBW(unsigned char bw)
{
	unsigned char ret;
	ret = Spi_Rw(0x1D, 0);
	ret &= 0x0F;
	ret |= bw<<4;
	Spi_Rw(0x1D|0x80, ret);
}
// ���þ�������ʣ�1~4��
void SX1278LoRaSetCR(unsigned char cr)
{
	unsigned char ret;
	ret = Spi_Rw(0x1D, 0);
	ret &= 0xF1;
	ret |= cr<<1;
	Spi_Rw(0x1D|0x80, ret);
}
// LoRa ��ʼ��
void SX1278LoraInit(void)
{
	Spi_Rw(0x01|0x80, 0); // ����˯��ģʽ
	Spi_Rw(0x01|0x80, 0x80); // ���� LoRa ģʽ
	Spi_Rw(0x01|0x80, 1); // ���ô���ģʽ
	SX1278LoRaSetRFFrequency(430);// ������ƵƵ�ʣ�137~525MHz��
	SX1278LoRaSetRFPower(10); // ������Ƶ���ʣ�2~20dBm��
	Spi_Rw(0x1E|0x80, 7<<4); // ������Ƶ���ӣ�7~12��128
	SX1278LoRaSetBW(7); // �����źŴ���0~9��
	SX1278LoRaSetCR(1); // ���þ�������ʣ�1~4��
	TR = 0;
	Spi_Rw(0x01|0x80, 5); // ������������ģʽ
}
// LoRa ����: pucBuf-�������ݣ�ucSize-���ݸ���
void SX1278LoraTx(unsigned char *pucBuf, unsigned char ucSize)
{
	unsigned char i, ret;
	TR = 1;
	Spi_Rw(0x01|0x80, 1); // ���ô���ģʽ
	ret = Spi_Rw(0x0E, 0); // ��ȡ FifoTxBaseAddr
	Spi_Rw(0x0D|0x80, ret); // ���� FifoAddrPtr
	Spi_Rw(0x22|0x80, ucSize); // ���� PayloadLength
	for(i=0; i<ucSize; i++) // д���ݵ� FIFO
	Spi_Rw(0x00|0x80, pucBuf[i]);
	Spi_Rw(0x01|0x80, 3); // ���÷���ģʽ
	do
		{
			ret = Spi_Rw(0x12, 0); // ����־
		}while(ret & 8 == 0); // �ȴ��������
	Spi_Rw(0x12|0x80, 8); // ����������
	TR = 0;
	Spi_Rw(0x01|0x80, 5); // ������������ģʽ
}
// Lora ����: pucBuf-�������ݣ�����ֵ-���ݸ���
unsigned char SX1278LoraRx(unsigned char *pucBuf)
{
	unsigned char i, ret;
	ret = Spi_Rw(0x12, 0); // ����־
	if(ret & 0x40) // �������
{
	Spi_Rw(0x01|0x80, 1); // ���ô���ģʽ
	Spi_Rw(0x12|0x80, 0x40); // ����������
	ret = Spi_Rw(0x10, 0); // ��ȡ FifoRxCurrentAddr
	Spi_Rw(0x0D|0x80, ret); // ���� FifoAddrPtr
	ret = Spi_Rw(0x13, 0); // ��ȡ RxNbBytes
	for(i=0; i<ret; i++)
	pucBuf[i] = Spi_Rw(0, 0);// �� FIFO ������
	Spi_Rw(0x01|0x80, 5); // ������������ģʽ
}
	else
	ret = 0;
	return ret;
}