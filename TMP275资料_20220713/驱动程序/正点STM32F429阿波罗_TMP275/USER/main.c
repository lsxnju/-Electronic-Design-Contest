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
// ���Թ���: ����ԭ��STM32F429�����ޡ�LZE_ST_LINK2��TMP275ģ��
// ˵    ��: Ӳ������
//						 STM32F429							TMP275ģ��
//								3V3		<----------->		VCC
//								GND		<----------->		GND
//								PB8		<----------->		ALE
//								PB6		<----------->		SCL
//								PB7		<----------->		SDA
//    
//-----------------------------------------------------------------
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "exti.h"
#include "tmp275.h"

//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ������
//-----------------------------------------------------------------
int main(void)
{
	uint8_t Tx_Buf[32];
	float temp;
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
	delay_init(180);                //��ʼ����ʱ����
	uart_init(115200);              //��ʼ��USART
	LED_Init();                     //��ʼ��LED 
	KEY_Init();                     //��ʼ������
	SDRAM_Init();                   //SDRAM��ʼ��
	LCD_Init();                     //LCD��ʼ��
	TMP275_Init(TMP275_ADDR0);			// ��ʼ��TMP275
  EXTI_Init();      							// ���жϳ�ʼ��
	printf("AAAA");
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(30,0,240,24,24,(uint8_t *)"Elite STM32F4 ^_^"); 
	LCD_ShowString(30,30,240,24,24,(uint8_t *)"TMP275  TEST"); 
	LCD_ShowString(30,60,240,24,24,(uint8_t *)"LZ Electronic"); 
	POINT_COLOR=BLUE;
	LCD_ShowString(30,100,240,16,16,(uint8_t *)"Temperature: 0.0000"); 
	LCD_ShowString(30,120,240,16,16,(uint8_t *)"state: Normal");
  while(1)
  {
    Read_Temperature(TMP275_ADDR0, &temp);
		sprintf ((char *)Tx_Buf,"Temperature: %6.4f    ", temp);
		LCD_ShowString(30,100,240,16,16, (uint8_t *)Tx_Buf);
		delay_ms(250);	
	}
}
