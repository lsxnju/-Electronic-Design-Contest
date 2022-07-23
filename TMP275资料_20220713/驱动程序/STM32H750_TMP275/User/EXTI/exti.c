//-----------------------------------------------------------------
// 程序描述:
// 		 外部中断驱动程序
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)外部中断初始化，中断时执行相应的事情
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2
// 说    明: 通过按键来实现外部中断，每按一次按键就会触发一次中断；
//    				按键K1：关闭或点亮蓝灯
//    				按键K2：关闭或点亮绿灯
//    				按键K3：关闭或点亮红灯
//    				按键K4：关闭所有灯
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "exti.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "lcd.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void EXTI_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: 外部中断初始化
// 入口参数: 无
// 返回参数: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void EXTI_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();   					// 开启GPIOB时钟
	
	GPIO_Initure.Pin=GPIO_PIN_0; 						 	
	GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;   // 下降沿触发
	GPIO_Initure.Pull=GPIO_PULLUP;						// 上拉
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	// 中断线0-PE0
	HAL_NVIC_SetPriority(EXTI0_IRQn,1,1);    	// 抢占优先级为1，子优先级为1
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);          	// 使能中断线0
}

//-----------------------------------------------------------------
// void EXTI0_IRQHandler(void)
//-----------------------------------------------------------------
//
// 函数功能: 中断线0中断服务函数，调用中断处理公用函数
// 入口参数: 无
// 返回参数: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);// 调用中断处理公用函数
}

//-----------------------------------------------------------------
// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//-----------------------------------------------------------------
//
// 函数功能: 中断服务程序中需要做的事情
// 入口参数: uint16_t GPIO_Pin：中断引脚
// 返回参数: 无
// 注意事项: 在HAL库中所有的外部中断服务函数都会调用此函数
//
//-----------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	if(GPIO_Pin == GPIO_PIN_0)
	{
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET)
		{
			GPIO_Initure.Pin=GPIO_PIN_0; 						 	
			GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;   // 下降沿触发
			GPIO_Initure.Pull=GPIO_PULLUP;						// 上拉
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);
			LED_R_OFF;
			LCD_ShowString(30,130,200,16,16,"state: Normal");	//先在固定位置显示小数点  
		}
		else
		{
			GPIO_Initure.Pin=GPIO_PIN_0; 						 	
			GPIO_Initure.Mode=GPIO_MODE_IT_RISING;   	// 上升沿触发
			GPIO_Initure.Pull=GPIO_PULLUP;						// 上拉
			HAL_GPIO_Init(GPIOB,&GPIO_Initure);
			LED_R_ON;
			LCD_ShowString(30,130,200,16,16,"state: ALARM ");	//先在固定位置显示小数点  
		}
	}
}
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
