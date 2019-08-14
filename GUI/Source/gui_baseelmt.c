/**
******************************************************************************
  * @file       GUI_BaseElmt.c
  * @brief      图形基础元素
  * @version    1.0
  * @date       Aug-13-2019 Tue
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gui_baseelmt.h"


/** @addtogroup Base Element
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void GUI_ReversalEnable(void)
{
	ReversalFlag = 1;
	return ;
}
void GUI_ReversalDisable(void)
{
	ReversalFlag = 1;
	return ;
}

void GUI_DisplayString(u8 RowNumber,u8 Column,const u8 *StringFont,u8 StringLength)
{
	OLED_ShowString(RowNumber,2,Column,StringFont,StringLength);
	return ;
}

void GUI_DisplayPicture(u8 x,u8 y,u8 px,u8 py,const u8 *Picture)
{
	OLED_ShowPicture(x,y,px,py,Picture);
	
	return ;
}


// 设置为6位密码输入框
void GUI_DisplayPassBox(u8 RowNumber,u8 *Str)
{
	for(u32 i = 0; i < 6; i++)
	{
		if(Str[i] == 0) // 传入的字符序列对应字符值为0
		{
			// 显示‘_’
			OLED_Show_XxN8_Character(RowNumber,(i* 16) + 20,2,8,&Char_8x16[0][0]);
		}
		else // 传入的字符序列对应字符值非0
		{
			// 显示‘*’
			OLED_Show_XxN8_Character(RowNumber,(i* 16) + 20,2,8,&Char_8x16[1][0]);
		}
	}
	return ;
}

/**
  * @}
  */
