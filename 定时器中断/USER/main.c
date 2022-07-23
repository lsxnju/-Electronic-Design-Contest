#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "adc.h"
#include "timer.h"
#include "arm_math.h"  
#include "KeyBoard.h"
#include "relay.h"
#include "main.h"
#include "math.h"

/************************************************
 ALIENTEK 探索者STM32F407开发板 实验47_2
 DSP FFT实验-HAL库函数版
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

float fft_inputbuf[FFT_LENGTH*2];	//FFT输入数组，乘2是分别表示实部和虚部,但为什么没有溢出
float fft_outputbuf[FFT_LENGTH];	//FFT输出数组，存储模值
u8 fft_flag=0;//FFT开始标志，1为开始，0为未开始
u16 adc_times=0;
u16 timeout = 0;//初始化为0
u16 t=0;
int main(void)
{
    arm_cfft_radix2_instance_f32 scfft;
	u16 i;
	char key_return_c=0;
	float THD[6];
		
    HAL_Init();                   	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	
	uart_init(115200);             	//初始化USART
	LED_Init();						//初始化LED	
	KEY_Init();						//初始化按键
 	LCD_Init();           			//初始化LCD FSMC接口
	Init_KeyBoard();				//初始化键盘
	RELAY_Init();
	MY_ADC_Init();                  //初始化ADC1通道5，一定要先初始化ADC，再初始化TIM，否则定时器中断里会卡住
	//TIM3_Init(8-1,82-1); 			//128KHZ对应时钟，两参数相乘 = 656.25为最佳，近似取656
	//TIM3_Init(5-1,84-1);			//这个值是极限值
	TIM3_Init(4-1,82-1);			//256KHZ对应时钟
	POINT_COLOR = WHITE;
	BACK_COLOR=BLACK;
	LCD_ShowString(10,10,200,32,32,"KEY1 Run FFT");//显示提示信息  
	arm_cfft_radix2_init_f32(&scfft,FFT_LENGTH,0,1);//初始化scfft结构体，设定FFT相关参数
    while(1)
	{
		ROW_H_COL_L();
		key_return_c = ReadKeyBoard();	//返回对应的按键编号
		if(key_return_c==KEY_1)
		{
			adc_times=0;
			fft_flag=1;
			delay_ms(2);//等待采样完成
			arm_cfft_radix2_f32(&scfft,fft_inputbuf);	//FFT计算（基2）
			arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	//把运算结果复数求模得幅值 
			LCD_ShowString(100,100,200,32,32,"FFT_output:");//显示提示信息 
			for(i=2;i<11;i+=2){//打印前5个FFT值
				THD[i/2] = fft_outputbuf[i]/4096 * 3.3;//转换为电压值
				//LCD_ShowxFloat(100, 132+32*i/2, fft_outputbuf[i], 2, 9, 32, 0);
				LCD_ShowxFloat(100, 132+32*i/2, THD[i/2], 2, 9, 32, 0);
			}
			THD[0]=0;
			for(i=2;i<6;i++){
				THD[0]+=THD[i]*THD[i];//pow(THD[i],2);
			}
			THD[0]=sqrt(THD[0])/THD[1];
			LCD_ShowString(100,132+32*7,16*4,32,32,"THD:");//显示提示信息 
			LCD_ShowxFloat(100+16*5, 132+32*7, THD[0], 2, 9, 32, 0);
		}
		else if(key_return_c==KEY_2){//继电器0
			RELAY0=!RELAY0;
		}
		else if(key_return_c==KEY_3){//继电器1
			RELAY1=!RELAY1;
		}
		else if(key_return_c==KEY_4){//继电器2
			RELAY2=!RELAY2;
		}
		else if(key_return_c==KEY_5){//继电器3
			RELAY3=!RELAY3;
		}
		else if(key_return_c==KEY_6){//继电器4
			RELAY4=!RELAY4;
		}
		else delay_ms(10);
		t++;
		if((t%50)==0){
			LED1=!LED1;//1s闪烁一次
			t=0;
		}			
	} 
}

