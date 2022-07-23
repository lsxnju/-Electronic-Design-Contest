#ifndef _DIGITOUT_H
#define _DIGITOUT_H
#include "sys.h"

#define DigitOutput PBout(14)   	//
void Output_start(void);
void Output_stop(void);
void Output_FourDigit(u16 data);
#endif
