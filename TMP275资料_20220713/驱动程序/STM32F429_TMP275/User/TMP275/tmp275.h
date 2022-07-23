//-----------------------------------------------------------------
// ��������:
//     TMP275��������ͷ�ļ�
// ��    ��: ���ǵ���
// ��ʼ����: 2022-06-27
// �������: 2022-06-27
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2022-06-27)TMP275��ʼ����Ӧ��
// ���Թ���: ����STM32F429+CycloneIV����ϵͳ��ƿ����塢LZE_ST_LINK2��TMP275ģ��
// ˵    ��: 
//    
//-----------------------------------------------------------------
#ifndef _TMP275_H
#define _TMP275_H
#include "stm32f429_winner.h"

//-----------------------------------------------------------------
// �궨��
//-----------------------------------------------------------------
#define TMP275_ADDR0			0x90		// A2:0  A1:0  A0:0
#define TMP275_ADDR1			0x92		// A2:0  A1:0  A0:1
#define TMP275_ADDR2			0x94		// A2:0  A1:1  A0:0
#define TMP275_ADDR3			0x96		// A2:0  A1:1  A0:1
#define TMP275_ADDR4			0x98		// A2:1  A1:0  A0:0
#define TMP275_ADDR5			0x9A		// A2:1  A1:0  A0:1
#define TMP275_ADDR6			0x9C		// A2:1  A1:1  A0:0
#define TMP275_ADDR7			0x9E		// A2:1  A1:1  A0:1

#define TEMP_ADDR					0x00		// �¶ȼĴ�����ֻ��
#define CONFIG_ADDR				0x01		// ���üĴ�������д
#define TLOW_ADDR					0x02		// TLOW�Ĵ�������д
#define THIGH_ADDR				0x03		// THIGH�Ĵ�������д

//-----------------------------------------------------------------
// �ⲿ��������
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

