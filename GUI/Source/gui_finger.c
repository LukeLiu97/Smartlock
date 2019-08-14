/**
******************************************************************************
  * @file       gui_finger.c
  * @brief      指纹图形交互
  * @version    1.0
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gui_finger.h"

/** @addtogroup GUI Fingerprint
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void GUI_Finger_EnrollNewUser(void)
{
	u8 IDTemp;
	
	GUI_ClearScreen();
	
	GUI_DisplayString(0,24,&FingerMangeString1_16x16[0][0],5);
	
	/* 等待手指按下 */
	while(MG200_DETECT_Status() != SET)
	{
	}

	IDTemp = 0x00;// 使用自动分配
	if(Finger_EnrollNewUser(IDTemp) == 0)
	{
		GUI_DisplayString(2,24,&DeleteString1_16x16[4][0],2);
		printf("Add new user success\r\n");
	}
	else
	{
		GUI_DisplayString(2,24,&DeleteString2_16x16[4][0],2);
		printf("Could new a user\r\n");
	}
	
	Delay(1000);
	
	GUI_ClearScreen();
	
	return ;
}

void GUI_Finger_EraseAllUser(void)
{
	u8 Temp;
	
	GUI_ClearScreen();
	
	if(Finger_EraseAllUser(&Temp) == 0)
	{
		GUI_DisplayString(4,32,&DeleteString1_16x16[0][0],4);
	}
	else
	{
		GUI_DisplayString(4,32,&DeleteString2_16x16[0][0],4);
	}

	Delay(1000);
	
	GUI_ClearScreen();
}

/**
  * @}
  */
