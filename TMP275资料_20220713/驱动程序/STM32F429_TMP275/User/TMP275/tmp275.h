//-----------------------------------------------------------------
// 程序描述:
//     TMP275驱动程序头文件
// 作    者: 凌智电子
// 开始日期: 2022-06-27
// 完成日期: 2022-06-27
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2022-06-27)TMP275初始化及应用
// 调试工具: 凌智STM32F429+CycloneIV电子系统设计开发板、LZE_ST_LINK2、TMP275模块
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef _TMP275_H
#define _TMP275_H
#include "stm32f429_winner.h"

//-----------------------------------------------------------------
// 宏定义
//-----------------------------------------------------------------
#define TMP275_ADDR0			0x90		// A2:0  A1:0  A0:0
#define TMP275_ADDR1			0x92		// A2:0  A1:0  A0:1
#define TMP275_ADDR2			0x94		// A2:0  A1:1  A0:0
#define TMP275_ADDR3			0x96		// A2:0  A1:1  A0:1
#define TMP275_ADDR4			0x98		// A2:1  A1:0  A0:0
#define TMP275_ADDR5			0x9A		// A2:1  A1:0  A0:1
#define TMP275_ADDR6			0x9C		// A2:1  A1:1  A0:0
#define TMP275_ADDR7			0x9E		// A2:1  A1:1  A0:1

#define TEMP_ADDR					0x00		// 温度寄存器，只读
#define CONFIG_ADDR				0x01		// 配置寄存器，读写
#define TLOW_ADDR					0x02		// TLOW寄存器，读写
#define THIGH_ADDR				0x03		// THIGH寄存器，读写

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void MX_I2C1_Init(void);
extern void TMP275_Init(uint8_t device);
extern void Read_Temperature(uint8_t device, float *Temp);
extern void Configuration_TMP275(uint8_t device, uint8_t dat);
extern void Set_Temp_Range(uint8_t device, uint16_t TL, uint16_t TH);

#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

