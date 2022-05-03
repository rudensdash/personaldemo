#include "74hc595.h"
unsigned char code LED[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char code pos[] = {0x00,0x01,0x02,0x04,0x08};

// static 函数表示不对外提供调用接口
static void delay1us(unsigned int num){
	while(num--);
}

/**
  * @brief  将一个8字节数据发送到74HC595, 可以是位选值(哪一个数码管亮) , 也可以是段选值(具体显示的字符).
  *         该函数不对其他源文件提供.
  * @param code_tmp 数据值
  */
static void push_data(unsigned char code_tmp){
	unsigned char i;
	for(i = 0; i < 8; i++){
		// 发送数据从高位开始发送
		if(code_tmp & 0x80){
			DS = 1;
		}else{
			DS = 0;
		}
		SH = 1;
		code_tmp <<= 1;
		SH = 0;
	}

}

/**
  * @brief  设置显示位选(哪一个数码管亮), 该文件不对其他源文件提供.
  *
  * @param code_pos 位选值 0x01 从左往右第一个数码管, 0x08 最右边数码管 0x01 0x02 0x04 0x08
  */
static void set_view(unsigned char code_pos){
	SH = 0;
	OE = 0;
	push_data(code_pos);
}

void pos_show(unsigned char pos , unsigned char context){
	set_view(0);
	set_view(pos);
	push_data(context);
	OE = 0;
	ST = 0;
	ST = 1;
    delay1us(3000); // 延时3000微秒以便更好地显示效果。
}

void show(unsigned char buf[4]){
	pos_show(POS_FIRST , LED[buf[0]]);
	pos_show(POS_SECOND, LED[buf[1]]);
	pos_show(POS_THIRD , LED[buf[2]]);
	pos_show(POS_LAST  , LED[buf[3]]);
}
