//-----------------------------------------------------------------
// ��������:
//     TMP275��������
// ��    ��: ���ǵ���
// ��ʼ����: 2014-01-28
// �������: 2014-01-28
// �޸�����: 2014-02-16
// ��ǰ�汾: V1.0.1
// ��ʷ�汾:
//  - V1.0: (2014-02-07)TMP275����
// - V1.0.1:(2014-02-16)ͷ�ļ��в���������ͷ�ļ�
// ���Թ���: ����STM32���Ŀ����塢LZE_ST_LINK2��TMP275ģ��
// ˵    ��:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "tmp275.h"
#include "I2C.h"

//-----------------------------------------------------------------
// void TMP275_Init(void)
//-----------------------------------------------------------------
// 
// ��������: TMP275��ʼ��
// ��ڲ���: device���豸��ַ
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void TMP275_Init(uint8_t device)
{
	IIC_Init();
	Configuration_TMP275(device, 0x60);			// ������ȡ��12λ�ֱ��ʣ�����1������;������ͼ��Ծ������Ƚ�ģʽ������ת��
	Set_Temp_Range(device, 0x020F, 0x0210);	// �����¶Ⱦ�����33�棬��33/0.0625=544=0x0220��

}

//-----------------------------------------------------------------
// void Read_Temperature(uint8_t device, float *Temp)
//-----------------------------------------------------------------
// 
// ��������: �����¶�
// ��ڲ���: device���豸��ַ
//					 Temp�������õ����¶�
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Read_Temperature(uint8_t device, float *Temp)
{
	uint8_t recv_data[2] = {0};
	uint16_t rd_data;

	IIC_Start(); 
	IIC_Send_Byte(device);
	IIC_Wait_Ack();
	IIC_Send_Byte(TEMP_ADDR);
	IIC_Wait_Ack();
	IIC_Start(); 
	IIC_Send_Byte(device|0x01);
	IIC_Wait_Ack();
	recv_data[0]=IIC_Read_Byte(1);
	recv_data[1]=IIC_Read_Byte(1);
	IIC_Stop();
	rd_data = ((uint16_t)recv_data[0]<<4) | ((uint16_t)recv_data[1]>>4);
	if(rd_data & 0x0800)
		*Temp = (float)(rd_data ^ 0x0FFF) * -0.0625;
	else
		*Temp = (float)(rd_data * 0.0625);
}

//-----------------------------------------------------------------
// void e53_ia_init(void)
//-----------------------------------------------------------------
// 
// ��������: TMP275����
// ��ڲ���: device���豸��ַ
//					 dat�����ò���
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Configuration_TMP275(uint8_t device, uint8_t dat)
{
	IIC_Start(); 
	IIC_Send_Byte(device);
	IIC_Wait_Ack();
	IIC_Send_Byte(CONFIG_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte(dat);
	IIC_Wait_Ack();
	IIC_Stop();
}

//-----------------------------------------------------------------
// void Set_Temp_Range(uint8_t device, uint16_t TL, uint16_t TH)
//-----------------------------------------------------------------
// 
// ��������: �����¶Ⱦ�����Χ
// ��ڲ���: device���豸��ַ
//					 TL������¶ȵ�����������
//					 TH������¶ȵ�����������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Set_Temp_Range(uint8_t device, uint16_t TL, uint16_t TH)
{
	IIC_Start(); 
	IIC_Send_Byte(device);
	IIC_Wait_Ack();
	IIC_Send_Byte(TLOW_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)((TL>>4)&0x00FF));
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)((TL<<4)&0x00FF));
	IIC_Wait_Ack();
	IIC_Stop();
	
	IIC_Start(); 
	IIC_Send_Byte(device);
	IIC_Wait_Ack();
	IIC_Send_Byte(THIGH_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)((TH>>4)&0x00FF));
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)((TH<<4)&0x00FF));
	IIC_Wait_Ack();
	IIC_Stop();
}
