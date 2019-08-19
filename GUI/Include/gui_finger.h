/**
******************************************************************************
  * @file       gui_finger.h
  * @brief      指纹图形交互界面
  * @version    1.0
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_FINGER_H
#define __GUI_FINGER_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h" // 引入数据类型定义

#include "gui_baseelmt.h"
#include "time.h"

#include "finger.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */

    void GUI_Finger_EnrollNewUser(void);
    void GUI_Finger_EraseAllUser(void);
    void GUI_Finger_Compare(void);

#ifdef __cplusplus
}
#endif

#endif
