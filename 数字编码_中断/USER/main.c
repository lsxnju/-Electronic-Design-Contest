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
 ALIENTEK ̽����STM32F407������ʵ��0-1
 Template����ģ��-�½������½�ʹ��-HAL��汾
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


/***ע�⣺�����̺ͽ̳��е��½�����3.3С�ڶ�Ӧ***/
extern u8	TIM5CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ
extern u8	detect_flag;//Ĭ��Ϊ0����Ҫ�����첨ʱ����Ϊ1����
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
	
    HAL_Init();                    	 			//��ʼ��HAL��    
    Stm32_Clock_Init(336,8,2,7); //����ʱ��,168Mhz
	delay_init(168); //��ʼ����ʱ����
	Init_KeyBoard();
	uart_init(115200); //��ʼ��USART
	LED_Init();
	TM1637_Init();	
	
	TIM14_PWM_Init(500-1,84-1);    		//84M/84=1M�ļ���Ƶ�ʣ��Զ���װ��Ϊ500����ôPWMƵ��Ϊ1M/500=2kHZ
	TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1); //��1MHZ��Ƶ�ʼ���
	while(1)
	{
		ROW_H_COL_L();
		key_return_c = ReadKeyBoard();	//���ض�Ӧ�İ������
		if(key_return_c==KEY_1){//�������룬���뷢��
			LED0=!LED0;
			TM1637_clear();
			data = input_int(0, 0);//enter֮��Ż��˳���������ܺ�LCD�϶�����ʾ���ڵ�����
			Output_start();//enter������һ�б���⵽
			while(1){//��Ƭ���ڷ��͵�ʱ���ܸ���������
				Output_FourDigit(data);//
				ROW_H_COL_L();
				key_return_c = ReadKeyBoard();	//���ض�Ӧ�İ������
				if(key_return_c==KEY_2){//��������
					Output_stop();
				}
			}
		}
		if(key_return_c==KEY_3){//����ģʽ
			TIM_SetTIM14Compare1(TIM_GetTIM14Capture1()+1); 
			if(TIM_GetTIM14Capture1()==300)TIM_SetTIM14Compare1(0);        
			if(TIM5CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
			{
				temp=TIM5CH1_CAPTURE_STA&0X3F; 
				temp*=0XFFFFFFFF;		 	    //���ʱ���ܺ�
				temp+=TIM5CH1_CAPTURE_VAL;      //�õ��ܵĸߵ�ƽʱ�䣬��Ƶ84�����Ե�λ��us
				printf("HIGH:%lld us\r\n",temp);//��ӡ�ܵĸߵ�ƽʱ��
				TIM5CH1_CAPTURE_STA=0;          //������һ�β���
			}
		}
	}
}
