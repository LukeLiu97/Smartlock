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


/* Exported types ------------------------------------------------------------*/
typedef struct SmartLockStu_
{
	u8 AdminPassword[7];
	u8 UserPassword[7];
	u8 MuteMode;
	u8 LockFlag;
	u8 IDCardHash[16];
}SmartLockStu;

typedef struct TimeStu_
{
	u32 Year;
	u8 Month;
	u8 Day;
	u8 Date;
	u8 Hour;
	u8 Minute;
	u8 Second;
}TimeStu;

typedef struct FingerPackStu_
{
	u8 Data[8];
	u8 Count;
	u8 Over;
}FingerPackStu;

/* Extern variables ----------------------------------------------------------*/
extern u8 ReversalFlag;	 

extern u8 CurrentWindowMode;

extern u8 CurrentUserMode;

extern SmartLockStu SmartLock;

extern FingerPackStu FingerPack;

extern TimeStu UserTime;

extern u32 UnBusy_Count;

extern u32 TimeDisplay;

#define BUFF_LENGTH 256  // 定义输入缓冲区长度
extern u8 StringBuff[BUFF_LENGTH];
extern u16 gTouchStatus;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define SmartLock_OpenDoor() (SmartLock.LockFlag = 0x00)
#define SmartLock_CloseDoor() (SmartLock.LockFlag = 0x01)

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
