/**
******************************************************************************
  * @file       GUI_BaseElmt.h
  * @brief      图形基础元素
  * @version    1.0
  * @date       Aug-13-2019 Tue
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_BASEELMT_H
#define __GUI_BASEELMT_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "oled.h"
#include "font.h"
#include "voice.h"

    /* Gloabal variables ---------------------------------------------------------*/
    extern u8 ReversalFlag;

    /* Extern functions --------------------------------------------------------- */
    extern u8 OLED_Show_XxN8_Character(u8 Row, u8 Column, u8 RowHeight, u8 Width, const u8 *FontArray);
    extern void OLED_ShowString(u8 RowNumber, u8 RowHeight, u8 Column, const u8 *StringFont, u8 StringLength);
    extern void OLED_ShowPicture(u8 x, u8 y, u8 px, u8 py, const u8 *Picture);
    extern void OLED_Clear(void);
    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    /* Exported macro ------------------------------------------------------------*/

#define GUI_ClearScreen OLED_Clear
#define GUI_ClearPart OLED_ClearPart
    /* Exported functions ------------------------------------------------------- */

    void GUI_ReversalEnable(void);
    void GUI_ReversalDisable(void);

    void GUI_DisplayPassBox(u8 RowNumber, u8 *Str);
    void GUI_DisplayChar(u8 RowNumber, u8 Column, u8 RowHeight, u8 Width, const u8 *FontArray);
    void GUI_Show8StrTextBox(u8 RowNumber, u8 RowHeight, u8 *Str);
    void GUI_DisplayString(u8 RowNumber, u8 Column, const u8 *StringFont, u8 StringLength);
    void GUI_ShowOperationTipRow(u8 RowNumber, u8 RowHeight); // 显示文字 按#确认 按*取消

    void GUI_DisplayPicture(u8 x, u8 y, u8 px, u8 py, const u8 *Picture);
    void GUI_DisplayNumber(u8 RowNumber, u8 Column, u32 Number, u8 ShowLength, u8 FontSize);

#ifdef __cplusplus
}
#endif

#endif
