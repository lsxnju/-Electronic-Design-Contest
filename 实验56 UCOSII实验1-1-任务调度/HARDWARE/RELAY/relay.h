#ifndef _RELAY_H
#define _RELAY_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define Relay1 PFout(8)   //Relay1
#define Relay2 PFout(9)   //Relay2
#define Relay3 PFout(10)
#define Relay4 PCout(0)
#define Relay5 PCout(1)

void Relay_Init(void);
#endif
