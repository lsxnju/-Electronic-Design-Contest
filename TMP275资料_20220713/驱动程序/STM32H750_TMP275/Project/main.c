//-----------------------------------------------------------------
// 程序描述:
//     TMP275温度测量实验
// 作    者: 凌智电子
// 开始日期: 2022-06-27
// 完成日期: 2022-06-27
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2022-06-27)	TMP275温度测量实验
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2、4.3寸/2.8寸 TFT液晶屏、TMP275模块
// 说    明: 硬件连接
//						STM32H750VBT6						TMP275模块
//								3V3		<----------->		VCC
//								GND		<----------->		GND
//								PB0		<----------->		ALE
//								PB6		<----------->		SCL
//								PB7		<----------->		SDA
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "system.h"
#include "delay.h"
#include "lcd.h"
#include "led.h"
#include "usart.h"
#include "tmp275.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 主程序
//-----------------------------------------------------------------
int main(void)
{
	uint8_t Tx_Buf[32];
	float temp;
//	CPU_CACHE_Enable();			// 启用CPU缓存
  HAL_Init();          		// 初始化HAL库
	MPU_Memory_Protection();// 设置保护区域
	SystemClock_Config(); 	// 设置系统时钟,400Mhz  
	SysTick_clkconfig(400);	// SysTick参数配置
	LED_Init();           	// 初始化LED 
	uart_init(115200);			// 初始化串口
	LCD_Init();           	// LCD初始化
	TMP275_Init(TMP275_ADDR0);	// 初始化TMP275
	POINT_COLOR=RED; 				// 笔画颜色
	LCD_ShowString(30,10,200,16,16,"STM32H750 Core Board");	
	LCD_ShowString(30,30,200,16,16,"TMP275 Temperature TEST");	
	LCD_ShowString(30,50,200,16,16,"LZ Electronic");
	LCD_ShowString(30,70,200,16,16,"2022-06-27"); 
	POINT_COLOR=BLUE;// 设置字体为蓝色   
	LCD_ShowString(30,110,200,16,16,"Temperature: 0.0000");	// 单位℃
	LCD_ShowString(30,130,200,16,16,"state: Normal");
	delay_ms(250);	
	while(1)
	{
		Read_Temperature(TMP275_ADDR0, &temp);
		sprintf ((char *)Tx_Buf,"Temperature: %6.4f    ", temp);
		LCD_ShowString(30,110,300,16,16,(char *)Tx_Buf);			//单位℃
		printf("Temperature:%6.4f℃\r\n", temp);
		delay_ms(250);
	}
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
