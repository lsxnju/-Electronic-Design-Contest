//-----------------------------------------------------------------
// 程序描述:
//     TMP275驱动程序
// 作    者: 凌智电子
// 开始日期: 2014-01-28
// 完成日期: 2014-01-28
// 修改日期: 2014-02-16
// 当前版本: V1.0.1
// 历史版本:
//  - V1.0: (2014-02-07)TMP275配置
// - V1.0.1:(2014-02-16)头文件中不包含其他头文件
// 调试工具: 凌智STM32核心开发板、LZE_ST_LINK2、TMP275模块
// 说    明:
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "tmp275.h"
#include "I2C.h"

//-----------------------------------------------------------------
// void TMP275_Init(void)
//-----------------------------------------------------------------
// 
// 函数功能: TMP275初始化
// 入口参数: device：设备地址
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void TMP275_Init(uint8_t device)
{
	IIC_Init();
	Configuration_TMP275(device, 0x60);			// 连续读取，12位分辨率，出现1个错误就警报，低极性警报，比较模式，连续转换
	Set_Temp_Range(device, 0x020F, 0x0210);	// 设置温度警报线33℃，（33/0.0625=544=0x0220）

}

//-----------------------------------------------------------------
// void Read_Temperature(uint8_t device, float *Temp)
//-----------------------------------------------------------------
// 
// 函数功能: 测量温度
// 入口参数: device：设备地址
//					 Temp：测量得到的温度
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void Read_Temperature(uint8_t device, float *Temp)
{
	uint8_t recv_data[2] = {0};
	uint16_t rd_data;

	IIC_Start(); 
	IIC_Send_Byte(device);
	IIC_Wait_Ack();
	IIC_Send_Byte(TEMP_ADDR);
	IIC_Wait_Ack();
	IIC_Start(); 
	IIC_Send_Byte(device|0x01);
	IIC_Wait_Ack();
	recv_data[0]=IIC_Read_Byte(1);
	recv_data[1]=IIC_Read_Byte(1);
	IIC_Stop();
	rd_data = ((uint16_t)recv_data[0]<<4) | ((uint16_t)recv_data[1]>>4);
	if(rd_data & 0x0800)
		*Temp = (float)(rd_data ^ 0x0FFF) * -0.0625;
	else
		*Temp = (float)(rd_data * 0.0625);
}

//-----------------------------------------------------------------
// void e53_ia_init(void)
//-----------------------------------------------------------------
// 
// 函数功能: TMP275配置
// 入口参数: device：设备地址
//					 dat：配置参数
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void Configuration_TMP275(uint8_t device, uint8_t dat)
{
	IIC_Start(); 
	IIC_Send_Byte(device);
	IIC_Wait_Ack();
	IIC_Send_Byte(CONFIG_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte(dat);
	IIC_Wait_Ack();
	IIC_Stop();
}

//-----------------------------------------------------------------
// void Set_Temp_Range(uint8_t device, uint16_t TL, uint16_t TH)
//-----------------------------------------------------------------
// 
// 函数功能: 设置温度警报范围
// 入口参数: device：设备地址
//					 TL：最低温度的数字量数据
//					 TH：最高温度的数字量数据
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void Set_Temp_Range(uint8_t device, uint16_t TL, uint16_t TH)
{
	IIC_Start(); 
	IIC_Send_Byte(device);
	IIC_Wait_Ack();
	IIC_Send_Byte(TLOW_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)((TL>>4)&0x00FF));
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)((TL<<4)&0x00FF));
	IIC_Wait_Ack();
	IIC_Stop();
	
	IIC_Start(); 
	IIC_Send_Byte(device);
	IIC_Wait_Ack();
	IIC_Send_Byte(THIGH_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)((TH>>4)&0x00FF));
	IIC_Wait_Ack();
	IIC_Send_Byte((uint8_t)((TH<<4)&0x00FF));
	IIC_Wait_Ack();
	IIC_Stop();
}
