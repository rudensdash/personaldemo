#include "ds18b20.h"
#include "74hc595.h"
#include "uart.h"

BYTE temp_publish[8] = { 0 }; 
WORD temp_dat = 0;
void ds18b20_show(TValue obj);

void delay1us(unsigned int num){
	while(num--);
}

void delay1s(unsigned int num){
	unsigned int i;
	while(num--){
		for(i = 0; i < 1250; i++);
	}
}

void combine_temperature(TValue obj){
	if(obj.Integer > 100){
			temp_publish[0] = obj.Integer / 100 + '0';
			temp_publish[1] = ((obj.Integer / 10) % 10) + '0';
			temp_publish[2] = (obj.Integer % 10) + '0';
			temp_publish[3] = '\0';
	}else{
		if(obj.Sign){
			temp_publish[0] = '-';
			temp_publish[1] = (obj.Integer / 10) + '0';
			temp_publish[2] = (obj.Integer % 10) + '0';
			temp_publish[3] = '\0';
		}else{
			temp_publish[0] = (obj.Integer / 10) + '0';
			temp_publish[1] = (obj.Integer % 10) + '0';
			temp_publish[2] = '\0';
		}
	}
}

void handler(){
	combine_temperature(result);
	if(result.Integer != temp_dat){
		uart_send_data((const unsigned char*)temp_publish);
		temp_dat = result.Integer;
	}	
}

int main(){
	uart_init();
    while(1){
        ds18b20_show(ds18b20_start_template(handler));
	}
}


// ��ʾ - ���� 74hc595
void ds18b20_show(TValue obj){
	WORD tmp = obj.Littlepart * 10000;

	pos_show(0x08,LED[(tmp / 100) % 10]);
	delay1us(3000);
	pos_show(0x04,LED[tmp / 10000]);
	delay1us(3000);
	pos_show(0x02,LED[obj.Integer % 10] & 0x7f);
	delay1us(3000);
	pos_show(0x01,LED[(obj.Integer / 10) % 10]);
	delay1us(3000);
	//pos_show(0x01,LED[obj.Integer / 100]);
	
}