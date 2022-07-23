#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "KeyBoard.h"
#include "TM1637.h"
#include "led.h"
#include "DigitOut.h"
#include "timer.h"
#include "main.h"

/************************************************
 ALIENTEK 探索者STM32F407开发板实验0-1
 Template工程模板-新建工程章节使用-HAL库版本
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


/***注意：本工程和教程中的新建工程3.3小节对应***/
extern u8	TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值
extern u8	detect_flag;//默认为0，需要开启检波时设置为1即可
void Delay(__IO uint32_t nCount);

void Delay(__IO uint32_t nCount)
{
  while(nCount--){}
}

int main(void)
{
	long long temp=0; 
    char key_return_c;
	u16  data=0;
	
    HAL_Init();                    	 			//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7); //设置时钟,168Mhz
	delay_init(168); //初始化延时函数
	Init_KeyBoard();
	uart_init(115200); //初始化USART
	LED_Init();
	TM1637_Init();	
	
	TIM14_PWM_Init(500-1,84-1);    		//84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以1MHZ的频率计数
	while(1)
	{
		ROW_H_COL_L();
		key_return_c = ReadKeyBoard();	//返回对应的按键编号
		if(key_return_c==KEY_1){//数字输入，编码发送
			LED0=!LED0;
			TM1637_clear();
			data = input_int(0, 0);//enter之后才会退出，在数码管和LCD上都会显示现在的数字
			Output_start();//enter键在上一行被检测到
			while(1){//单片机在发送的时候不能干其他事情
				Output_FourDigit(data);//
				ROW_H_COL_L();
				key_return_c = ReadKeyBoard();	//返回对应的按键编号
				if(key_return_c==KEY_2){//结束发送
					Output_stop();
				}
			}
		}
		if(key_return_c==KEY_3){//接收模式
			TIM_SetTIM14Compare1(TIM_GetTIM14Capture1()+1); 
			if(TIM_GetTIM14Capture1()==300)TIM_SetTIM14Compare1(0);        
			if(TIM5CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
			{
				temp=TIM5CH1_CAPTURE_STA&0X3F; 
				temp*=0XFFFFFFFF;		 	    //溢出时间总和
				temp+=TIM5CH1_CAPTURE_VAL;      //得到总的高电平时间，分频84，所以单位是us
				printf("HIGH:%lld us\r\n",temp);//打印总的高点平时间
				TIM5CH1_CAPTURE_STA=0;          //开启下一次捕获
			}
		}
	}
}
