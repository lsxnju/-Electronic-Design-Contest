//-----------------------------------------------------------------
// TMP275����ͷ�ļ�
// ͷ�ļ���: tmp275.h
// ��    ��: ���ǵ���
// ��ʼ����: 2014-01-28
// �������: 2014-01-28
// �޸�����: 2014-02-16
// ��ǰ�汾: V1.0.1
// ��ʷ�汾:
//   - V1.0: (2014-02-07)TMP275����
//-----------------------------------------------------------------
#ifndef _TMP275_H
#define _TMP275_H
#include <stdio.h>
#include <stdint.h>

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
extern void TMP275_Init(uint8_t device);
extern void Read_Temperature(uint8_t device, float *Temp);
extern void Configuration_TMP275(uint8_t device, uint8_t dat);
extern void Set_Temp_Range(uint8_t device, uint16_t TL, uint16_t TH);

#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

