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
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "global.h"

#include "time.h"

    /* Gloabal variables ---------------------------------------------------------*/
    extern u8 ReversalFlag;

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */

    void OLED_Init(void);
    void OLED_Config(void);
    void OLED_Draw_Point(u8 x, u8 y);
    void OLED_Clear(void);
    void OLED_ClearPart(u8 StartRowNum, u8 Height, u8 StartColumn, u8 Width);

    //u8 OLED_Show_Xx16_Character(u8 Row,u8 Column,u8 Width,const u8 *FontArray);
    u8 OLED_Show_XxN8_Character(u8 Row, u8 Column, u8 RowHeight, u8 Width, const u8 *FontArray);
    void OLED_ShowString(u8 RowNumber, u8 RowHeight, u8 Column, const u8 *StringFont, u8 StringLength);
    void OLED_ShowPicture(u8 x, u8 y, u8 px, u8 py, const u8 *Picture);
#ifdef __cplusplus
}
#endif

#endif
