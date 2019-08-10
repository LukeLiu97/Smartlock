/**
******************************************************************************
  * @file       oled.h
  * @brief      
  * @version    1.0
  * @date       Aug-09-2019 Fri
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OLED_H
#define __OLED_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
	 
#include "systick.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void OLED_Init(void);	 
void OLED_Config(void);
void OLED_Draw_Point(u8 x,u8 y);
void OLED_Clear(void);	 

void OLED_HalfWidthCharacter(u8 Column,u8 Row);
void OLED_FullWidthCharacter(u8 Column,u8 Row);
	 
#ifdef __cplusplus
}
#endif

#endif
