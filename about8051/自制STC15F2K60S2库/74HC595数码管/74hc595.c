#include "74hc595.h"
unsigned char code LED[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
unsigned char code pos[] = {0x00,0x01,0x02,0x04,0x08};

// static ������ʾ�������ṩ���ýӿ�
static void delay1us(unsigned int num){
	while(num--);
}

/**
  * @brief  ��һ��8�ֽ����ݷ��͵�74HC595, ������λѡֵ(��һ���������) , Ҳ�����Ƕ�ѡֵ(������ʾ���ַ�).
  *         �ú�����������Դ�ļ��ṩ.
  * @param code_tmp ����ֵ
  */
static void push_data(unsigned char code_tmp){
	unsigned char i;
	for(i = 0; i < 8; i++){
		// �������ݴӸ�λ��ʼ����
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
  * @brief  ������ʾλѡ(��һ���������), ���ļ���������Դ�ļ��ṩ.
  *
  * @param code_pos λѡֵ 0x01 �������ҵ�һ�������, 0x08 ���ұ������ 0x01 0x02 0x04 0x08
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
    delay1us(3000); // ��ʱ3000΢���Ա���õ���ʾЧ����
}

void show(unsigned char buf[4]){
	pos_show(POS_FIRST , LED[buf[0]]);
	pos_show(POS_SECOND, LED[buf[1]]);
	pos_show(POS_THIRD , LED[buf[2]]);
	pos_show(POS_LAST  , LED[buf[3]]);
}
