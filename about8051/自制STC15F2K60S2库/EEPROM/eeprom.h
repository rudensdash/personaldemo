#ifndef __EEPROM__H
#define __EEPROM__H

#include "STC15F2K60S2.h"
#include "intrins.h"

// 等待时间 参考手册修改
#define WAITTIME 0x03

typedef unsigned char BYTE;
typedef unsigned short WORD;

extern BYTE Edat; // 存储读取值的全局变量，用作第二种获取读取值的途径

enum IAP_CONTROL { IAPEN = 0x80, SWBS = 0x40, SWRST = 0x20, CMD_FAIL = 0x10 };

/**
 *	@brief	EEPROM 工作模式
 */
typedef enum EEPROM_CMD {
  EEPROM_STANDBY = 0x00,
  EEPROM_READ = 0x01,
  EEPROM_WRITE = 0x02,
  EEPROM_ERASE = 0x03
} EEPROM_CMD;

/**
 * @brief 从eeprom读取一个字节返回
 *
 * @param addr 需要读取的目标地址 IAP15L2K61S2 可用地址为 0x0000 到 0xF3FF
 * @return BYTE （unsigned char）
 */
BYTE eeprom_iap_read(WORD addr);

/**
 * @brief 写入一个字节到eeprom指定地址，需要注意的是，当地址本身值为0xFF时，可以
 * 		  进行写操作，而当地址本身值不为0xFF时，则需要在写入前进行擦除。而IAP操作
 *        只有扇区级擦除，没有字节擦除，在写入数据时需要读出要保留的数据，而后写入
 * 		  所以建议把同一批次写操作的数据存到同一扇区。
 *
 * @param addr 需要写入的目标地址
 * @param datA 需要写入的数据
 */
void eeprom_iap_write(WORD addr, BYTE datA);

/**
 * @brief 扇区级擦除，扇区中任一地址就代表扇区的地址。
 *
 * @param addr
 */
void eeprom_iap_erase_sector(WORD addr);

#endif