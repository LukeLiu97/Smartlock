/**
******************************************************************************
  * @file       motor.h
  * @brief      电机控制驱动头文件
  * @version    1.0
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_H
#define __MOTOR_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "time.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    /* Exported macro ------------------------------------------------------------*/

#define MOTOR_IA_H() GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define MOTOR_IA_L() GPIO_ResetBits(GPIOA, GPIO_Pin_5)

#define MOTOR_IB_H() GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define MOTOR_IB_L() GPIO_ResetBits(GPIOA, GPIO_Pin_4)

    /* Exported functions ------------------------------------------------------- */
    void Motor_Init(void);
    void Motor_CloseLock(void);
    void Motor_OpenLock(void);

#ifdef __cplusplus
}
#endif

#endif
