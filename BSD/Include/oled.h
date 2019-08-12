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

//u8 OLED_Show_Xx16_Character(u8 Row,u8 Column,u8 Width,const u8 *FontArray);
u8 OLED_Show_XxN8_Character(u8 Row,u8 Column,u8 RowHeight,u8 Width,const u8 *FontArray);

void OLED_ShowPicture(u8 x,u8 y,u8 px,u8 py,const u8 *Picture);
#ifdef __cplusplus
}
#endif

#endif
