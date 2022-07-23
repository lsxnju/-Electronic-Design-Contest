#include "DigitOut.h"
#include "delay.h"

#define OutFreq 16 //16Hz
#define DelayTime 1000/OutFreq//ÿ�ε�ʱ��
#define OUT_MSB//��λ�ȴ��������ɸ�λ����λ����ÿ�����ֵĶ�����Ҳ���ߵ��ʹ�

void Output_Init()
{
	GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOFʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_14; //PB14
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

void Output_OneDigit(u8 digit)
{
	u8 a[4]={0};
	u8 i;
	if(digit<16){
		i=0;//��λ��Ŷ����Ƹ�λ
		//10����ת2����
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
		Output_OneDigit((u8)(data / 1000));//ǧλ
		Output_OneDigit((u8)(data / 100 % 10));//��λ
		Output_OneDigit((u8)(data % 100 / 10));//ʮλ
		Output_OneDigit((u8)(data % 10));	//��λ
	#else
		Output_OneDigit((u8)(data % 10));	//��λ
		Output_OneDigit((u8)(data % 100 / 10));//ʮλ
		Output_OneDigit((u8)(data / 100 % 10));//��λ
		Output_OneDigit((u8)(data / 1000));//ǧλ
	#endif
}

void Output_start()
{
	Output_Init();//��ʼ���������
	Output_OneDigit(14);//1110��ʼ����
}

void Output_stop()
{
	Output_OneDigit(15);//1111��������
}
