//-----------------------------------------------------------------
// ��������:
//     TMP275��������ͷ�ļ�
// ��    ��: ���ǵ���
// ��ʼ����: 2022-06-27
// �������: 2022-06-27
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2022-06-27)TMP275��ʼ����Ӧ��
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2��TMP275ģ��
// ˵    ��: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "tmp275.h"
#include "exti.h"
//-----------------------------------------------------------------

I2C_HandleTypeDef hi2c1;

//-----------------------------------------------------------------
// void MX_I2C1_Init(void)
//-----------------------------------------------------------------
// 
// ��������: IIC��ʼ��
// ��ڲ���: �� 
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10C0ECFF;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    while(1);
  }
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_DISABLE) != HAL_OK)
  {
    while(1);
  }
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    while(1);
  }
}

//-----------------------------------------------------------------
// void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
//-----------------------------------------------------------------
//
// ��������: IIC�ײ��ʼ����ʱ��ʹ�ܣ���������
// ��ڲ���: i2cHandle:IIC���
// ���ز���: ��
// ע������: �˺����ᱻHAL_I2C_Init()����
//
//-----------------------------------------------------------------
void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      while(1);
    }
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // PB6     ------> I2C1_SCL
    // PB7     ------> I2C1_SDA
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    __HAL_RCC_I2C1_CLK_ENABLE();
  }
}

//-----------------------------------------------------------------
// void TMP275_Init(void)
//-----------------------------------------------------------------
// 
// ��������: TMP275��ʼ��
// ��ڲ���: device���豸��ַ
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void TMP275_Init(uint8_t device)
{
	MX_I2C1_Init();
	Configuration_TMP275(device, 0x60);			// ������ȡ��12λ�ֱ��ʣ�����1������;������ͼ��Ծ������Ƚ�ģʽ������ת��
	Set_Temp_Range(device, 0x020F, 0x0210);	// �����¶Ⱦ�����33�棬��33/0.0625=544=0x0220��
	EXTI_Init();
}

//-----------------------------------------------------------------
// void Read_Temperature(uint8_t device, float *Temp)
//-----------------------------------------------------------------
// 
// ��������: �����¶�
// ��ڲ���: device���豸��ַ
//					 Temp�������õ����¶�
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Read_Temperature(uint8_t device, float *Temp)
{
	uint8_t recv_data[2] = {0};
	uint32_t receive_len = 2;
	uint16_t rd_data;

	uint8_t send_data[1] = {TEMP_ADDR};
	uint16_t send_len = 1;

	HAL_I2C_Master_Transmit(&hi2c1, device, send_data, send_len, 0xFFFF);
	HAL_I2C_Master_Receive(&hi2c1, device, recv_data, receive_len, 0xFFFF);
	rd_data = ((uint16_t)recv_data[0]<<4) | ((uint16_t)recv_data[1]>>4);
	if(rd_data & 0x0800)
		*Temp = (float)(rd_data ^ 0x0FFF) * -0.0625;
	else
		*Temp = (float)(rd_data * 0.0625);
	printf("BH1750 data:0x%x%x\r\n", recv_data[0], recv_data[1]);
}

//-----------------------------------------------------------------
// void e53_ia_init(void)
//-----------------------------------------------------------------
// 
// ��������: TMP275����
// ��ڲ���: device���豸��ַ
//					 dat�����ò���
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Configuration_TMP275(uint8_t device, uint8_t dat)
{
  uint8_t send_data[2] = {0x00, 0x00};
  uint16_t send_len = 2;

	send_data[0] = CONFIG_ADDR;
	send_data[1] = dat;
	
	HAL_I2C_Master_Transmit(&hi2c1, device, send_data, send_len, 0xFFFF); 
}

//-----------------------------------------------------------------
// void Set_Temp_Range(uint8_t device, uint16_t TL, uint16_t TH)
//-----------------------------------------------------------------
// 
// ��������: �����¶Ⱦ�����Χ
// ��ڲ���: device���豸��ַ
//					 TL������¶ȵ�����������
//					 TH������¶ȵ�����������
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void Set_Temp_Range(uint8_t device, uint16_t TL, uint16_t TH)
{
  uint8_t send_data[3] = {0x00, 0x00, 0x00};
  uint16_t send_len = 3;

	send_data[0] = TLOW_ADDR;
	send_data[1] = (uint8_t)((TL>>4)&0x00FF);
	send_data[2] = (uint8_t)((TL<<4)&0x00FF);
	HAL_I2C_Master_Transmit(&hi2c1, device, send_data, send_len, 0xFFFF); 
	
	send_data[0] = THIGH_ADDR;
	send_data[1] = (uint8_t)((TH>>4)&0x00FF);
	send_data[2] = (uint8_t)((TH<<4)&0x00FF);
	HAL_I2C_Master_Transmit(&hi2c1, device, send_data, send_len, 0xFFFF); 
}
