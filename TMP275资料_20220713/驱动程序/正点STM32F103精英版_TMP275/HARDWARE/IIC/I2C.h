//-----------------------------------------------------------------
// IIC驱动头文件
// 头文件名: I2C.h
// 作    者: 凌智电子
// 开始日期: 2014-01-28
// 完成日期: 2014-01-28
// 修改日期: 2014-02-16
// 当前版本: V1.0.1
// 历史版本:
//   - V1.0: (2014-02-07)IIC配置
//-----------------------------------------------------------------
#ifndef __I2C_H
#define __I2C_H
#include "sys.h"

//-----------------------------------------------------------------
// 位操作
//-----------------------------------------------------------------
#define IIC_SCL    PAout(6) //SCL
#define IIC_SDA    PAout(7) //SDA	 
#define READ_SDA   PAin(7)  //输入SDA 

#define SDA_IN()  {GPIOA->CRL&=0X0FFFFFFF;GPIOA->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOA->CRL&=0X0FFFFFFF;GPIOA->CRL|=(u32)3<<28;}
//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void IIC_Init(void);			 
extern void IIC_Start(void);
extern void IIC_Stop(void);
extern void IIC_Send_Byte(uint8_t txd);
extern uint8_t IIC_Read_Byte(unsigned char ack);
extern uint8_t IIC_Wait_Ack(void);
extern void IIC_Ack(void);
extern void IIC_NAck(void);
extern void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
extern uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  

#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
