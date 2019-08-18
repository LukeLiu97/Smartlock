/**
******************************************************************************
  * @file       finger.h
  * @brief      
  * @version    1.0
  * @date       Aug-13-2019 Tue
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FINGER_H
#define __FINGER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"

#include "uart.h"

#include "global.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    typedef enum
    {
        FingerPack_UnUse = 0x00,             // 保留
        FingerPack_CaptureAndExtract = 0x51, // 采集与提取特征点
        FingerPack_EraseAll = 0x54,          // 删除所有用户
        FingerPack_SlaveAddr = 0x62,         // 模块地址
        FingerPack_MasterAddr = 0x63,        // MCU地址
        FingerPack_Start = 0x6C,             // 起始码
        FingerPack_Match1N = 0x71,           // 对比所有用户指纹
        FingerPack_Enroll = 0x7F,            // 注册新用户

    } FingerPack_TypeDef;

    /* Exported macro ------------------------------------------------------------*/

#define MG200_PWR_OFF() (GPIO_ResetBits(GPIOB, GPIO_Pin_9))
#define MG200_PWR_ON() (GPIO_SetBits(GPIOB, GPIO_Pin_9))

#define MG200_DETECT_Status() (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))

    /* Exported functions ------------------------------------------------------- */

    void Fingerprint_Init(void);
    u8 Finger_CaptureAndExtract(u8 time);
    u8 Finger_EnrollNewUser(u8 ID);
    u8 Finger_Compare(u8 *ID);
    u8 Finger_EraseAllUser(u8 *Result);

#ifdef __cplusplus
}
#endif

#endif
