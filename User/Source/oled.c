/**
******************************************************************************
  * @file       oled.c
  * @brief      
  * @version    1.0
  * @date       Aug-09-2019 Fri
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "oled.h"

/** @addtogroup SPI peri
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

#define OLED_DC_H()			(GPIO_SetBits(GPIOA,GPIO_Pin_11))
#define OLED_DC_L()			(GPIO_ResetBits(GPIOA,GPIO_Pin_11))
#define OLED_RES_H()		(GPIO_SetBits(GPIOA,GPIO_Pin_12))
#define OLED_RES_L()		(GPIO_ResetBits(GPIOA,GPIO_Pin_12))
#define OLED_CS_H()			(GPIO_SetBits(GPIOA,GPIO_Pin_15))
#define OLED_CS_L()			(GPIO_ResetBits(GPIOA,GPIO_Pin_15))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void OLED_Init(void)
{
	/*
		GPIO ����
		OLED_CS PA15 (��Ҫ��ӳ��) OLED_DC PA11 OLED_RES PA12  
	*/
	
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* Configure PA11 and PA12 , PA15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	/* 
		GPIO ����Ϊ SPI2
		SPI_MOSI PB15 SPI_MISO PB14 SPI_SCLK PB13 
	*/
	
	/* GPIOB Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* Configure PB13 and PB14 , PB15 in   mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	/* Configure PB14 in   mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* SPI2 mode Setting */
	SPI_InitTypeDef  SPI_InitStructure;
	
	 /* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* SPI2 configuration ------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	/* SPI2 enable -------------------------------------------------------------*/
	SPI_Cmd(SPI2,ENABLE);
	
	/* GPIO׼�� */
	OLED_CS_H();
	OLED_RES_H();
	
	return ;
}

u8 SPI_Write_Read(u8 SendData)
{
	u8 RecvData;
	
	/* �ȴ����ͼĴ���Ϊ�� */
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE) == RESET)
	{
	}
	/* �������� */
	SPI_I2S_SendData(SPI2,SendData);
	
	/* �ȴ����ܼĴ����ǿ� */
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET)
	{
	}
	/* �������� */
	RecvData = SPI_I2S_ReceiveData(SPI2);

	return RecvData;
}


void OLED_SendCmd(u8 Cmd)
{
	/* ѡ��OLED Ϊ���豸 */
	OLED_CS_L();
	
	/* ָʾOLED��ȡ���� */
	OLED_DC_L();
	
	SPI_Write_Read(Cmd);
	
	/* �������߿���Ȩ */
	OLED_CS_H();
	
	return ;
}

void OLED_SendData(u8 Data)
{
	/* ѡ��OLED Ϊ���豸 */
	OLED_CS_L();
	
	/* ָʾOLED��ȡ���� */
	OLED_DC_H();
	
	SPI_Write_Read(Data);
	
	/* �������߿���Ȩ */
	OLED_CS_H();
	
	return ;
}

void OLED_Config(void)
{
	OLED_RES_L();
	delay_ms(100);
	OLED_RES_H();  
	delay_ms(100);  //��λ

	OLED_SendCmd(0xAE); //�ر���ʾ
	OLED_SendCmd(0xD5); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_SendCmd(80);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_SendCmd(0xA8); //��������·��
	OLED_SendCmd(0X3F); //Ĭ��0X3F(1/64)
	OLED_SendCmd(0xD3); //������ʾƫ��
	OLED_SendCmd(0X00); //Ĭ��Ϊ0

	OLED_SendCmd(0x40); //������ʾ��ʼ�� [5:0],����.

	OLED_SendCmd(0x8D); //��ɱ�����
	OLED_SendCmd(0x14); //bit2������/�ر�
	OLED_SendCmd(0x20); //�����ڴ��ַģʽ
	OLED_SendCmd(0x02); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_SendCmd(0xA1); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_SendCmd(0xC8); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_SendCmd(0xDA); //����COMӲ����������
	OLED_SendCmd(0x12); //[5:4]����

	OLED_SendCmd(0x81); //�Աȶ�����
	OLED_SendCmd(0xEF); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_SendCmd(0xD9); //����Ԥ�������
	OLED_SendCmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_SendCmd(0xDB); //����VCOMH ��ѹ����
	OLED_SendCmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_SendCmd(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_SendCmd(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
	OLED_SendCmd(0xAF); //������ʾ
	
	return ;
}

void OLED_SetLocation(u8 x ,u8 y)	//������ʾλ�� x ʱ�У�0-127�У�  y���У�0-7��һ��8����
{
	OLED_SendCmd(0XB0+y);				//������Y  0-7 ��
	
	OLED_SendCmd(0X00+(x & 0X0F));		//��4λ   
	OLED_SendCmd(0X10+((x & 0XF0)>>4));	//��4λ
	
}

//OLED����
void OLED_Draw_Point(u8 x,u8 y) //x : 0-127  y : 0-63
{
	OLED_SendCmd(0x00+x);					//�����������Y������
	OLED_SendData(0x10+((x & 0x0F)>>4));		//���������4λ
	OLED_SendData(0x00+(x & 0x0F));					//���������4λ

	
	OLED_SendCmd(0xB0+y);					//�����������Y������
	OLED_SendData(0x10+((y & 0x0F)>>4));		//���������4λ
	OLED_SendData(0x00+(y & 0x0F));					//���������4λ

	return ;
}

//����
void OLED_Clear(void)
{
	u8 i,j;
	for(j=0;j<8;j++)
	{
		OLED_SetLocation(0 ,j);
		
		for(i = 0;i < 128 ;i++)
		{
			OLED_SendData(0x00);
		}
	}
	
	return ;
}

void OLED_HalfWidthCharacter(u8 Column,u8 Row)
{
	// 0 <= Column < 16
	// 0 <= Row    < 4
	u8 i;
	
	OLED_SetLocation(Column * 8 ,Row * 2);
		
	for(i = 0;i < 8 ;i++)
	{
		OLED_SendData(0xFF);
	}
	OLED_SetLocation(Column * 8 ,Row * 2 + 1);
	for(i = 0;i < 8 ;i++)
	{
		OLED_SendData(0xFF);
	}
	
	return ;
}

void OLED_FullWidthCharacter(u8 Column,u8 Row)
{
	 u8 Char_16x16[2][16] =
		{
			0x40,0x42,0xCC,0x00,0x00,0x44,0x54,0x54,0x54,0x7F,0x54,0x54,0x54,0x44,0x40,0x00,
			0x00,0x00,0x7F,0x20,0x10,0x00,0xFF,0x15,0x15,0x15,0x55,0x95,0x7F,0x00,0x00,0x00
		};/*"��",0*//* (16 X 16 , ���� )*/
	
	// 0 <= Column < 15
	// 0 <= Row    < 3
	u8 i;
	
	OLED_SetLocation(Column * 8 ,Row * 2);	
	for(i = 0;i < 16 ;i++)
	{
		OLED_SendData(Char_16x16[0][i]);
	}
	OLED_SetLocation(Column * 8 ,Row * 2 + 1);
	for(i = 0;i < 16 ;i++)
	{
		OLED_SendData(Char_16x16[1][i]);
	}
	
	return ;
}

/**
  * @}
  */