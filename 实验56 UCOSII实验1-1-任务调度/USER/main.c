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

//ALIENTEK 探索者STM32F407开发板 实验56
//UCOSII实验1-任务调度 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK


/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO      			10 //开始任务的优先级设置为最低
//设置任务堆栈大小
#define START_STK_SIZE  				64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);	
 			   
//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO       			7 
//设置任务堆栈大小
#define LED0_STK_SIZE  		    		64
//任务堆栈	
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//任务函数
void led0_task(void *pdata);


//LED1任务
//设置任务优先级
#define LED1_TASK_PRIO       			6
//设置任务堆栈大小
#define LED1_STK_SIZE  					64
//任务堆栈
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void led1_task(void *pdata);

/*
//LED2任务
//设置任务优先级
#define LED2_TASK_PRIO       			7
//设置任务堆栈大小
#define LED2_STK_SIZE  					64
//任务堆栈
OS_STK LED2_TASK_STK[LED2_STK_SIZE];
//任务函数
void led2_task(void *pdata);*/
//对输入数字进行编码
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart_init(115200);		//初始化串口波特率为115200	
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口 
	LCD_Init();					//LCD初始化
	Init_KeyBoard_Port();//按键初始化
	//Relay_Init();				//继电器in初始化
	TIM3_Int_Init(50-1,84-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms     
	//EXTIX_Init();				//中断初始化
	POINT_COLOR=BLACK; 
	LCD_Display_Dir(1);
	OSInit();   
 	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	
}

 //开始任务
void start_task(void *pdata)
{
  OS_CPU_SR cpu_sr=0;
	pdata = pdata; 
  OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)  

 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				 
 	//OSTaskCreate(led2_task,(void *)0,(OS_STK*)&LED2_TASK_STK[LED2_STK_SIZE-1],LED2_TASK_PRIO);	 	  
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
} 

//LED0任务
void led0_task(void *pdata)
{	 	
	while(1)
	{
		printf("%d ",num);
		//LED1=!LED1;
		//delay_ms(90);
		ROW_H_COL_L();
		key_input=ReadKeyBoard();
		if(key_input==10||key_input==11||key_input==12||key_input==15)			//暂停
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

//LED1任务
void led1_task(void *pdata)
{	
	while(1){
		printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& ");
		/*LED0=!LED0;
		delay_ms(100);
		*/
		if(flag>=0)
		{
			//flag=1时，对数值重新赋值
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
//LED2任务
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
//对当前按键进行编码
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


