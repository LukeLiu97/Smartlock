/**
******************************************************************************
  * @file       gui_menu.c
  * @brief      菜单图形交互源文件
  * @version    1.0
  * @date       Aug-16-2019 Fri
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gui_menu.h"

/** @addtogroup GUI menu
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

u8 Menu_Move(u8 *MenuList, u8 MenuListLenth)
{
    u8 KeyValue;

    LED3_OFF();

    KeyValue = Key_Scan();

    if (KeyValue != 0) /* 电容模块检测到按键变化 */
    {
#ifdef DEBUG
        printf("KeyValue = %c\r\n", KeyValue);
#endif

        Voice_Play(VoiceCmd_Di);

        if (KeyValue == '#')
        {
            return MenuPlace_Check;
        }
        else if (KeyValue == '*')
        {
            return MenuPlace_Back;
        }
        else if (KeyValue == '2')
        {
            ArrayForward(MenuList, MenuListLenth);
            return MenuPlace_Shift;
        }
        else if (KeyValue == '8')
        {
            ArrayBackward(MenuList, MenuListLenth);
            return MenuPlace_Shift;
        }
        else
        {
        }
    }
    else
    {
    }

    return MenuPlace_NoEnter;
}

u8 Menu_Move2D(u8 *MenuSubList, u8 MenuListLenth, u8 *MenuListCurrentColumn, u8 MenuListWidth)
{
    u8 KeyValue;

    LED3_OFF();

    KeyValue = Key_Scan();

    if (KeyValue != 0) /* 电容模块检测到按键变化 */
    {
#ifdef DEBUG
        printf("KeyValue = %c\r\n", KeyValue);
#endif

        Voice_Play(VoiceCmd_Di);

        switch (KeyValue)
        {
        case '#':
            return MenuPlace_Check;
        case '*':
            return MenuPlace_Back;
        case '2':
            ArrayForward(MenuSubList, MenuListLenth);
            return MenuPlace_Shift;
        case '8':
            ArrayBackward(MenuSubList, MenuListLenth);
            return MenuPlace_Shift;
        case '4':
        {
            if (*MenuListCurrentColumn == 0)
            {
                *MenuListCurrentColumn = MenuListWidth - 1;
            }
            else
            {
                *MenuListCurrentColumn -= 1;
            }
            return MenuPlace_Shift;
        }
        case '6':
            if (*MenuListCurrentColumn == MenuListWidth - 1)
            {
                *MenuListCurrentColumn = 0;
            }
            else
            {
                *MenuListCurrentColumn += 1;
            }
            return MenuPlace_Shift;
        default:;
        }
    }
    else
    {
    }

    return MenuPlace_NoEnter;
    ;
}

/**
  * @}
  */
