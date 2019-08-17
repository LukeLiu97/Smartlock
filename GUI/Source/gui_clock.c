/**
******************************************************************************
  * @file       gui_clock.c
  * @brief      时钟图形化交互源文件
  * @version    1.0
  * @date       Aug-16-2019 Fri
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gui_clock.h"

/** @addtogroup GUI Clock
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void GUI_CLOCK_SettingTime(u32 *LastSubMenu)
{	
	static u8 CurrentPlace[6][10] = 
	{
		{0,1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,8,9},
	};
	static u8 CurrentColumnPlace = 0;
	
	const u8 HighLightPlace[6] = {16,28,52,64,90,102};
	
	switch (Menu_Move2D(CurrentPlace[CurrentColumnPlace],10,&CurrentColumnPlace,6))
	{
		case MenuPlace_Check:
//			SubMenu_Change(LastMenu,CurrentPlace[1]);
			*LastSubMenu = ClockSubMenu_Clock;
			GUI_ClearScreen();
			break;
		case MenuPlace_Back:
			*LastSubMenu = ClockSubMenu_Father;
			GUI_ClearScreen();
			break;
		case MenuPlace_Shift:
			*LastSubMenu = ClockSubMenu_Clock;
	//		GUI_ClearScreen();
			break;
		default:
			*LastSubMenu = ClockSubMenu_Clock;
			break;
	}
	
	OLED_ShowString(0,2,32,&ClockSettingTimeString_16x16[0][0],4); // 居中显示“修改时间”
	
	GUI_DisplayNumber(3,20,CurrentPlace[0][9],1,1);
	GUI_DisplayNumber(4,16,CurrentPlace[0][0],1,3);
	GUI_DisplayNumber(7,20,CurrentPlace[0][1],1,1);
	
	GUI_DisplayNumber(3,32,CurrentPlace[1][9],1,1);
	GUI_DisplayNumber(4,28,CurrentPlace[1][0],1,3);
	GUI_DisplayNumber(7,32,CurrentPlace[1][1],1,1);

	
	GUI_DisplayNumber(3,56,CurrentPlace[2][9],1,1);
	GUI_DisplayNumber(4,52,CurrentPlace[2][0],1,3);
	GUI_DisplayNumber(7,56,CurrentPlace[2][1],1,1);
	
	GUI_DisplayNumber(3,68,CurrentPlace[3][9],1,1);
	GUI_DisplayNumber(4,64,CurrentPlace[3][0],1,3);
	GUI_DisplayNumber(7,68,CurrentPlace[3][1],1,1);
	
	
	GUI_DisplayNumber(3,94,CurrentPlace[4][9],1,1);
	GUI_DisplayNumber(4,90,CurrentPlace[4][0],1,3);
	GUI_DisplayNumber(7,94,CurrentPlace[4][1],1,1);
	
	GUI_DisplayNumber(3,106,CurrentPlace[5][9],1,1);
	GUI_DisplayNumber(4,102,CurrentPlace[5][0],1,3);
	GUI_DisplayNumber(7,106,CurrentPlace[5][1],1,1);
	
	
	GUI_ReversalEnable();
	GUI_DisplayNumber(4,HighLightPlace[CurrentColumnPlace],CurrentPlace[CurrentColumnPlace][0],1,3);
	GUI_ReversalDisable();
	
	Delay(50);
	
	if(*LastSubMenu != ClockSubMenu_Clock)
	{
		GUI_ClearScreen();
	}
	else
	{
	}
	
	return ;
}

void GUI_CLOCK_SettingDate(u32 *LastSubMenu)
{	
	static u8 CurrentPlace[6][10] = 
	{
		{0,1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,8,9},
		{0,1,2,3,4,5,6,7,8,9},
	};
	static u8 CurrentColumnPlace = 0;
	
	const u8 HighLightPlace[6] = {28,40,64,76,100,112};
	
	switch (Menu_Move2D(CurrentPlace[CurrentColumnPlace],10,&CurrentColumnPlace,6))
	{
		case MenuPlace_Check:
//			SubMenu_Change(LastMenu,CurrentPlace[1]);
			*LastSubMenu = ClockSubMenu_Date;
			GUI_ClearScreen();
			break;
		case MenuPlace_Back:
			*LastSubMenu = ClockSubMenu_Father;
			GUI_ClearScreen();
			break;
		case MenuPlace_Shift:
			*LastSubMenu = ClockSubMenu_Date;
	//		GUI_ClearScreen();
			break;
		default:
			*LastSubMenu = ClockSubMenu_Date;
			break;
	}
	
	OLED_ShowString(0,2,32,&ClockSettingTimeString_16x16[0][0],4); // 居中显示“修改时间”
	
	GUI_DisplayNumber(4,4,20,2,3); // "20"
	
	GUI_DisplayNumber(3,HighLightPlace[0]+4,CurrentPlace[0][9],1,1);
	GUI_DisplayNumber(4,HighLightPlace[0],CurrentPlace[0][0],1,3);
	GUI_DisplayNumber(7,HighLightPlace[0]+4,CurrentPlace[0][1],1,1);
	
	GUI_DisplayNumber(3,HighLightPlace[1]+4,CurrentPlace[1][9],1,1);
	GUI_DisplayNumber(4,HighLightPlace[1],CurrentPlace[1][0],1,3);
	GUI_DisplayNumber(7,HighLightPlace[1]+4,CurrentPlace[1][1],1,1);

	
	GUI_DisplayNumber(3,HighLightPlace[2]+4,CurrentPlace[2][9],1,1);
	GUI_DisplayNumber(4,HighLightPlace[2],CurrentPlace[2][0],1,3);
	GUI_DisplayNumber(7,HighLightPlace[2]+4,CurrentPlace[2][1],1,1);
	
	GUI_DisplayNumber(3,HighLightPlace[3]+4,CurrentPlace[3][9],1,1);
	GUI_DisplayNumber(4,HighLightPlace[3],CurrentPlace[3][0],1,3);
	GUI_DisplayNumber(7,HighLightPlace[3]+4,CurrentPlace[3][1],1,1);
	
	
	GUI_DisplayNumber(3,HighLightPlace[4],CurrentPlace[4][9],1,1);
	GUI_DisplayNumber(4,HighLightPlace[4],CurrentPlace[4][0],1,3);
	GUI_DisplayNumber(7,HighLightPlace[4]+4,CurrentPlace[4][1],1,1);
	
	GUI_DisplayNumber(3,HighLightPlace[5]+4,CurrentPlace[5][9],1,1);
	GUI_DisplayNumber(4,HighLightPlace[5],CurrentPlace[5][0],1,3);
	GUI_DisplayNumber(7,HighLightPlace[5]+4,CurrentPlace[5][1],1,1);
	
	
	GUI_ReversalEnable();
	GUI_DisplayNumber(4,HighLightPlace[CurrentColumnPlace],CurrentPlace[CurrentColumnPlace][0],1,3);
	GUI_ReversalDisable();
	
	Delay(50);
	
	if(*LastSubMenu != ClockSubMenu_Date)
	{
		GUI_ClearScreen();
	}
	else
	{
	}
	
	return ;
}


/**
  * @}
  */
