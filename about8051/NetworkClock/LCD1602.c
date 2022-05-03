#include "LCD1602.h"
#define dt 125
// 
/*
void delay1ms(uint c){
	uchar a,b;
	for (; c>0; c--)
	{
		 for (b=199;b>0;b--)
		 {
		  	for(a=1;a>0;a--);
		 }      
	}
}
*/
void delay1us(uint c){
	for(;c > 0; c--);
}

#ifndef LCD1602_4PINS
void LCD_Write_CMD(uchar value){	
	RS = 0;
	RW = 0;
	DataPort = value;
	EN = 1;
	delay1us(dt);
	EN = 0;
	// while EN from 1 level to 0 level ,the command will be run.
}
#else
void LCD_Write_CMD(uchar value){	
	EN = 0;
	RW = 0;
	RS = 0;
	
	DataPort = value; //4λ����LCD�������λ�ӿڣ�ÿ��ֻ�������λ
	delay1us(dt);
	EN = 1;
	delay1us(dt);
	EN = 0;
	
	DataPort = value << 4; //�������λ��
	delay1us(dt);
	
	EN = 1;
	delay1us(dt);
	EN = 0;	


}

#endif

// LCD��Ļд���ݺ���������char����
#ifndef LCD1602_4PINS
void LCD_Write_Data(char value){
	RS = 1;
	RW = 0;
	DataPort = value;
	EN = 1;
	delay1us(dt);
	EN = 0;

}
#else

void LCD_Write_Data(char value)
{
	EN = 0;
	RS = 1;
	RW = 0;
	DataPort = value; //4λ����LCD�������λ�ӿڣ�ÿ��ֻ�������λ
	delay1us(dt);
	EN = 1;
	delay1us(dt);
	EN = 0;
	
	DataPort = value << 4; //�������λ��
	delay1us(dt);
	
	EN = 1;
	delay1us(dt);
	EN = 0;	

}
#endif
// LCD1602 init
#ifndef		LCD1602_4PINS

void LCDInit()						  //LCD��ʼ���ӳ���
{
	LCD_Write_CMD(0x38);  //����ʾ
	LCD_Write_CMD(0x0c);  //����ʾ����ʾ���
	LCD_Write_CMD(0x06);  //дһ��ָ���1
	LCD_Write_CMD(0x01);  //����
	LCD_Write_CMD(0x80);  //��������ָ�����
}

#else

void LCDInit()						  //LCD��ʼ���ӳ���
{
	LCD_Write_CMD(0x32);	 //��8λ����תΪ4λ����
	LCD_Write_CMD(0x28);	 //����λ���µĳ�ʼ��
	LCD_Write_CMD(0x0c);  //����ʾ����ʾ���
	LCD_Write_CMD(0x06);  //дһ��ָ���1
	LCD_Write_CMD(0x01);  //����
	LCD_Write_CMD(0x80);  //��������ָ�����
}

#endif