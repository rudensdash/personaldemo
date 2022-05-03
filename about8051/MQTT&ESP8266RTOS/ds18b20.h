#ifndef __DS18B20__H
#define __DS18B20__H
#include "STC15F2K60S2.H"
#include <intrins.h>

// --------------- DS18B20 命令表 ------------------
#define DS18B20_CMD_SKIP_ROM 0xCC           // 跳过ROM
#define DS18B20_CMD_READ_ROM 0x33           // 读ROM
#define DS18B20_CMD_MATCH_ROM 0x55          //
#define DS18B20_CMD_CONVER_TEMPERATURE 0x44 // 温度转换开启
#define DS18B20_CMD_ALARM_SEARCH 0xEC       // 报警搜索
#define DS18B20_CMD_WRITE_SCRATCHPAD 0x4E   // 写 scratch pad
#define DS18B20_CMD_READ_SCRATCHPAD 0xBE    // 读 scratch pad
#define DS18B20_CMD_COPY_SCRATCHPAD 0x48    // 复制 scratch pad
#define DS18B20_CMD_READ_POWER_SUPPLY 0XB4  // 读取DS18B20电源供应状态
#define DS18B20_CMD_RECALL 0xB8             // 读取RECALL状态
// -----—— PS: scratch pad 就是用来存储温度数据的 ------

/*
 *  外置链接配置 （配置数据传递引脚）
 */
sbit DQ = P1 ^ 4;
//------------------------------------
typedef unsigned char BYTE;
typedef unsigned int WORD;

// --- 类型定义 ---

typedef struct TValue {
  BYTE Sign;         // postive 正 0 negative 负 1
  WORD Integer;      // 温度整数部分
  double Littlepart; // 温度小数部分
} TValue;

/**
 * @brief 用来转换二进制数据所用的转换表.
 */
extern WORD BTDList[8];
extern double BTDListL[4];
extern TValue result;

typedef void (*handler_cb_t)(void); // 回调处理函数

// ------------------------------------

/* demo -

  void handler(){  //handler 内的处理根据需要
    combine_temperature(result);
    if(result.Integer != temp_dat){
      uart_send_data((const unsigned char*)temp_publish);
      temp_dat = result.Integer;
    }
  }

  void main(){
    uart_init();
      while(1){
          ds18b20_show(ds18b20_start_template(handler));
          // ds1b20_show 为显示函数
    }
  }


*/

/**
 *   温度处理函数在中函数中进行定义，完成后作为启动模板函数的实参传入启动模板，
 *   模板内部通过调用函数指针来执行对应的操作，需要注意的是，result是一个跨
 *   文件的全局变量，所以*无需*再定义自己的result，result变量对应转换后的温度。直接
 *   对result变量进行处理，即是对温度的处理。
 */

/**
 * @brief  温度转换的启动模板，传入一个回调处理函数，用来处理得到的温度数据。
 *
 * @param func 回调处理函数
 * @return TValue
 */
TValue ds18b20_start_template(handler_cb_t func);

/**
 * @brief 依照ds18b20手册中的初始化时序进行复位, 这次每次读取数据前的必须操作.
 *
 * @return bit 为 0 表示初始化成功 , 1表示初始化失败
 */
bit ds18b20_init();

/**
 * @brief	ds18b20的写命令函数, 用于往ds18b20中写入命令,
 * 每个命令由一个字节组成, 通常的表示形式为十六 进制.
 * @param command 命令码 可参考文档
 */
void ds18b20_write_cmd(BYTE command);

/**
 * @brief	从ds18b20读入一位数据, 需要注意的是,
 * 从ds18b20读取数据是从低位开始读取. 这个函数不需要用户 来调用.
 * @return bit
 */
bit ds18b20_read_bit();

/**
 * @brief	从ds18b20读取一个字节的数据, 通常温度数据占两个字节,
 * 第一次调用该函数读取的是低8位数据, 再 一次调用则读取高8位数据.
 * 所以实际上使用该函数时, 需要连续两次调用, 才能读出正确的温度值.
 * @return  BYTE
 */
BYTE ds18b20_read_byte();

/**
 * @brief	用于匹配芯片时序的延时函数, 内部使用单层循环, 一个循环大约 2
 * 个机器周期, 若在12M左右的晶振下, 一个机器周期大概是1us.
 *
 * @param num 延时时间
 */
void ds18b20_delay(WORD num);

/**
 * @brief	计算从ds18b20获取的温度, 并将符号状态、小数、整数分别存储,
 * 返回一个包含该三元组的结构体.
 *
 * @param  valueH	温度值的高8位数据.
 * @param  valueL	温度值的低8位数据.
 * @return TValue	存储温度值的结构体.
 */
TValue ds18b20_calculate_temperature(BYTE valueH, BYTE valueL);

/**
 * @brief	向ds18b20芯片发送convert命令, 命令码 0x44,
 * convert命令用于打开ds18b20的温度转换触发器, 当ds18b20芯片初始化后,
 * 默认的温度值是+85摄氏度, 需要发送convert命令来使芯片触发转换温度值。
 * 		每次转换都会需要使用.
 *
 * @return bit 0 表示正在有温度进行转换, 表示转换完成。
 */
bit ds18b20_convertT();

// --- 非核心 计算用 代码 ---

/**
 * @brief
 *当从寄存器接收到了温度数据，要存入温度结构体来以便显示和后期处理，该函数可以将从寄存器接收的数据
 *			通过事先定义好的转换表，来转换成对应的十进制小数、整数(整数通常被类型隐式转换，无需调用).
 *
 * @param source	传入需要转换数, 单位是一个字节。
 * @param flag		用来确认需要被转换数据的类型, 为 0 表示温度的整数部分，
 *1 表示温度的小数部分.
 * @return 		double
 */
double BinaryToDecimal(BYTE source, bit flag);

/**
 *			底层代码函数，这一部分主要是库内部调用需要，不对外使用。
 *			使用该库文件的时候，无需调用这两个函数。
 */
void ds18b20_write_zero(); // 向ds18b20写一位0.

void ds18b20_write_one(); // 向ds18b20写一位1.
#endif