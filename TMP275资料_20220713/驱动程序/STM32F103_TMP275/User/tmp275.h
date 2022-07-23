//-----------------------------------------------------------------
// TMP275驱动头文件
// 头文件名: tmp275.h
// 作    者: 凌智电子
// 开始日期: 2014-01-28
// 完成日期: 2014-01-28
// 修改日期: 2014-02-16
// 当前版本: V1.0.1
// 历史版本:
//   - V1.0: (2014-02-07)TMP275配置
//-----------------------------------------------------------------
#ifndef _TMP275_H
#define _TMP275_H
#include <stdio.h>
#include <stdint.h>

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
extern void TMP275_Init(uint8_t device);
extern void Read_Temperature(uint8_t device, float *Temp);
extern void Configuration_TMP275(uint8_t device, uint8_t dat);
extern void Set_Temp_Range(uint8_t device, uint16_t TL, uint16_t TH);

#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

