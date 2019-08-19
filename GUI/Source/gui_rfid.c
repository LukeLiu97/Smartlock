/**
******************************************************************************
  * @file       gui_rfid.c
  * @brief      RFID模块图形交互界面源文件
  * @version    1.0
  * @date       Aug-15-2019 Thu
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gui_rfid.h"

/** @addtogroup GUI RFID
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u8 PasswordA[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // 默认密码

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

u8 RFID_CardSelect(u8 (*SerialNum)[4])
{
    u8 CardType[2] = {0};

    /* 寻卡 */
    if (PcdRequest(PICC_REQALL, CardType) == MI_OK)
    {
#ifdef DEBUG
        printf("CardType = %d%d\r\n", CardType[0], CardType[1]);
#endif
        /* 防冲撞 */
        if (PcdAnticoll(*SerialNum) == MI_OK)
        {
#ifdef DEBUG
            printf("SerialNum = %2x%2x%2x%2x\r\n", (*SerialNum)[0], (*SerialNum)[1], (*SerialNum)[2], (*SerialNum)[3]);
#endif
            /* 选卡 */
            if (PcdSelect(*SerialNum) == MI_OK)
            {
#ifdef DEBUG
                printf("Select Card Success \r\n");
#endif
                Voice_Play(VoiceCmd_Di);
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

// 16个扇区共BlockAddr 0~63，建议使用较后地址
// DataBlockAddr ControlBlockAddr要对应
u8 GUI_RFID_WriteCard(u8 DataBlockAddr, u8 ControlBlockAddr, u8 WriteData[16])
{
    u8 SerialNum[4];

    while (1)
    {
        if (RFID_CardSelect(&SerialNum) == 0)
        {
            break;
        }
        else
        {
        }
    }

    /* 验证密码A */
    if (PcdAuthState(PICC_AUTHENT1A, ControlBlockAddr, PasswordA, SerialNum) == MI_OK)
    {
        if (PcdWrite(DataBlockAddr, WriteData) == MI_OK)
        {
#ifdef DEBUG
            printf("Write Card OK \r\n");
#endif
            Voice_Play(VoiceCmd_SETTING_SUCCESS);
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

void GUI_RFID_AuthorizeNewCard(void)
{
    GUI_ClearScreen();

    GUI_DisplayString(0, 32, &MenuString4_16x16[0][0], 4); // 居中显示“登记卡片”

    GUI_DisplayString(4, 24 + 0, &Char_16x16[0][0], 1);         // “请”
    GUI_DisplayString(4, 24 + 16, &LayUpString_16x16[0][0], 2); // “放置”
    GUI_DisplayString(4, 24 + 48, &MenuString4_16x16[4][0], 2); // “卡片”

    Voice_Play(VoiceCmd_PUTCARD);

    if (GUI_RFID_WriteCard(61, 63, SmartLock.IDCardHash) == 0)
    {
        GUI_ClearScreen();
        GUI_DisplayString(4, 32 + 0, &EnrollString_16x16[0][0], 2);
        GUI_DisplayString(4, 32 + 32, &SuccessString_16x16[0][0], 2);
        //		Voice_Play(VoiceCmd_SETTING_SUCCESS);
    }
    else
    {
        GUI_ClearScreen();
        GUI_DisplayString(4, 32 + 0, &EnrollString_16x16[0][0], 2);
        GUI_DisplayString(4, 32 + 32, &FailString_16x16[0][0], 2);
        //		Voice_Play(VoiceCmd_REPETE);
    }
    Delay(1000);

    GUI_ClearScreen();
    return;
}

u8 GUI_RFID_ReadCard(u8 DataBlockAddr, u8 ControlBlockAddr, u8 (*ReadData)[16])
{
    u8 SerialNum[4];

    if (RFID_CardSelect(&SerialNum) == 0)
    {
        /* 验证密码A */
        if (PcdAuthState(PICC_AUTHENT1A, ControlBlockAddr, PasswordA, SerialNum) == MI_OK)
        {
            if (PcdRead(DataBlockAddr, *ReadData) == MI_OK)
            {
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

void GUI_RFID_CompareCard(void)
{
    u8 ReadDataTemp[16];

    if (GUI_RFID_ReadCard(61, 63, &ReadDataTemp) == 0)
    {
        Voice_Play(VoiceCmd_Di);

        GUI_ClearScreen();

        GUI_DisplayPicture(32, 0, 64, 64, &RFIDCard_64px[0][0]);
        GUI_ReversalDisable();

        if (HashCompare(ReadDataTemp, SmartLock.IDCardHash, sizeof(SmartLock.IDCardHash)) == 0)
        {
            Voice_Play(VoiceCmd_DOOROPEN_SUCCESS);
            Delay(300);
            GUI_ClearScreen();
            GUI_DisplayString(2, 48, &IdentifyString_16x16[0][0], 2);
            GUI_DisplayString(4, 48, &SuccessString_16x16[0][0], 2);

            SmartLock_OpenDoor();
        }
        else
        {
            Voice_Play(VoiceCmd_PASSWORD_ERROR);

            Delay(300);
            GUI_ClearScreen();
            GUI_DisplayString(2, 48, &IdentifyString_16x16[0][0], 2);
            GUI_DisplayString(4, 48, &FailString_16x16[0][0], 2);

            SmartLock_CloseDoor();
        }

        Delay(1000);
        GUI_ClearScreen();
    }
    else
    {
        return;
    }
}

/**
  * @}
  */
