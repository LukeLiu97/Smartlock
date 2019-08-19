/**
******************************************************************************
  * @file       gui_memory.c
  * @brief      存储管理图形交互源文件
  * @version    1.0
  * @date       Aug-18-2019 Sun
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gui_memory.h"

/** @addtogroup GUI Memory
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void GUI_Memory_Clear(void)
{
    u32 i = 0;
    const u8 WriteData = 0xFF;
    u8 ReadData;

    /* 屏蔽全局中断 */
    __set_PRIMASK(1);

    for (i = 0; i < 256; i++)
    {
        if (AT24C04_WriteBuff(i, &WriteData, 1) != 0)
        {
            break;
        }
        else
        {
            if (AT24C04_ReadBuff(i, &ReadData, 1) != 0)
            {
                break;
            }
            else
            {
                /* 写入数据不一致 */
                if (WriteData != ReadData)
                {
                    break;
                }
                else
                {
                }
            }
        }
    }
    /* 允许全局中断 */
    __set_PRIMASK(0);

    if (i != 256)
    {
        GUI_ClearScreen();
        GUI_DisplayString(4, 32 + 0, &DeleteString_16x16[0][0], 2);
        GUI_DisplayString(4, 32 + 32, &FailString_16x16[0][0], 2);
        Voice_Play(VoiceCmd_REPETE);
    }
    else
    {
        GUI_ClearScreen();
        GUI_DisplayString(4, 32 + 0, &DeleteString_16x16[0][0], 2);
        GUI_DisplayString(4, 32 + 32, &SuccessString_16x16[0][0], 2);
        Voice_Play(VoiceCmd_SETTING_SUCCESS);
    }

    SmartLock.MemoryFlag = RESET;

    Delay(1000);

    return;
}

void GUI_Memory_Enable(void)
{
    u8 OK_FLag = RESET;
    u8 WriteData = EEPROM_ENABLE_FLAG;
    u8 ReadData1, ReadData2;

    /* 屏蔽全局中断 */
    __set_PRIMASK(1);

    if (AT24C04_WriteBuff(EEPROM_ENABLE_BYTE, &WriteData, 1) != 0 || AT24C04_WriteBuff(EEPROM_ENABLE_BYTE + EEPROM_OFFSET, &WriteData, 1) != 0)
    {
    }
    else
    {
        if (AT24C04_ReadBuff(EEPROM_ENABLE_BYTE, &ReadData1, 1) != 0 || AT24C04_ReadBuff(EEPROM_ENABLE_BYTE + EEPROM_OFFSET, &ReadData2, 1) != 0)
        {
        }
        else
        {
            /* 写入数据不一致 */
            if (ReadData1 != ReadData2)
            {
            }
            else
            {
                OK_FLag = SET;
            }
        }
    }

    /* 允许全局中断 */
    __set_PRIMASK(0);

    if (OK_FLag == RESET)
    {
        GUI_ClearScreen();
        GUI_DisplayString(4, 32 + 0, &EnableString_16x16[0][0], 2);
        GUI_DisplayString(4, 32 + 32, &FailString_16x16[0][0], 2);
        Voice_Play(VoiceCmd_REPETE);
        SmartLock.MemoryFlag = RESET;
    }
    else
    {
        GUI_ClearScreen();
        GUI_DisplayString(4, 32 + 0, &EnableString_16x16[0][0], 2);
        GUI_DisplayString(4, 32 + 32, &SuccessString_16x16[0][0], 2);
        Voice_Play(VoiceCmd_SETTING_SUCCESS);
        SmartLock.MemoryFlag = SET;
        AT24C04_SaveSmartLockData();
    }

    Delay(1000);

    return;
}

/**
  * @}
  */
