/**
******************************************************************************
  * @file       gui_memory.h
  * @brief      存储管理图形交互头文件
  * @version    1.0
  * @date       Aug-18-2019 Sun
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_MEMORY_H
#define __GUI_MEMORY_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "gui_baseelmt.h"

#include "global.h"
#include "at24c04.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */
    void GUI_Memory_Clear(void);
    void GUI_Memory_Enable(void);

#ifdef __cplusplus
}
#endif

#endif
