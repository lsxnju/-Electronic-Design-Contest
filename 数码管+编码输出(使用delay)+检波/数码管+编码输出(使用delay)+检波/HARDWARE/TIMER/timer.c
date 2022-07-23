#include "timer.h"
#include "led.h"
#include "DigitOut.h"
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ���ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/4/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
////////////////////////////////////////////////////////////////////////////////// 	
//�޸�˵��
//V1.1 20170407
//����TIM14_PWM_Init����,����PWM���
//V1.2 20170408
//����TIM5_CH1_Cap_Init����,�������벶��
////////////////////////////////////////////////////////////////////////////////// 

TIM_HandleTypeDef TIM3_Handler;      	//��ʱ��3��� 
TIM_HandleTypeDef TIM4_Handler;      	//��ʱ��4���
TIM_HandleTypeDef TIM14_Handler;      	//��ʱ��14��� 
TIM_OC_InitTypeDef TIM14_CH1Handler;	//��ʱ��14ͨ��1���

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM4_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                          //ͨ�ö�ʱ��4
    TIM4_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM4_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM4_Handler);//����Ὺ��
    
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}
//TIM14 PWM���ֳ�ʼ�� 
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void TIM14_PWM_Init(u16 arr,u16 psc)
{  
    TIM14_Handler.Instance=TIM14;          	//��ʱ��14
    TIM14_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM14_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM14_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM14_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM14_Handler);       //��ʼ��PWM
    
    TIM14_CH1Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM14_CH1Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM14_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM14_Handler,&TIM14_CH1Handler,TIM_CHANNEL_1);//����TIM14ͨ��1
	
    HAL_TIM_PWM_Start(&TIM14_Handler,TIM_CHANNEL_1);//����PWMͨ��1
}


//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	}
	else if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //ʹ��TIM4ʱ��
		HAL_NVIC_SetPriority(TIM4_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //����ITM4�ж�   
	}
}

//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM14_CLK_ENABLE();			//ʹ�ܶ�ʱ��14
	__HAL_RCC_GPIOF_CLK_ENABLE();			//����GPIOFʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_9;           	//PF9
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	GPIO_Initure.Alternate= GPIO_AF9_TIM14;	//PF9����ΪTIM14_CH1
	HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}

//����TIMͨ��4��ռ�ձ�
//compare:�Ƚ�ֵ
void TIM_SetTIM14Compare1(u32 compare)
{
	TIM14->CCR1=compare; 
}

//��ȡTIM����/�ȽϼĴ���ֵ
u32 TIM_GetTIM14Capture1(void)
{
	return  HAL_TIM_ReadCapturedValue(&TIM14_Handler,TIM_CHANNEL_1);
}


//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}


////�ص���������ʱ���жϷ���������
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if(htim==(&TIM3_Handler))
//    {
//        LED1=!LED1;        //LED1��ת
//    }
//}


/***************************************************************************
****************************************************************************
  ���������벶�����Դ��ʵ����غ���Դ��
****************************************************************************
****************************************************************************/

TIM_HandleTypeDef TIM5_Handler;         //��ʱ��5���

//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)u32Ҳ��32λ�����ƣ�Ҳ��8λ16���ƣ����ȡ0XFFFF FFFF
//psc��ʱ��Ԥ��Ƶ��
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{  
    TIM_IC_InitTypeDef TIM5_CH1Config;  
    
    TIM5_Handler.Instance=TIM5;                          	//ͨ�ö�ʱ��5
    TIM5_Handler.Init.Prescaler=psc;                     	//��Ƶϵ��
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    	//���ϼ�����
    TIM5_Handler.Init.Period=arr;                        	//�Զ�װ��ֵ
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;	//ʱ�ӷ�Ƶ����
    HAL_TIM_IC_Init(&TIM5_Handler);							//��ʼ�����벶��ʱ������
    
    TIM5_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    	//�����ز���
    TIM5_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;	//ӳ�䵽TI1��
    TIM5_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          	//���������Ƶ������Ƶ
    TIM5_CH1Config.ICFilter=0;                          	//���������˲��������˲�
    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CH1Config,TIM_CHANNEL_1);//����TIM5ͨ��1
	
    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_1);   	//����TIM5�Ĳ���ͨ��1�����ҿ��������ж�
    __HAL_TIM_ENABLE_IT(&TIM5_Handler,TIM_IT_UPDATE);   	//ʹ�ܸ����ж�
}


//��ʱ��5�ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_IC_Init()����
//htim:��ʱ��5���
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM5_CLK_ENABLE();            //ʹ��TIM5ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //�����������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    GPIO_Initure.Alternate=GPIO_AF2_TIM5;   //PA0����ΪTIM5ͨ��1
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

    HAL_NVIC_SetPriority(TIM5_IRQn,2,0);    //�����ж����ȼ�����ռ���ȼ�2�������ȼ�0
    HAL_NVIC_EnableIRQ(TIM5_IRQn);          //����ITM5�ж�ͨ��  
}


//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
//u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬��8λ�����ƣ���0000 0000��1111 1111��  				
u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
u32	TIM4_VAL;				//TIM4�ļ���ֵ
u8	detect_flag=0;			//�Ƿ���м첨�����õ�ʱ������Ϊ0��ʡ��Դ
//u8	DETECT_flag=1,HIGH_flag=0,LOW_flag=0;	//�ߵ͵�ƽ��־λ���Լ��ж����������͵ı�־λ
u32 count=0;//
u8 recv_data[24];//���δ洢4λ������
//u8 recv_sum=0;
u8 recv_pos_i=0;
//u8 *recv_pos=recv_data;  
u8 recv_i=0;
u8 finish_flag=0;

//��ʱ��5�жϷ�����
void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM5_Handler);//��ʱ�����ô�����
}
 

//��ʱ�������жϣ�����������жϴ���ص������� �ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//�����жϣ����������ʱִ��
{
//	TIM5CH1_CAPTURE_STA++;//��������7��8λ��ʾ״̬��ǰ6λ��¼��������������ò���
}


//��ʱ�����벶���жϴ���ص��������ú�����HAL_TIM_IRQHandler�лᱻ����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//�����жϷ���ʱִ��
{
	if(detect_flag){//�����첨
//		/*		�����ܷ��ⷽ����		*/
//		//ע���ѹ��ҪŪ�ɸ��������ֵ>=1.4V���ܼ�⵽��Ƶ��40Kû���⣬50K��ʶ�𲻵���
//		count++;
//		if(count%CarrierFreq==0){//2s��˸һ��
//			LED1=!LED1;
//			count=0;
//		}
//		/*		over		*/
		if(count==0){//�ߵ�ƽ�ĵ�һ��������
			TIM5CH1_CAPTURE_VAL=0;
			TIM4_VAL=0;//������������п��ޣ�����ÿ��ʹ�ö������¸�ֵ
			__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);//��TIM5�ļ���ֵ���㣬��ʼ�Ըߵ�ƽ��������
			__HAL_TIM_SET_COUNTER(&TIM4_Handler,0);
			count++;
		}
		else{
			TIM4_VAL=__HAL_TIM_GET_COUNTER(&TIM4_Handler);//��ȡ��ǰ�ļ���ֵ��Ҳ�����������ؼ��ʱ�䣬��λus
			/*	�ߵ�ƽ����ֵ͵�ƽ�����Ӵ���һ���µĸߵ�ƽ���洢����������	*/
			if(count > (OSC_NUM/2) && TIM4_VAL > DelayTime_us*0.8){//�͵�ƽ
				u8 high_num = count/OSC_NUM;//�ߵ�ƽ����
				u8 low_num = TIM4_VAL/DelayTime_us;//�͵�ƽ��Ŀ
//				recv_sum = recv_sum + high_num + low_num;
				for(recv_i=0;recv_i<high_num; recv_i++){//д��1
//					*recv_pos = 1;
//					recv_pos++;//��������һλ
					recv_data[recv_pos_i]=1;//�ȼ�д��
					recv_pos_i++;
					if(recv_pos_i>=20)
						recv_pos_i = 4;//ǰ4λ�洢 ��ʼ����
				}
				for(recv_i=0;recv_i<low_num; recv_i++){//д��0
//					*recv_pos = 0;
//					recv_pos++;//��������һλ
					recv_data[recv_pos_i]=0;//�ȼ�д��
					recv_pos_i++;
					if(recv_pos_i>=20)
						recv_pos_i = 4;//ǰ4λ�洢 ��ʼ����
				}
				TIM5CH1_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_1);//��ȡ�˴β���ʱ����ֵ
				TIM5CH1_CAPTURE_VAL -= TIM4_VAL;//����õ��ߵ�ƽʱ��
				if(TIM5CH1_CAPTURE_VAL/DelayTime_us != high_num){
					BACK_COLOR=YELLOW;
					POINT_COLOR=RED;
					LCD_ShowxString(10, 30, 16*12, 32, 32, "Detect Error!", 0);//���ּ��㷽�������ͬ������
				}
				//������һ�ּ�������һ���µĸߵ�ƽ��
				count=1;//Ҳ��ȡ0���ص��ʼ���ж���
				__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);
			}
			else{//�ߵ�ƽ�е���
				count++;
			}
			__HAL_TIM_SET_COUNTER(&TIM4_Handler,0);//����������
		}
	}
}//��ʱ���ж�

//û�п����벶����˲������������ɸѡ��������Ϊ���ӣ����ҷ��²���
//	if(detect_flag)//�����첨	(TIM5CH1_CAPTURE_STA&0B10000000)==0)//��δ��������һ��
//	{
//		if(DETECT_flag==1){//��⵽��һ��������
//			DETECT_flag=0;
//			TIM5CH1_CAPTURE_VAL=0;//
//			count=0;
////			__HAL_TIM_DISABLE(&TIM5_Handler);        //�رն�ʱ��5
//			__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);//��TIM5�ļ���ֵ����
////			__HAL_TIM_ENABLE(&TIM5_Handler);//ʹ�ܶ�ʱ��5��׼��������һ�������أ���ʱ��0��ʼ����
//		}
//		else{//�ǵ�һ�������أ�count_flag==1
//			TIM5CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_1);//��ȡ��ǰ�Ĳ���ֵ.
//			if(TIM5CH1_CAPTURE_VAL<=DelayTime*2){//�ܿ��ּ�⵽�������أ�˵���п������ź�
//				count++;//���ź�����񵴼�����625����1��1
//				if(count > OSC_NUM/2){//OSC_NUM����1���ߵ�ƽ�ڰ��������񵴣�Ĭ��625��
//					HIGH_flag=1;//�ܼ�⵽����һ����񵴾���Ϊ�Ǹߵ�ƽ�ź�
//				}
//				__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);//��TIM5�ļ���ֵ����
//			}
//			else{//1���Ѿ�������⵽�ߵ�ƽ(1)����ʱ��û�м�⵽��һ�������أ�ͻȻ��⵽1������Ҫ�ж�������������һ���ߵ�ƽ����
//				 //2����һ�������أ�֮��ܳ�ʱ��û���µ������أ�˵��������������ֵӦ����ȥ�������жϵ�ǰ�������Ƿ�Ϊ����
//				if(HIGH_flag==1){//��Ӧ����1
//					count/= (OSC_NUM);//��һ�θߵ�ƽ�м���1
//					for(i=0;i<count;i++){
//						recv_data[i]=1;//д��1
//					}
//					HIGH_flag=0;//������
//					DETECT_flag=1;//���һ���Ƿ�Ϊ����
//				}
//				else{
//					DETECT_flag=1;//���˺ܾò��ٴμ�⵽�����أ��ж�Ϊ����
//					__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);//��TIM5�ļ���ֵ����
//				}
//			}
//			
//		}
//	}//if(detect_flag)	
