//-----------------------------------------------------------------
// ��������:
//    TMP275�¶Ȳ���ʵ��
// ��    ��: ���ǵ���
// ��ʼ����: 2022-06-27
// �������: 2022-06-27
//  - V1.0: (2022-06-27)	TMP275�¶Ȳ���ʵ��
// ���Թ���: ����STM32���Ŀ����塢LZE_ST LINK2��2.8��Һ����TMP275ģ��
// ˵    ��: Ӳ������
//							 ������							  TMP275ģ��
//								3V3		<----------->		VCC
//								GND		<----------->		GND
//								PB0		<----------->		ALE
//								PB6		<----------->		SCL
//								PB7		<----------->		SDA
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "stm32f10x.h"
#include "Delay.h"
#include "EXTInterrupt.h"
#include "LED.h"
#include "lze_lcd.h"
#include "tmp275.h"
#include "PeripheralInit.h"

//-----------------------------------------------------------------
// ������
//-----------------------------------------------------------------
int main(void)
{
	uint8_t Tx_Buf[32];
	float temp;
  PeripheralInit();
  LCD_Clear(LCD_COLOR_WHITE);

	LCD_WriteString(30, 10, LCD_COLOR_RED, LCD_COLOR_WHITE, (uint8_t *)"STM32F103 Core Board");
	LCD_WriteString(30, 30, LCD_COLOR_RED, LCD_COLOR_WHITE, (uint8_t *)"TMP275 Temperature TEST");
	LCD_WriteString(30, 50, LCD_COLOR_RED, LCD_COLOR_WHITE, (uint8_t *)"LZ Electronic");
	LCD_WriteString(30, 70, LCD_COLOR_RED, LCD_COLOR_WHITE, (uint8_t *)"2022-06-27");
	LCD_WriteString(30,110, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)"Temperature: 0.0000");
	LCD_WriteString(30,130, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)"state: Normal");
	Delay_1ms(250);	
  while (1)
  {
		Read_Temperature(TMP275_ADDR0, &temp);
		sprintf ((char *)Tx_Buf,"Temperature: %6.4f    ", temp);
		LCD_WriteString(30,110, LCD_COLOR_BLUE, LCD_COLOR_WHITE, (uint8_t *)Tx_Buf);
		Delay_1ms(250);
  }
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
