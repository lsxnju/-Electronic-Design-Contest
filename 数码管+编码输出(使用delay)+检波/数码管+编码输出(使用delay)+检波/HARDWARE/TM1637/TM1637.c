#include "TM1637.h"
#include "delay.h"
#include "string.h"

#define TM1637_BRIGHTNESS 1  // 亮度等級调节0-7

void TM1637_clear(void)
{
	u8 i;
	//清空数码管
	for (i = 0; i < 4; i++)
    {
        TM1637_write_display_byte(i, 0x00);
    }
}
/*
GPIO口配置：
            PA0:CLK
            PA1：DIN
*/
void TM1637_Init(void)
{                                                                                                          // TM1640接口初始化
	GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
	
    GPIO_Initure.Pin=TM1640_DIN | TM1640_SCLK; //
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	TM1637DSP_Control(1, TM1637_BRIGHTNESS); //开屏指令写入，亮度一般2
	TM1637_clear();
}

/*
开始时序模拟 
*/
void I2C_start()
{
    I2C_SCL = 1;
    I2C_SDA = 1;

    delay_us(1);

    I2C_SDA = 0; // 时钟线设置为常态，低电平。
}

/*
I2C 总线写1个字节


不包含应答位的检测
*/
void I2C_write_byte(char byte_to_write)
{
    unsigned char i;

    for (i = 0; i < 8; i++)
    {
        I2C_SCL = 0;

        I2C_SDA = byte_to_write & 0x01;
        delay_us(1);

        I2C_SCL = 1;
        delay_us(1);

        I2C_SCL = 0;
        delay_us(1);

        byte_to_write >>= 1;
    }

    return;
}

void I2C_check_ack()
{
    // I2C 检查应答位，实际未检查，只发送了应答位时钟，认为TM1637工作正常

    I2C_SCL = 0;
    delay_us(1); //在第八个时钟下降沿之后延时5us，开始判断ACK 信号

    /*  while(I2C_SDA)
      {
          ;
      }*/

    I2C_SCL = 1;
    delay_us(1);

    I2C_SCL = 0;
}

void I2C_send_ack()
{
    I2C_SDA = 0;
    delay_us(1);

    I2C_SCL = 1;
    delay_us(1);

    I2C_SCL = 0;
    delay_us(1);

    I2C_SDA = 1;
    delay_us(1);
}

void I2C_stop()
{
    I2C_SCL = 0;
    delay_us(1);

    I2C_SDA = 0;
    delay_us(1);

    I2C_SCL = 1;
    delay_us(1);

    I2C_SDA = 1;
}

/*
command:写入命令（以0x8*）
*/
void TM1637_write_cmd(unsigned char command)
{
    I2C_start();
    I2C_write_byte(command);
    I2C_check_ack();
    I2C_stop();
}

/*
enable_display：1开屏，0,息屏
bright：亮度等级 0 - 7
*/
void TM1637DSP_Control(unsigned char enable_display, unsigned char bright)
{
    TM1637_write_cmd(0x80 + (enable_display ? 0x08 : 0x00) + bright); /*0x8f(0x08开屏，0x07亮度等级调整)*/
}

void TM1637_write_display_bytes(unsigned char first_addr,
                                unsigned char bytes_to_write[],
                                unsigned char num_of_bytes)
{
    unsigned char i;

    I2C_start();
    I2C_write_byte(0x40);
    I2C_check_ack();
    I2C_stop();

    I2C_start();
    I2C_write_byte(0xC0 + first_addr);
    I2C_check_ack();

    for (i = 0; i < num_of_bytes; i++)
    {
        I2C_write_byte(bytes_to_write[i]);
        I2C_check_ack();
    }

    I2C_stop();
}

// addr: 0 - 5
/*
数据读写模式设置 0x40:  写数据到显示寄存器
                0x42:  读键扫数据
地址增加模式设置 0x44:  固定地址
                0x40:  自动地址增加
测试模式设置（内 0x48:  测试模式
部使用）         0x40:  普通模式


地址命令设设置  0xC0:CH0地址
               0xC1:CH1地址
               0xC2:CH2地址
               0xC3:CH3地址
               0xC4:CH4地址
               0xC5:CH5地址
*/
void TM1637_write_display_byte(unsigned char addr, unsigned char segments)
{
    I2C_start();
    I2C_write_byte(0x40 + addr);
    I2C_check_ack();
    I2C_stop();

    I2C_start();
    I2C_write_byte(0xC0 + addr);
    I2C_check_ack();

    I2C_write_byte(segments);
    I2C_check_ack();

    I2C_stop();
}

unsigned char TM1637_SEG_TAB[] =
    {
        0x3F // 00111111 0
        ,
        0x06 // 00000110 1
        ,
        0x5B // 01011011 2
        ,
        0x4F // 01001111 3
        ,
        0x66 // 01100110 4
        ,
        0x6d // 01101101 5
        ,
        0x7d // 01111101 6
        ,
        0x07 // 00000111 7
        ,
        0x7F // 01111111 8
        ,
        0x6F // 01101111 9
        ,
        0x77 // 01110111 A
        ,
        0x7C // 01111100 b
        ,
        0x39 // 00111001 C
        ,
        0x5E // 01011110 d
        ,
        0x79 // 01111001 E
        ,
        0x71 // 01110001 F
};

void TM1637_show_digit(unsigned char pos, unsigned char digit, unsigned char with_dot)//最后一位控制小数点
{
    TM1637_write_display_byte(pos, TM1637_SEG_TAB[digit] | (with_dot ? 0x20 : 0x00));
}

void TM1637_showint(u16 data)
{
	if (data > 9999)
	{
		data = 9999;//最多四位数
	}
	if (data > 999)//四位数
	{
		TM1637_show_digit(0, (u8)(data / 1000), 0);//千位
	    TM1637_show_digit(1, (u8)(data / 100 % 10), 0);//百位
		TM1637_show_digit(2, (u8)(data % 100 / 10), 0);//十位
	    TM1637_show_digit(3, (u8)(data % 10), 0);//个位
	}else if (data > 99)//三位数
	{
		TM1637_show_digit(1, (u8)(data / 100), 0);//百位
		TM1637_show_digit(2, (u8)(data % 100 / 10), 0);//十位
	    TM1637_show_digit(3, (u8)(data % 10), 0);//个位
	}else if (data > 9)//两位数
	{
		TM1637_show_digit(2, (u8)(data / 10), 0);//十位
	    TM1637_show_digit(3, (u8)(data % 10), 0);//个位
	}else//一位数
	{
		TM1637_show_digit(3, (u8)data, 0);
	}
}

void TM1637_display(u16 data)
{
	TM1637_show_digit(0, (u8)(data / 1000), 0);//千位
	TM1637_show_digit(1, (u8)(data / 100 % 10), 0);//百位
	TM1637_show_digit(2, (u8)(data % 100 / 10), 0);//十位
	TM1637_show_digit(3, (u8)(data % 10), 0);//个位
}
u16 BinToDec(u8* Bin_data, u8 num)//哪个数据，几位转换，默认MSB高位在前
{
	u8 Dec_num = num/4, i, j;
	u16 Dec_data=0;
	u8 Dec[6]={0};//最多不过6位
	for(i=0;i<Dec_num;i++){
		for(j=0; j<4; j++){//4位二进制
			Dec[i] = Dec[i]*2 + *Bin_data;
			Bin_data++;
		}
		Dec_data = Dec_data*10 + Dec[i];
	}
	return Dec_data;
}

u8 Hex[6]={0};//最多不过6位
u8* BinToHex(u8* Bin_data, u8 num)
{
	u8 Hex_num = num/4, i, j;
	memset(Hex, 0, sizeof(u8)*6);//清零
	for(i=0;i<Hex_num;i++){
		for(j=0; j<4; j++){//4位二进制
			Hex[i] = Hex[i]*2 + *Bin_data;
			Bin_data++;//注意不要访问越界
		}
	}
	return Hex;
}

u8 Confirm_Bin(u8* Bin_data, u8 num, char* refer)
{
	char Bin[8]={0};
	u8 i;
	for(i=0;i<num;i++){
		Bin[i] = '0'+Bin_data[i];
	}
	if(strcmp(Bin,refer)==0)
		return 1;//一致
	else
		return 0;//不一致
	
}
