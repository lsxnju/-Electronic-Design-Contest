#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "timer.h"
#include "LCD12864.h"
#include "includes.h"
#include "KeyBoard.h"
//#include "relay.h"
//#include "exti.h"

//ALIENTEK ̽����STM32F407������ ʵ��56
//UCOSIIʵ��1-������� -�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK


/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO      			10 //��ʼ��������ȼ�����Ϊ���
//���������ջ��С
#define START_STK_SIZE  				64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);	
 			   
//LED0����
//�����������ȼ�
#define LED0_TASK_PRIO       			7 
//���������ջ��С
#define LED0_STK_SIZE  		    		64
//�����ջ	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//������
void led0_task(void *pdata);


//LED1����
//�����������ȼ�
#define LED1_TASK_PRIO       			6
//���������ջ��С
#define LED1_STK_SIZE  					64
//�����ջ
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//������
void led1_task(void *pdata);

/*
//LED2����
//�����������ȼ�
#define LED2_TASK_PRIO       			7
//���������ջ��С
#define LED2_STK_SIZE  					64
//�����ջ
OS_STK LED2_TASK_STK[LED2_STK_SIZE];
//������
void led2_task(void *pdata);*/
//���������ֽ��б���
void encoder(int key_input);
int flag;
extern int Enter_flag;
int code[20]={1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int code1[20]={1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int key_input=0;	
int stop=0;
int stop_flag;
int num=0;


int main(void)
{ 
	flag=-1;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200	
	delay_init(168);		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿� 
	LCD_Init();					//LCD��ʼ��
	Init_KeyBoard_Port();//������ʼ��
	//Relay_Init();				//�̵���in��ʼ��
	TIM3_Int_Init(50-1,84-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����5000��Ϊ500ms     
	//EXTIX_Init();				//�жϳ�ʼ��
	POINT_COLOR=BLACK; 
	LCD_Display_Dir(1);
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();	
}

 //��ʼ����
void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)  

 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				 
 	//OSTaskCreate(led2_task,(void *)0,(OS_STK*)&LED2_TASK_STK[LED2_STK_SIZE-1],LED2_TASK_PRIO);	 	  
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
} 

//LED0����
void led0_task(void *pdata)
{	 	
	while(1)
	{
		printf("%d ",num);
		//LED1=!LED1;
		//delay_ms(90);
		ROW_H_COL_L();
		key_input=ReadKeyBoard();
		if(key_input==10||key_input==11||key_input==12||key_input==15)			//��ͣ
		{
			if(stop_flag==1)
			{
				LCD_Clear(WHITE);
				stop=1;
				flag=-1;
				stop_flag=0;
				num=0;
			}
		}
		else
		{
			stop_flag=1;
			
		}
		if(key_input==13)
		{
			POINT_COLOR=RED; 
			LCD_ShowxNum(660,375,num,4,24,0);
			POINT_COLOR=BLACK; 
			stop=0;
			encoder(num);
			for(int i=0;i<20;i++)
			{
				printf("%d:%d ",i,code[i]);
			}
			flag=1;
			delay_ms(10);
			num=0;
		}
		else if(key_input>=0 && key_input<=9)
		{
			
			num=num*10+key_input;
			LCD_ShowxNum(660,375,num,4,24,0);
		}
	}
}

//LED1����
void led1_task(void *pdata)
{	
	while(1){
		printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& ");
		/*LED0=!LED0;
		delay_ms(100);
		*/
		if(flag>=0)
		{
			//flag=1ʱ������ֵ���¸�ֵ
			if(flag==1){
				for(int i=0;i<=15;i++)
				{
					code1[i+4]=code[i+4];
				}
				flag=0;
			}
			for(int j=0;j<=19;j++)
			{
				LED0=code1[j];
				delay_ms(56);
			}
		}
		else
		{
			if(stop==1)
			{
				stop=0;
				for(int j=0;j<=3;j++)
				{
					LED0=1;
					delay_ms(56);
				}
			}
			
			LED0=0;
			delay_ms(100);
		}
		
	}
}
/*
//LED2����
void led2_task(void *pdata)
{	
	while(1){
		printf("task2 ");
		Relay2=1;
		delay_ms(1);
		//printf("%ld ",Relay2);
		Relay2=0;
		delay_ms(1);
		//printf("%ld ",Relay2);
	}
}*/
//�Ե�ǰ�������б���
void encoder(int key_input)
{
	int key_num[4]={0,0,0,0};
	int key=key_input;
	for(int i=3;i>=0;i--)
	{
		key_num[i]=key%10;
		key=key/10;		
	}
	for(int i=3;i>=0;i--)
	{
		for(int j=3;j>=0;j--)
		{
			code[4*i+j+4]=key_num[i]%2;
			key_num[i]=key_num[i]/2;
		}
	}
	
}


