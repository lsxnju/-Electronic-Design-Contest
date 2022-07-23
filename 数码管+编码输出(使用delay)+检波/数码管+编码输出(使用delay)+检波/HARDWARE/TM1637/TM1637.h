#ifndef __TM1637_H
#define __TM1637_H	 
#include "sys.h"

#define TM1640_GPIOPORT	GPIOB	//定义IO接口
#define TM1640_SCLK	GPIO_PIN_12	//定义IO接口
#define TM1640_DIN	GPIO_PIN_13	//定义IO接口


#define TM1640_LEDPORT	0xC8	//定义IO接口

#define I2C_SCL  PBout(12) //定义模拟IIC总线的时钟线
#define I2C_SDA  PBout(13)  //定义模拟IIC总线的数据线

void TM1637_Init(void);//初始化
void TM1637DSP_Control(unsigned char enable_display, unsigned char bright);
void TM1637(void);	
void TM1637_init(void);
void TM1637_show_digit(unsigned char pos, unsigned char digit, unsigned char with_dot);
void TM1637_showint(u16 data);
void TM1637_display(u16 data);
void TM1637_write_display_byte(unsigned char addr, unsigned char segments);
void TM1637_clear(void);
u8 Confirm_Bin(u8* Bin_data, u8 num, char* refer);
u8* BinToHex(u8* Bin_data, u8 num);
u16 BinToDec(u8* Bin_data, u8 num);//哪个数据，几位转换，默认MSB高位在前
#endif
