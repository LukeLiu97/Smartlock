/**
******************************************************************************
  * @file       gui_clock.h
  * @brief      时钟图形化交互头文件
  * @version    1.0
  * @date       Aug-16-2019 Fri
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_CLOCK_H
#define __GUI_CLOCK_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "gui_baseelmt.h"
#include "gui_menu.h"
#include "ui.h"

#include "global.h"

#include "rtc.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    typedef enum
    {
        ClockSubMenu_Father = 0,
        ClockSubMenu_Clock = 1,
        ClockSubMenu_Date = 2,
    } ClockSubMenu_TypeDef;

    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */

    void GUI_CLOCK_SettingTime(u32 *LastSubMenu);
    void GUI_CLOCK_SettingDate(u32 *LastSubMenu);
    void GUI_DispClock(void);

#ifdef __cplusplus
}
#endif

#endif
