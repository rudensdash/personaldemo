#include <reg52.h>
#include <intrins.h>
#include "LCD1602.h"
#define uchar unsigned char
#define uint unsigned int
#define MAXCOUNT 20
uchar S = 0;
bit TimerFlag;
sbit TimeKey = P3^1;
char* order = "!time";
char COUNT = 0;
char tempbuf[9];
char RcvTime[9] = {0};
bit UpdataTimeFlag;

// 时间结构体
struct time{
	int hour;
	int min;
	int sec;
}time;



// system Init
void system_Init();


// Cstr 转 Cint 

int StoN(char* str);
// Cint to Cstr
char* NtoS(int num,char* str);
// Time Fromat

void TimeFormat(char* TimeString);
// 格式化输出
void printFormat();


void Calculate();

// 串口发送请求时间

void timeRequest();

// 输出到液晶屏

void PrintData(char* Text);

// Timer Init

void Timer0Init();

// Usart Init

void Usart_Init();