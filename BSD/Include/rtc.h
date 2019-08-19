/**
******************************************************************************
  * @file       rtc.h
  * @brief      Real time clock drive
  * @version    1.0
  * @date       Aug-15-2019 Thu
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "global.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */
    void RTC_Init(void);
    void RTC_TimeCalulate(TimeStu *Time);

    u8 Set_RTCTime(TimeStu *Time);
    void Read_RTCTime(TimeStu *Time);

#ifdef __cplusplus
}
#endif

#endif
