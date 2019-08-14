/**
******************************************************************************
  * @file       global.h
  * @brief      全局类型变量定义
  * @version    1.0 
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_H
#define __GLOBAL_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Extern variables ----------------------------------------------------------*/
extern u8 ReversalFlag;	 

extern u8 CurrentWindowMode;

extern u8 CurrentUserMode;

/* Exported types ------------------------------------------------------------*/
typedef struct SmartLockStu_
{
	u8 AdminPassword[7];
	u8 UserPassword[7];
	u8 MuteMode;
	u8 LockFlag;
}SmartLockStu;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
