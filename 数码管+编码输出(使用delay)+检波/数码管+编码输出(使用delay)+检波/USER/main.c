#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "KeyBoard.h"
#include "TM1637.h"
#include "led.h"
#include "DigitOut.h"
#include "timer.h"
#include "lcd.h"
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
//extern u8	TIM5CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值
extern u8	detect_flag;//默认为0，需要开启检波时设置为1即可
extern u8 	recv_data[24];
extern u8 	finish_flag;//完成一个循环的接收
//extern u8 	recv_sum;
void Delay(__IO uint32_t nCount);

void Delay(__IO uint32_t nCount)
{
  while(nCount--){}
}

int main(void)
{
    char key_return_c;
	u16  data=0;
	u16	 data_new=0;
	
    HAL_Init();                    	 			//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7); //设置时钟,168Mhz
	delay_init(168); //初始化延时函数
	Init_KeyBoard();
	uart_init(115200); //初始化USART
	LED_Init();
	TM1637_Init();	
	
	TIM14_PWM_Init(500-1,84-1);    		//84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //以1MHZ的频率计数，u32最大0XFFFFFFFF=4,294,967,295，也即溢出时间接近12万小时，不可能溢出
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
			LCD_ShowxString(10, 30, 16*12, 32, 32, "Accepting...!", 0);		
			data = 0;
			data_new = 0;
			detect_flag=1;
			TM1637_clear();
			if(finish_flag){//完成一次传输
				if(Confirm_Bin(recv_data, 4, "1110")){
					data_new = BinToDec(recv_data+4, 16);
					if(data != data_new){
						u8* temp = BinToHex(recv_data+4, 16);
						u8 i;
						TM1637_display(data_new);//数码管更新数字
						for(i=0;i<4;i++){
							if(temp[i]==15){
								detect_flag = 0;//终止接收
								delay_ms(5000);//5s后熄灭数码管
								TM1637_clear();
							}
						}
						data = data_new;
					}
				}
				else{
					LCD_ShowxString(10, 30, 16*12, 32, 32, "Detect Error!", 0);//报错
				}
				
			}
		}
		if(key_return_c==KEY_4){//停止接收   
			detect_flag=0;
		}
	}
}
