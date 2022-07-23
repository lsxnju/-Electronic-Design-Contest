#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

#include "sys.h"

#define ROW1		PCin(9)
#define ROW2		PCin(7)
#define ROW3		PGin(8)
#define ROW4		PGin(6)

#define LINE1		PDout(2)
#define LINE2		PCout(11)
#define LINE3		PAout(15)
#define LINE4		PAout(13)

void keyboard_init(void);
void keyboard_GPIO_init(void);

#endif

