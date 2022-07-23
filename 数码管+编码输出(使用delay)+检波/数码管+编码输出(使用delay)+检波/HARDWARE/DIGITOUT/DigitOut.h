#ifndef _DIGITOUT_H
#define _DIGITOUT_H
#include "sys.h"

#define DigitOutput PBout(14)   	//
#define CarrierFreq 10000 //10kHz
#define OutFreq 16 //16Hz
#define OSC_NUM CarrierFreq/OutFreq//一个高电平内的振荡次数
#define DelayTime_ms 1000/OutFreq//每段的时间，单位ms
#define DelayTime_us 1000000/OutFreq//每段的时间，单位us
#define OUT_MSB//高位先传，数字由高位到低位传，每个数字的二进制也按高到低传

void Output_start(void);
void Output_stop(void);
void Output_FourDigit(u16 data);
#endif
