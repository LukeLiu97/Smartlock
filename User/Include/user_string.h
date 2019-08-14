/**
******************************************************************************
  * @file       User_String.h
  * @brief      用户自定的字符串函数
  * @version    1.0
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_STRING_H
#define __USER_STRING_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "stm32f10x.h"
	
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
s8 String_Compare(const u8 *String1,const u8 *String2);
s32 String_ViolentMatch(const u8 *TargetString,const u8 *MatchingString);

#ifdef __cplusplus
}
#endif

#endif
