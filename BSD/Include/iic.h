/**
******************************************************************************
  * @file       iic.h
  * @brief      
  * @version    1.0
  * @date       Wed 07-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IIC_H
#define __IIC_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "time.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define NOP_TIME 4
/* Exported macro ------------------------------------------------------------*/
//SCK PB4 SDA PB5
#define SDA_Status() (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5))
#define SCK_H() (GPIO_SetBits(GPIOB, GPIO_Pin_4))
#define SCK_L() (GPIO_ResetBits(GPIOB, GPIO_Pin_4))
#define SDA_H() (GPIO_SetBits(GPIOB, GPIO_Pin_5))
#define SDA_L() (GPIO_ResetBits(GPIOB, GPIO_Pin_5))

    /* Exported functions ------------------------------------------------------- */
    void IIC_Software_Init(void);
    void IIC_Start(void);
    void IIC_Stop(void);
    void IIC_SendACK(void);
    void IIC_SendNoACK(void);
    u8 IIC_CheckACK(void);
    void IIC_SendByte(u8 data);
    u8 IIC_RecvByte(u8 End);

#ifdef __cplusplus
}
#endif

#endif
