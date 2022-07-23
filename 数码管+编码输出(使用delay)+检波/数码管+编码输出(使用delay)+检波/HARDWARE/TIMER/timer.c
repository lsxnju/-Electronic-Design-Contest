#include "timer.h"
#include "led.h"
#include "DigitOut.h"
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器中断驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/4/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved	
////////////////////////////////////////////////////////////////////////////////// 	
//修改说明
//V1.1 20170407
//新增TIM14_PWM_Init函数,用于PWM输出
//V1.2 20170408
//新增TIM5_CH1_Cap_Init函数,用于输入捕获
////////////////////////////////////////////////////////////////////////////////// 

TIM_HandleTypeDef TIM3_Handler;      	//定时器3句柄 
TIM_HandleTypeDef TIM4_Handler;      	//定时器4句柄
TIM_HandleTypeDef TIM14_Handler;      	//定时器14句柄 
TIM_OC_InitTypeDef TIM14_CH1Handler;	//定时器14通道1句柄

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=psc;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM3_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM4_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                          //通用定时器4
    TIM4_Handler.Init.Prescaler=psc;                     //分频系数
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM4_Handler.Init.Period=arr;                        //自动装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM4_Handler);//里面会开启
    
    HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}
//TIM14 PWM部分初始化 
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
void TIM14_PWM_Init(u16 arr,u16 psc)
{  
    TIM14_Handler.Instance=TIM14;          	//定时器14
    TIM14_Handler.Init.Prescaler=psc;       //定时器分频
    TIM14_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM14_Handler.Init.Period=arr;          //自动重装载值
    TIM14_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM14_Handler);       //初始化PWM
    
    TIM14_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM14_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM14_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM14_Handler,&TIM14_CH1Handler,TIM_CHANNEL_1);//配置TIM14通道1
	
    HAL_TIM_PWM_Start(&TIM14_Handler,TIM_CHANNEL_1);//开启PWM通道1
}


//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	}
	else if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();            //使能TIM4时钟
		HAL_NVIC_SetPriority(TIM4_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM4中断   
	}
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM14_CLK_ENABLE();			//使能定时器14
	__HAL_RCC_GPIOF_CLK_ENABLE();			//开启GPIOF时钟
	
	GPIO_Initure.Pin=GPIO_PIN_9;           	//PF9
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	GPIO_Initure.Alternate= GPIO_AF9_TIM14;	//PF9复用为TIM14_CH1
	HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}

//设置TIM通道4的占空比
//compare:比较值
void TIM_SetTIM14Compare1(u32 compare)
{
	TIM14->CCR1=compare; 
}

//获取TIM捕获/比较寄存器值
u32 TIM_GetTIM14Capture1(void)
{
	return  HAL_TIM_ReadCapturedValue(&TIM14_Handler,TIM_CHANNEL_1);
}


//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}


////回调函数，定时器中断服务函数调用
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if(htim==(&TIM3_Handler))
//    {
//        LED1=!LED1;        //LED1反转
//    }
//}


/***************************************************************************
****************************************************************************
  下面是输入捕获相关源码实验相关函数源码
****************************************************************************
****************************************************************************/

TIM_HandleTypeDef TIM5_Handler;         //定时器5句柄

//定时器5通道1输入捕获配置
//arr：自动重装值(TIM2,TIM5是32位的!!)u32也即32位二进制，也即8位16进制，最大取0XFFFF FFFF
//psc：时钟预分频数
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{  
    TIM_IC_InitTypeDef TIM5_CH1Config;  
    
    TIM5_Handler.Instance=TIM5;                          	//通用定时器5
    TIM5_Handler.Init.Prescaler=psc;                     	//分频系数
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    	//向上计数器
    TIM5_Handler.Init.Period=arr;                        	//自动装载值
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;	//时钟分频因子
    HAL_TIM_IC_Init(&TIM5_Handler);							//初始化输入捕获时基参数
    
    TIM5_CH1Config.ICPolarity=TIM_ICPOLARITY_RISING;    	//上升沿捕获
    TIM5_CH1Config.ICSelection=TIM_ICSELECTION_DIRECTTI;	//映射到TI1上
    TIM5_CH1Config.ICPrescaler=TIM_ICPSC_DIV1;          	//配置输入分频，不分频
    TIM5_CH1Config.ICFilter=0;                          	//配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM5_Handler,&TIM5_CH1Config,TIM_CHANNEL_1);//配置TIM5通道1
	
    HAL_TIM_IC_Start_IT(&TIM5_Handler,TIM_CHANNEL_1);   	//开启TIM5的捕获通道1，并且开启捕获中断
    __HAL_TIM_ENABLE_IT(&TIM5_Handler,TIM_IT_UPDATE);   	//使能更新中断
}


//定时器5底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_IC_Init()调用
//htim:定时器5句柄
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM5_CLK_ENABLE();            //使能TIM5时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    GPIO_Initure.Alternate=GPIO_AF2_TIM5;   //PA0复用为TIM5通道1
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

    HAL_NVIC_SetPriority(TIM5_IRQn,2,0);    //设置中断优先级，抢占优先级2，子优先级0
    HAL_NVIC_EnableIRQ(TIM5_IRQn);          //开启ITM5中断通道  
}


//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
//u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态，8位二进制，由0000 0000到1111 1111，  				
u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
u32	TIM4_VAL;				//TIM4的计数值
u8	detect_flag=0;			//是否进行检波，不用的时候设置为0节省资源
//u8	DETECT_flag=1,HIGH_flag=0,LOW_flag=0;	//高低电平标志位，以及判断上升沿类型的标志位
u32 count=0;//
u8 recv_data[24];//依次存储4位二进制
//u8 recv_sum=0;
u8 recv_pos_i=0;
//u8 *recv_pos=recv_data;  
u8 recv_i=0;
u8 finish_flag=0;

//定时器5中断服务函数
void TIM5_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM5_Handler);//定时器共用处理函数
}
 

//定时器更新中断（计数溢出）中断处理回调函数， 该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//更新中断（溢出）发生时执行
{
//	TIM5CH1_CAPTURE_STA++;//例程里是7、8位表示状态，前6位记录溢出次数，这里用不到
}


//定时器输入捕获中断处理回调函数，该函数在HAL_TIM_IRQHandler中会被调用
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//捕获中断发生时执行
{
	if(detect_flag){//开启检波
//		/*		测试能否检测方波振荡		*/
//		//注意电压不要弄成负数，峰峰值>=1.4V都能检测到，频率40K没问题，50K就识别不到了
//		count++;
//		if(count%CarrierFreq==0){//2s闪烁一次
//			LED1=!LED1;
//			count=0;
//		}
//		/*		over		*/
		if(count==0){//高电平的第一次上升沿
			TIM5CH1_CAPTURE_VAL=0;
			TIM4_VAL=0;//这两个归零可有可无，后面每次使用都会重新赋值
			__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);//将TIM5的计数值归零，开始对高电平计算脉宽
			__HAL_TIM_SET_COUNTER(&TIM4_Handler,0);
			count++;
		}
		else{
			TIM4_VAL=__HAL_TIM_GET_COUNTER(&TIM4_Handler);//获取当前的计数值，也即两个上升沿间隔时间，单位us
			/*	高电平后出现低电平，并接触到一个新的高电平，存储二进制数据	*/
			if(count > (OSC_NUM/2) && TIM4_VAL > DelayTime_us*0.8){//低电平
				u8 high_num = count/OSC_NUM;//高电平个数
				u8 low_num = TIM4_VAL/DelayTime_us;//低电平数目
//				recv_sum = recv_sum + high_num + low_num;
				for(recv_i=0;recv_i<high_num; recv_i++){//写入1
//					*recv_pos = 1;
//					recv_pos++;//步进到下一位
					recv_data[recv_pos_i]=1;//等价写法
					recv_pos_i++;
					if(recv_pos_i>=20)
						recv_pos_i = 4;//前4位存储 开始数据
				}
				for(recv_i=0;recv_i<low_num; recv_i++){//写入0
//					*recv_pos = 0;
//					recv_pos++;//步进到下一位
					recv_data[recv_pos_i]=0;//等价写法
					recv_pos_i++;
					if(recv_pos_i>=20)
						recv_pos_i = 4;//前4位存储 开始数据
				}
				TIM5CH1_CAPTURE_VAL = HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_1);//读取此次捕获时的数值
				TIM5CH1_CAPTURE_VAL -= TIM4_VAL;//作差得到高电平时间
				if(TIM5CH1_CAPTURE_VAL/DelayTime_us != high_num){
					BACK_COLOR=YELLOW;
					POINT_COLOR=RED;
					LCD_ShowxString(10, 30, 16*12, 32, 32, "Detect Error!", 0);//两种计算方法结果不同，报错
				}
				//开启新一轮计数（是一个新的高电平）
				count=1;//也可取0，回到最开始的判断里
				__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);
			}
			else{//高电平中的振荡
				count++;
			}
			__HAL_TIM_SET_COUNTER(&TIM4_Handler,0);//计数器归零
		}
	}
}//定时器中断

//没有开输入捕获的滤波，纯靠软件来筛选噪声，较为复杂，暂且放下不管
//	if(detect_flag)//开启检波	(TIM5CH1_CAPTURE_STA&0B10000000)==0)//还未完整捕获一次
//	{
//		if(DETECT_flag==1){//检测到第一个上升沿
//			DETECT_flag=0;
//			TIM5CH1_CAPTURE_VAL=0;//
//			count=0;
////			__HAL_TIM_DISABLE(&TIM5_Handler);        //关闭定时器5
//			__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);//将TIM5的计数值归零
////			__HAL_TIM_ENABLE(&TIM5_Handler);//使能定时器5，准备捕获下一次上升沿，此时从0开始计数
//		}
//		else{//非第一个上升沿，count_flag==1
//			TIM5CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&TIM5_Handler,TIM_CHANNEL_1);//获取当前的捕获值.
//			if(TIM5CH1_CAPTURE_VAL<=DelayTime*2){//很快又检测到了上升沿，说明有可能是信号
//				count++;//对信号里的振荡计数，625个是1个1
//				if(count > OSC_NUM/2){//OSC_NUM代表1个高电平内包含几个振荡，默认625个
//					HIGH_flag=1;//能检测到超过一半的振荡就认为是高电平信号
//				}
//				__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);//将TIM5的计数值归零
//			}
//			else{//1、已经连续检测到高电平(1)，长时间没有检测到下一个上升沿，突然检测到1个，需要判断是噪声还是下一个高电平到来
//				 //2、第一个上升沿，之后很长时间没有新的上升沿，说明是噪声，计数值应当舍去，再来判断当前上升沿是否为噪声
//				if(HIGH_flag==1){//对应类型1
//					count/= (OSC_NUM);//上一次高电平有几个1
//					for(i=0;i<count;i++){
//						recv_data[i]=1;//写入1
//					}
//					HIGH_flag=0;//先清零
//					DETECT_flag=1;//检测一下是否为噪声
//				}
//				else{
//					DETECT_flag=1;//隔了很久才再次检测到上升沿，判定为噪声
//					__HAL_TIM_SET_COUNTER(&TIM5_Handler,0);//将TIM5的计数值归零
//				}
//			}
//			
//		}
//	}//if(detect_flag)	
