/**
******************************************************************************
  * @file       at24c04.c
  * @brief      AT24C04 drive source file
  * @version    1.1
  * @date       Aug-15-2019 Thu
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "at24c04.h"

/** @addtogroup AT24C04 Drive
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void AT24C04_Init(void)
{
	/* 保留暂不使用，该函数执行前需先初始化KEY */
	
	return ;
}

u8 AT24C04_ReadByte(const u8 ByteAddress,u8 *ReadData)
{
	IIC_Start();
	IIC_SendByte(AT24C04_WRITE_ADDR);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	IIC_SendByte(ByteAddress);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	
	IIC_Start();
	IIC_SendByte(AT24C04_READ_ADDR);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	*ReadData = IIC_RecvByte(0);
	IIC_CheckACK();
	
	IIC_Stop();
	
	return 0;
}


// 建议执行后延时一段时间
u8 AT24C04_WriteByte(const u8 ByteAddress,const u8 WriteData)
{
	IIC_Start();
	IIC_SendByte(AT24C04_WRITE_ADDR);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	IIC_SendByte(ByteAddress);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	IIC_SendByte(WriteData);
	if(IIC_CheckACK()!= 1)
	{
		return 1;
	}
	else
	{
	}
	IIC_Stop();
	
	/* 建议等待一段时间 */
	Delay(10);
	
	return 0;
}


u8 AT24C04_WriteBuff(u8 Addr,const u8 *Buff,u8 Size)
{
	for(u32 i = 0; i < Size;i++)
	{
		if(AT24C04_WriteByte(Addr++,*(Buff + i)) != 0)
		{
			return 1;
		}
		else
		{
		}
	}
	
	return 0;
}

u8 AT24C04_ReadBuff(u8 Addr,u8 *Buff,u8 Size)
{
	for(u32 i = 0; i < Size;i++)
	{
		if(AT24C04_ReadByte(Addr++,Buff + i) != 0)
		{
			return 1;
		}
		else
		{
		}
	}
	
	return 0;
}

// 正常写入0 重复写入1 读取失败2 写入失败3 写入数据错误4
u8 AT24C04_Save16Byte(const u8 Data[16])
{
	u8 ReadTemp[16] = {0};
	if(AT24C04_ReadBuff(0x32,ReadTemp,16) != 0)
	{
		return 2;
	}
	else
	{
		if(HashCompare(Data,ReadTemp,16) == 0)
		{
			// 数据一致不再重复写入
			return 1;
		}
		else
		{
			if(AT24C04_WriteBuff(0x32,Data,16) != 0)
			{
				return 3;
			}
			else
			{
				if(AT24C04_ReadBuff(0x32,ReadTemp,16) != 0)
				{
					return 2;
				}
				else
				{
					/* 写入数据不一致 */
					if(HashCompare(Data,ReadTemp,16) != 0)
					{
						return 4;
					}
					else
					{
						return 0;
					}
				}
			}
		}
	}
}


/**
  * @}
  */
