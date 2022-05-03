#include "eeprom.h"


BYTE Edat = 0;

static void eeprom_iap_set(EEPROM_CMD command){
	IAP_CMD = command;
}

static void eeprom_iap_trigger(){
	IAP_TRIG = 0x5A;
	IAP_TRIG = 0xA5;
}

static void eeprom_iap_clean(){
	IAP_CONTR = 0;
	IAP_CMD = 0;
	IAP_TRIG = 0;
	IAP_ADDRH = 0x80;
	IAP_ADDRL = 0;	
}


// 读 eeprom 
BYTE eeprom_iap_read(WORD addr){
	IAP_CONTR = IAPEN | WAITTIME;
	eeprom_iap_set(EEPROM_READ);
	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;
	eeprom_iap_trigger();
	_nop_();
	Edat = IAP_DATA;
	eeprom_iap_clean();

	return Edat;
}
// 写 eeprom
void eeprom_iap_write(WORD addr, BYTE datA){
	IAP_CONTR = IAPEN | WAITTIME;
	eeprom_iap_set(EEPROM_WRITE);
	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;
	IAP_DATA = datA;                  
	eeprom_iap_trigger();
	_nop_();
	eeprom_iap_clean();
}
// eeprom 没有字节擦除 只有扇区擦除
// 扇区中任意一个地址都是扇区的地址，不需要求出首地址。

void eeprom_iap_erase_sector(WORD addr){
	IAP_CONTR = IAPEN | WAITTIME;	
	//eeprom_lowpower_check();	
	eeprom_iap_set(EEPROM_ERASE);
	IAP_ADDRL = addr;
	IAP_ADDRH = addr >> 8;
	eeprom_iap_trigger();
	_nop_();
	eeprom_iap_clean();
}

