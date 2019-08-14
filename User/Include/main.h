/**
******************************************************************************
  * @file       main.h
  * @brief      主程序头文件
  * @note       
  * @version    1.1
  * @date       Tue 06-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "systick.h"
	 
#include "led.h"
#include "time.h"
#include "uart.h"
#include "voice.h"
#include "iic.h"
#include "mpr121.h"
#include "key.h"
#include "oled.h"
#include "font.h"
#include "tim.h"
#include "finger.h"
#include "motor.h"
#include "rfid.h"

#include "gui.h"	 
#include "ui.h"

	 
/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

u8 Admin_Check(void);

#ifdef __cplusplus
}
#endif

#endif
