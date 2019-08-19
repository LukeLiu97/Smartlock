/**
******************************************************************************
  * @file       User_misc.h
  * @brief      用户自定的函数或模块
  * @version    1.0
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_MISC_H
#define __USER_MISC_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */
    s8 String_Compare(const u8 *String1, const u8 *String2);
    s32 String_ViolentMatch(const u8 *TargetString, const u8 *MatchingString);
    u8 HashCompare(const u8 *Key1, const u8 *Key2, u8 Length);
    u8 FakePassword_Check(u8 *TagetStr, u8 *MatchStr);
    void ArrayBackward(u8 *Array, u8 Length);
    void ArrayForward(u8 *Array, u8 Length);

#ifdef __cplusplus
}
#endif

#endif
