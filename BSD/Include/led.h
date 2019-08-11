/**
******************************************************************************
  * @file       led.h
  * @brief      LED相关函数头文件
  * @version    1.0
  * @date       Tue 06-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define LED3_OFF() (GPIO_SetBits(GPIOB, GPIO_Pin_6))
#define LED3_ON() (GPIO_ResetBits(GPIOB, GPIO_Pin_6))
	 

#define LED2_OFF() (GPIO_SetBits(GPIOB, GPIO_Pin_8))
#define LED2_ON() (GPIO_ResetBits(GPIOB, GPIO_Pin_8))

/* Exported functions ------------------------------------------------------- */
void LED_Init(void);

#ifdef __cplusplus
}
#endif

#endif
