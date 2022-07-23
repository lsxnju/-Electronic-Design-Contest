//-----------------------------------------------------------------
// ��������:
//    TMP275�¶Ȳ���ʵ��
// ��    ��: ���ǵ���
// ��ʼ����: 2022-06-27
// �������: 2022-06-27
//  - V1.0: (2022-06-27)	TMP275�¶Ȳ���ʵ��
// ���Թ���: ����ԭ�Ӿ�Ӣ��STM32F103�����塢LZE_ST LINK2��2.8��Һ����TMP275ģ��
// ˵    ��: Ӳ������
//							 ������							  TMP275ģ��
//								3V3		<----------->		VCC
//								GND		<----------->		GND
//								PA5		<----------->		ALE
//								PA6		<----------->		SCL
//								PA7		<----------->		SDA
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "tmp275.h"
#include "EXTInterrupt.h"

//-----------------------------------------------------------------
// ������
//-----------------------------------------------------------------

int main(void)
{	 
	uint8_t Tx_Buf[32];
	float temp;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	TMP275_Init(TMP275_ADDR0);// ��ʼ��TMP275
  EXTInterrupt_Init();      // ���жϳ�ʼ��
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	LCD_ShowString(30,0,240,24,24,(uint8_t *)"Elite STM32F1 ^_^"); 
	LCD_ShowString(30,30,240,24,24,(uint8_t *)"TMP275  TEST"); 
	LCD_ShowString(30,60,240,24,24,(uint8_t *)"LZ Electronic"); 
	POINT_COLOR=BLUE;
	LCD_ShowString(30,100,240,16,16,(uint8_t *)"Temperature: 0.0000"); 
	LCD_ShowString(30,120,240,16,16,(uint8_t *)"state: Normal");

  while(1) 
	{		 
		Read_Temperature(TMP275_ADDR0, &temp);
		sprintf ((char *)Tx_Buf,"Temperature: %6.4f    ", temp);
		LCD_ShowString(30,100, 200, 16, 16, (uint8_t *)Tx_Buf);
		delay_ms(250);	   		 
	} 
}

