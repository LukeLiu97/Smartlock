/**
******************************************************************************
  * @file       key.h
  * @brief      按键相关函数头文件
  * @version    1.0
  * @date       Thu 08-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

//#ifdef __MPR121_H
#include "mpr121.h"
//#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define MPR_IRQ() (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3))

    /* Exported functions ------------------------------------------------------- */
    void Key_Init(void);
    u8 Key_Scan(void);
    void EXTI3_Init(void);
    void EXTI3_Disable(void);

#ifdef __cplusplus
}
#endif

#endif
