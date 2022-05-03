#include "AT24C02.h"

/**************************************
延时5微秒
**************************************/
void Delay5us()
{
    unsigned char n = 4;

    while (n--)
    {
        _nop_();
        _nop_();
    }
}


/**************************************
24c02起始信号
**************************************/
void AT24C02_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
}

/**************************************
停止信号
**************************************/
void AT24C02_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void AT24C02_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}

/**************************************
接收应答信号
**************************************/
bit AT24C02_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时

    return CY;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void AT24C02_SendByte(unsigned char dat)
{
    unsigned char i;

    for (i = 0; i < 8; i++)     //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    AT24C02_RecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
unsigned char AT24C02_RecvByte()
{
    unsigned char i;
    unsigned char dat = 0;

    SDA = 1;                    //使能内部上拉,准备读取数据
    for (i = 0; i < 8; i++)     //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= SDA;             //读数据
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }

    return dat;
}

/******************************************************/
void AT24C02_Write(unsigned char add, unsigned char val)
{
    AT24C02_Start();            //起始信号
    AT24C02_SendByte(0xa0);     //发送设备地址+写信号10100000，A1AA2A3决定后三位，最后移位是读还是写
    AT24C02_SendByte(add);     //发送存储单元地址
    AT24C02_SendByte(val);
    AT24C02_Stop();             //停止信号
}


/****************************************/
unsigned char AT24C02_Read(unsigned char add)
{
    unsigned char da;
    AT24C02_Start();            //起始信号
    AT24C02_SendByte(0xa0);     //发送设备地址+写信号
    AT24C02_SendByte(add);     //发送存储单元地址
    AT24C02_Start();            //起始信号
    AT24C02_SendByte(0xa1);     //发送设备地址+读信号
    da = AT24C02_RecvByte();
    AT24C02_SendACK(1);
    AT24C02_Stop();             //停止信号
    return da;
}
