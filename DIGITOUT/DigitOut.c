#include "DigitOut.h"
#include "delay.h"

#define OutFreq 16 //16Hz
#define DelayTime 1000/OutFreq//每段的时间
#define OUT_MSB//高位先传，数字由高位到低位传，每个数字的二进制也按高到低传

void Output_Init()
{
	GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOF时钟
	
    GPIO_Initure.Pin=GPIO_PIN_14; //PB14
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

void Output_OneDigit(u8 digit)
{
	u8 a[4]={0};
	u8 i;
	if(digit<16){
		i=0;//低位存放二进制高位
		//10进制转2进制
		while(digit!=0){
			a[i] = digit % 2;
			digit/=2;
			i++;
		}
		#ifdef OUT_MSB
		for(i=0;i<4;i++){
			DigitOutput = a[i];
			delay_us_f(DelayTime);//62.5us
		}
		#else
		for(i=3;i>=0;i--){
			DigitOutput = a[i];
			delay_us_f(DelayTime);//62.5us
		}
		#endif
	}
	else{
		
	}
}

void Output_FourDigit(u16 data)
{
	#ifdef OUT_MSB
		Output_OneDigit((u8)(data / 1000));//千位
		Output_OneDigit((u8)(data / 100 % 10));//百位
		Output_OneDigit((u8)(data % 100 / 10));//十位
		Output_OneDigit((u8)(data % 10));	//个位
	#else
		Output_OneDigit((u8)(data % 10));	//个位
		Output_OneDigit((u8)(data % 100 / 10));//十位
		Output_OneDigit((u8)(data / 100 % 10));//百位
		Output_OneDigit((u8)(data / 1000));//千位
	#endif
}

void Output_start()
{
	Output_Init();//初始化输出引脚
	Output_OneDigit(14);//1110开始发送
}

void Output_stop()
{
	Output_OneDigit(15);//1111结束发送
}
