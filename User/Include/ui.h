/**
******************************************************************************
  * @file       ui.h
  * @brief      User Interface hearder
  * @version    1.0
  * @date       Aug-12-2019 Mon
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UI_H
#define __UI_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
	 
#include "systick.h"
#include "time.h"
#include "led.h"
#include "voice.h"
#include "oled.h"
#include "key.h"
#include "font.h"
#include "tim.h"
	 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
typedef enum
{
	WindowMode_AllClear = 0,
	WindowMode_Admin	= 1,
	WindowMode_User		= 2,
	WindowMode_Setting	= 3
}WindowMode_TypeDef;

#define BUFF_LENGTH 256  // 定义输入缓冲区状态

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void Window_MainTask(void);

/* Extern variables ----------------------------------------------------------*/
extern u16 gTouchStatus;
	 
#ifdef __cplusplus
}
#endif

#endif
