/**
******************************************************************************
  * @file       gui_menu.h
  * @brief      菜单图形交互头文件
  * @version    1.0
  * @date       Aug-16-2019 Fri
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_MENU_H
#define __GUI_MENU_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "gui_baseelmt.h"
#include "key.h"
#include "led.h"
#include "user_misc.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    typedef enum
    {
        MenuPlace_NoEnter = 0,
        MenuPlace_Check = 1,
        MenuPlace_Back = 2,
        MenuPlace_Shift = 3
    } MenePlace_TypeDef;

    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */
    u8 Menu_Move(u8 *MenuList, u8 MenuListLenth);
    u8 Menu_Move2D(u8 *MenuSubList, u8 MenuListLenth, u8 *MenuListCurrentColumn, u8 MenuListWidth);

#ifdef __cplusplus
}
#endif

#endif
