//-----------------------------------------------------------------
// 程序描述:
//    TMP275温度测量实验
// 作    者: 凌智电子
// 开始日期: 2022-06-27
// 完成日期: 2022-06-27
//  - V1.0: (2022-06-27)	TMP275温度测量实验
// 调试工具: 正点原子精英版STM32F103开发板、LZE_ST LINK2、2.8寸液晶、TMP275模块
// 说    明: 硬件连接
//							 开发板							  TMP275模块
//								3V3		<----------->		VCC
//								GND		<----------->		GND
//								PA5		<----------->		ALE
//								PA6		<----------->		SCL
//								PA7		<----------->		SDA
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
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
// 主程序
//-----------------------------------------------------------------

int main(void)
{	 
	uint8_t Tx_Buf[32];
	float temp;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();
	TMP275_Init(TMP275_ADDR0);// 初始化TMP275
  EXTInterrupt_Init();      // 外中断初始化
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

