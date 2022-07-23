#ifndef _RELAY_H
#define _RELAY_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define Relay1 PFout(8)   //Relay1
#define Relay2 PFout(9)   //Relay2
#define Relay3 PFout(10)
#define Relay4 PCout(0)
#define Relay5 PCout(1)

void Relay_Init(void);
#endif
