//-----------------------------------------------------------------
// ��������:
//     TMP275�¶Ȳ���ʵ��
// ��    ��: ���ǵ���
// ��ʼ����: 2022-06-27
// �������: 2022-06-27
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2022-06-27)	TMP275�¶Ȳ���ʵ��
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2��4.3��/2.8�� TFTҺ������TMP275ģ��
// ˵    ��: Ӳ������
//						STM32H750VBT6						TMP275ģ��
//								3V3		<----------->		VCC
//								GND		<----------->		GND
//								PB0		<----------->		ALE
//								PB6		<----------->		SCL
//								PB7		<----------->		SDA
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "system.h"
#include "delay.h"
#include "lcd.h"
#include "led.h"
#include "usart.h"
#include "tmp275.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ������
//-----------------------------------------------------------------
int main(void)
{
	uint8_t Tx_Buf[32];
	float temp;
//	CPU_CACHE_Enable();			// ����CPU����
  HAL_Init();          		// ��ʼ��HAL��
	MPU_Memory_Protection();// ���ñ�������
	SystemClock_Config(); 	// ����ϵͳʱ��,400Mhz  
	SysTick_clkconfig(400);	// SysTick��������
	LED_Init();           	// ��ʼ��LED 
	uart_init(115200);			// ��ʼ������
	LCD_Init();           	// LCD��ʼ��
	TMP275_Init(TMP275_ADDR0);	// ��ʼ��TMP275
	POINT_COLOR=RED; 				// �ʻ���ɫ
	LCD_ShowString(30,10,200,16,16,"STM32H750 Core Board");	
	LCD_ShowString(30,30,200,16,16,"TMP275 Temperature TEST");	
	LCD_ShowString(30,50,200,16,16,"LZ Electronic");
	LCD_ShowString(30,70,200,16,16,"2022-06-27"); 
	POINT_COLOR=BLUE;// ��������Ϊ��ɫ   
	LCD_ShowString(30,110,200,16,16,"Temperature: 0.0000");	// ��λ��
	LCD_ShowString(30,130,200,16,16,"state: Normal");
	delay_ms(250);	
	while(1)
	{
		Read_Temperature(TMP275_ADDR0, &temp);
		sprintf ((char *)Tx_Buf,"Temperature: %6.4f    ", temp);
		LCD_ShowString(30,110,300,16,16,(char *)Tx_Buf);			//��λ��
		printf("Temperature:%6.4f��\r\n", temp);
		delay_ms(250);
	}
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
