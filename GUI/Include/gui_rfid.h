/**
******************************************************************************
  * @file       gui_rfid.h
  * @brief      RFID图形交互头文件
  * @version    1.0
  * @date       Aug-15-2019 Thu
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_RFID_H
#define __GUI_RFID_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"

#include "user_misc.h"

#include "gui_baseelmt.h"
#include "rfid.h"
#include "time.h"
#include "motor.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    extern u8 PasswordA[6]; // 默认密码暂不修改

    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */

    u8 RFID_CardSelect(u8 (*SerialNum)[4]);
    void GUI_RFID_AuthorizeNewCard(void);
    u8 GUI_RFID_ReadCard(u8 DataBlockAddr, u8 ControlBlockAddr, u8 (*ReadData)[16]);
    void GUI_RFID_CompareCard(void);

#ifdef __cplusplus
}
#endif

#endif
