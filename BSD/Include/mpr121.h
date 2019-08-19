/**
******************************************************************************
  * @file       mpr121.h
  * @brief      电容按键传感器驱动头文件
  * @version    1.0
  * @date       Wed 07-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MPR121_H
#define __MPR121_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "iic.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define MPR_ADD 0x5A //设备地址

#define KEY_NO 0x00
#define KEY_0 0x01
#define KEY_1 0x02
#define KEY_2 0x03
#define KEY_3 0x04
#define KEY_4 0x05
#define KEY_5 0x06
#define KEY_6 0x07
#define KEY_7 0x08
#define KEY_8 0x09
#define KEY_9 0x0A
#define KEY_ENTER 0x0B
#define KEY_BACK 0x0C

    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */
    void MPR121_Init(void);
    u16 MPR_TouchStatus(void);
    void MPR_IRQ_Init(void);
    u8 MPR_Read(u8 SlaveAddress, u8 RegisterAddress);

#ifdef __cplusplus
}
#endif

#endif
