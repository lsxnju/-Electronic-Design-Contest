#ifndef _DIGITOUT_H
#define _DIGITOUT_H
#include "sys.h"

#define DigitOutput PBout(14)   	//
#define CarrierFreq 10000 //10kHz
#define OutFreq 16 //16Hz
#define OSC_NUM CarrierFreq/OutFreq//һ���ߵ�ƽ�ڵ��񵴴���
#define DelayTime_ms 1000/OutFreq//ÿ�ε�ʱ�䣬��λms
#define DelayTime_us 1000000/OutFreq//ÿ�ε�ʱ�䣬��λus
#define OUT_MSB//��λ�ȴ��������ɸ�λ����λ����ÿ�����ֵĶ�����Ҳ���ߵ��ʹ�

void Output_start(void);
void Output_stop(void);
void Output_FourDigit(u16 data);
#endif
