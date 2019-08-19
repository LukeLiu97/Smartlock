/**
******************************************************************************
  * @file       gui_password.h
  * @brief      密码输入图形交互头文件
  * @version    1.0
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_PASSWORD_H
#define __GUI_PASSWORD_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"

#include "gui_baseelmt.h"

#include "user_misc.h"

#include "led.h"
#include "key.h"

    /* Exported functions ------------------------------------------------------- */

    extern u8 Key_Scan(void);

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */

    u8 GUI_Password_Enroll(u8 *PasswordBuff, u8 PasswordLength);
    u8 GUI_Password_Check(const u8 *Password, u8 PasswordLength);

#ifdef __cplusplus
}
#endif

#endif
