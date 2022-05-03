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
	
	DataPort = value; //4位总线LCD接入高四位接口，每次只传输高四位
	delay1us(dt);
	EN = 1;
	delay1us(dt);
	EN = 0;
	
	DataPort = value << 4; //传输低四位。
	delay1us(dt);
	
	EN = 1;
	delay1us(dt);
	EN = 0;	


}

#endif

// LCD屏幕写数据函数，参数char类型
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
	DataPort = value; //4位总线LCD接入高四位接口，每次只传输高四位
	delay1us(dt);
	EN = 1;
	delay1us(dt);
	EN = 0;
	
	DataPort = value << 4; //传输低四位。
	delay1us(dt);
	
	EN = 1;
	delay1us(dt);
	EN = 0;	

}
#endif
// LCD1602 init
#ifndef		LCD1602_4PINS

void LCDInit()						  //LCD初始化子程序
{
	LCD_Write_CMD(0x38);  //开显示
	LCD_Write_CMD(0x0c);  //开显示不显示光标
	LCD_Write_CMD(0x06);  //写一个指针加1
	LCD_Write_CMD(0x01);  //清屏
	LCD_Write_CMD(0x80);  //设置数据指针起点
}

#else

void LCDInit()						  //LCD初始化子程序
{
	LCD_Write_CMD(0x32);	 //将8位总线转为4位总线
	LCD_Write_CMD(0x28);	 //在四位线下的初始化
	LCD_Write_CMD(0x0c);  //开显示不显示光标
	LCD_Write_CMD(0x06);  //写一个指针加1
	LCD_Write_CMD(0x01);  //清屏
	LCD_Write_CMD(0x80);  //设置数据指针起点
}

#endif