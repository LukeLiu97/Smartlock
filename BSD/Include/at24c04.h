/**
******************************************************************************
  * @file       at24c04.h
  * @brief      AT24C04 drive header file
  * @version    1.0
  * @date       Aug-15-2019 Thu
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT24C04_H
#define __AT24C04_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"

#include "iic.h"

#include "time.h"
#include "user_misc.h"
#include "global.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/

#define AT24C04_ADDR (0x50)
#define AT24C04_WRITE_ADDR ((AT24C04_ADDR << 1) + 0)
#define AT24C04_READ_ADDR ((AT24C04_ADDR << 1) + 1)

    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */

    u8 AT24C04_ReadByte(const u8 ByteAddress, u8 *ReadData);
    u8 AT24C04_WriteByte(const u8 ByteAddress, const u8 WriteData);
    u8 AT24C04_WriteBuff(u8 Addr, const u8 *Buff, u8 Size);
    u8 AT24C04_ReadBuff(u8 Addr, u8 *Buff, u8 Size);
    u8 AT24C04_Save16Byte(const u8 Addr, const u8 Data[16]);
    void AT24C04_SaveSmartLockData(void);

#ifdef __cplusplus
}
#endif

#endif
