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
 ALIENTEK ̽����STM32F407������ ʵ��47_2
 DSP FFTʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

float fft_inputbuf[FFT_LENGTH*2];	//FFT�������飬��2�Ƿֱ��ʾʵ�����鲿,��Ϊʲôû�����
float fft_outputbuf[FFT_LENGTH];	//FFT������飬�洢ģֵ
u8 fft_flag=0;//FFT��ʼ��־��1Ϊ��ʼ��0Ϊδ��ʼ
u16 adc_times=0;
u16 timeout = 0;//��ʼ��Ϊ0
u16 t=0;
int main(void)
{
    arm_cfft_radix2_instance_f32 scfft;
	u16 i;
	char key_return_c=0;
	float THD[6];
		
    HAL_Init();                   	//��ʼ��HAL��    
    Stm32_Clock_Init(336,8,2,7);  	//����ʱ��,168Mhz
	delay_init(168);               	//��ʼ����ʱ����
	
	uart_init(115200);             	//��ʼ��USART
	LED_Init();						//��ʼ��LED	
	KEY_Init();						//��ʼ������
 	LCD_Init();           			//��ʼ��LCD FSMC�ӿ�
	Init_KeyBoard();				//��ʼ������
	RELAY_Init();
	MY_ADC_Init();                  //��ʼ��ADC1ͨ��5��һ��Ҫ�ȳ�ʼ��ADC���ٳ�ʼ��TIM������ʱ���ж���Ῠס
	//TIM3_Init(8-1,82-1); 			//128KHZ��Ӧʱ�ӣ���������� = 656.25Ϊ��ѣ�����ȡ656
	//TIM3_Init(5-1,84-1);			//���ֵ�Ǽ���ֵ
	TIM3_Init(4-1,82-1);			//256KHZ��Ӧʱ��
	POINT_COLOR = WHITE;
	BACK_COLOR=BLACK;
	LCD_ShowString(10,10,200,32,32,"KEY1 Run FFT");//��ʾ��ʾ��Ϣ  
	arm_cfft_radix2_init_f32(&scfft,FFT_LENGTH,0,1);//��ʼ��scfft�ṹ�壬�趨FFT��ز���
    while(1)
	{
		ROW_H_COL_L();
		key_return_c = ReadKeyBoard();	//���ض�Ӧ�İ������
		if(key_return_c==KEY_1)
		{
			adc_times=0;
			fft_flag=1;
			delay_ms(2);//�ȴ��������
			arm_cfft_radix2_f32(&scfft,fft_inputbuf);	//FFT���㣨��2��
			arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);	//��������������ģ�÷�ֵ 
			LCD_ShowString(100,100,200,32,32,"FFT_output:");//��ʾ��ʾ��Ϣ 
			for(i=2;i<11;i+=2){//��ӡǰ5��FFTֵ
				THD[i/2] = fft_outputbuf[i]/4096 * 3.3;//ת��Ϊ��ѹֵ
				//LCD_ShowxFloat(100, 132+32*i/2, fft_outputbuf[i], 2, 9, 32, 0);
				LCD_ShowxFloat(100, 132+32*i/2, THD[i/2], 2, 9, 32, 0);
			}
			THD[0]=0;
			for(i=2;i<6;i++){
				THD[0]+=THD[i]*THD[i];//pow(THD[i],2);
			}
			THD[0]=sqrt(THD[0])/THD[1];
			LCD_ShowString(100,132+32*7,16*4,32,32,"THD:");//��ʾ��ʾ��Ϣ 
			LCD_ShowxFloat(100+16*5, 132+32*7, THD[0], 2, 9, 32, 0);
		}
		else if(key_return_c==KEY_2){//�̵���0
			RELAY0=!RELAY0;
		}
		else if(key_return_c==KEY_3){//�̵���1
			RELAY1=!RELAY1;
		}
		else if(key_return_c==KEY_4){//�̵���2
			RELAY2=!RELAY2;
		}
		else if(key_return_c==KEY_5){//�̵���3
			RELAY3=!RELAY3;
		}
		else if(key_return_c==KEY_6){//�̵���4
			RELAY4=!RELAY4;
		}
		else delay_ms(10);
		t++;
		if((t%50)==0){
			LED1=!LED1;//1s��˸һ��
			t=0;
		}			
	} 
}

