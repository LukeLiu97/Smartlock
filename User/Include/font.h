/**
******************************************************************************
  * @file       font.h
  * @brief      字体头文件
  * @version    1.0
  * @date       Aug-10-2019 Sat
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONT_H
#define __FONT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"//需要u8类型定义
	 
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern const u8 Char_16x16[2][16];
extern const u8 String1_16xN[32][16];
extern const u8 String2_16xN[10][16];
	 
extern const u8 Char_8x16[2][16];
	 
extern const u8 String3_16xN[8][16];
extern const u8 String4_16xN[10][16];
	 
extern const u8 MenuString1_16x16[12][16];
extern const u8 MenuString2_16x16[8][16];
extern const u8 MenuString3_16x16[8][16];
extern const u8 MenuString4_16x16[8][16];
	 
extern const u8 Number_8x16[10][16];

extern const u8 Logo_72x64[8][72];
	 
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
