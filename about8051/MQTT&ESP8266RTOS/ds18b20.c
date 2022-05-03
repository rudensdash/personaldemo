#include "ds18b20.h"
WORD BTDList[8] = {1,2,4,8,16,32,64,128};
double BTDListL[4] = {0.5,0.25,0.125,0.625};
TValue result      = {0, 0, 0};
handler_cb_t callback_handler;

TValue ds18b20_start_template(handler_cb_t func){
    BYTE ValueL      = 0;
    BYTE ValueH      = 0;
	callback_handler = func;
	if(ds18b20_init() == 0){
			/**
			  *	ds18b20每次发送指令都需要先进行初始化。
			  */
			ds18b20_init();
			ds18b20_write_cmd(DS18B20_CMD_SKIP_ROM); // 跳过rom
			ds18b20_write_cmd(DS18B20_CMD_CONVER_TEMPERATURE); // 转换温度
			ds18b20_init();
			ds18b20_write_cmd(DS18B20_CMD_SKIP_ROM); // 跳过rom
			ds18b20_write_cmd(DS18B20_CMD_READ_SCRATCHPAD); // 读取数据
			ValueL = ds18b20_read_byte();
			ValueH = ds18b20_read_byte();
			result = ds18b20_calculate_temperature(ValueH,ValueL);
			callback_handler();
		}
	return result;
}

bit ds18b20_init(){
    bit flag;
    DQ = 0;
    ds18b20_delay(245);
    DQ = 1;
    // 等待ds18b20 自动拉低DQ
    ds18b20_delay(34);
    flag = DQ;
    ds18b20_delay(245);
    return flag;
}
/**
 * 写命令函数
 * 
*/
void ds18b20_write_cmd(BYTE command){

    WORD i;
    BYTE tmp;
    BYTE cmd = command;
    for (i = 0; i < 8; i++){
        tmp = cmd & 0x01;
        cmd >>= 1;
        if(tmp){
            ds18b20_write_one();
        }else{
            ds18b20_write_zero();
        }
    }
}

/**
  *  读函数 单位 bit 以及 BYTE
  */

bit ds18b20_read_bit(){
	bit flag = 0;
	DQ = 0;
	ds18b20_delay(2);
	DQ = 1;
	ds18b20_delay(3);
	if(DQ == 1)
		flag = 1;
	ds18b20_delay(10);
	return flag;
}

BYTE ds18b20_read_byte(){
    BYTE tmp = 0x00;
    WORD i;
    /**
     *  从低位开始读数
    */
    for (i = 0; i < 8; i++) {
		tmp >>= 1;
		if(ds18b20_read_bit() == 1)
			tmp |= 0x80;
        
    }
    return tmp;
}


TValue ds18b20_calculate_temperature(BYTE valueH,BYTE valueL){	
	// 高 8 位 高 5 位为符号位，低3对应 2的6次方 2的5次方 2的4次方。
	// 低 8 位 高 4 位对应 2 的 3 2 1 0 次方 ， 低 4 位对应2的 -1 -2 -3 -4次方（小数部分）
	TValue ResultT;
	BYTE LittlePart;
	BYTE Sign = 0;
	BYTE Integer;
	Sign = valueH & 0xf8;
	Integer = valueH & 0x07; // 0000 0111    0111 0000
	Integer <<= 4;
	Integer = Integer | ((valueL & 0xf0) >> 4);
	LittlePart = valueL & 0x0f;
	
	if(Sign == 0){
		ResultT.Sign = 0;
		P03 = 0;
	}else{
		ResultT.Sign = 1;
		Integer = (~Integer) + 1;
		LittlePart = (~LittlePart) + 1;
		P03 = 1;
	}
	// 转换整数
	ResultT.Integer = Integer;
	// 转换小数
	ResultT.Littlepart = BinaryToDecimal(LittlePart,1);
	
	return ResultT;
}



double BinaryToDecimal(BYTE source,bit flag){
	WORD i;
	double result = 0;
	BYTE tmp = source;
	// 查表法
	if(flag){
		for(i = 0; i < 4; i++){
			if((tmp & 0x01) == 1){
				result += BTDListL[i];
			}
			tmp >>= 1;
		}
	}else{
		for(i = 0; i < 8; i++){
			if((tmp & 0x01) == 1){
				result += (double)(BTDList[i]);
			}
			tmp >>= 1;
		}	
	}


	return result;
}

bit ds18b20_convertT(){
	bit flag;
	ds18b20_write_cmd(0x44);
	flag = DQ;
	return flag;
}

// 针对ds18b20的延时
void ds18b20_delay(WORD num){
    while(num--)
        ;
}


/**
 * 	基层函数实现
 */ 

// 写 0 函数
void ds18b20_write_zero(){
    DQ = 0;
    ds18b20_delay(45);
    DQ = 1;
}

// 写 1 函数
void ds18b20_write_one(){
    DQ = 0;
    ds18b20_delay(2);
    DQ = 1;
    ds18b20_delay(45);
}
