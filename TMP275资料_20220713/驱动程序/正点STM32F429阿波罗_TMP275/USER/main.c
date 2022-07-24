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
// 调试工具: 正点原子STM32F429阿波罗、LZE_ST_LINK2、TMP275模块
// 说    明: 硬件连接
//						 STM32F429							TMP275模块
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
// 主程序
//-----------------------------------------------------------------
int main(void)
{
	uint8_t Tx_Buf[32];
	float temp;
	Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz
	delay_init(180);                //初始化延时函数
	uart_init(115200);              //初始化USART
	LED_Init();                     //初始化LED 
	KEY_Init();                     //初始化按键
	SDRAM_Init();                   //SDRAM初始化
	LCD_Init();                     //LCD初始化
	TMP275_Init(TMP275_ADDR0);			// 初始化TMP275
  EXTI_Init();      							// 外中断初始化
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
