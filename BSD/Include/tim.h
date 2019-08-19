/**
******************************************************************************
  * @file       tim.h
  * @brief      通用定时器驱动头文件
  * @version    1.0
  * @date       Aug-12-2019 Mon
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */

    void TIM2_Delay_ms(u16 ms);
    void TIM3_Interrupt_Init(void);
    void TIM4_Delay_us(u16 us);

#ifdef __cplusplus
}
#endif

#endif
