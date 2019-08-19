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

    GUI_DisplayString(0, 24, &FingerMangeString1_16x16[0][0], 5);

    /* 等待手指按下 */
    while (MG200_DETECT_Status() != SET)
    {
    }

    IDTemp = 0x00; // 使用自动分配
    if (Finger_EnrollNewUser(IDTemp) == 0)
    {
        GUI_DisplayString(4, 32 + 0, &EnrollString_16x16[0][0], 2);
        GUI_DisplayString(4, 32 + 32, &SuccessString_16x16[0][0], 2);
#ifdef DEBUG
        printf("Add new user success\r\n");
#endif
    }
    else
    {
        GUI_DisplayString(4, 32 + 0, &EnrollString_16x16[0][0], 2);
        GUI_DisplayString(4, 32 + 32, &FailString_16x16[0][0], 2);
#ifdef DEBUG
        printf("Could new a user\r\n");
#endif
    }

    Delay(1000);

    GUI_ClearScreen();

    return;
}

void GUI_Finger_EraseAllUser(void)
{
    u8 Temp;

    GUI_ClearScreen();
    GUI_DisplayString(0, 32 + 0, &DeleteString_16x16[0][0], 2);
    GUI_DisplayString(0, 32 + 32, &MenuString3_16x16[4][0], 2);

    if (Finger_EraseAllUser(&Temp) == 0)
    {
        GUI_DisplayString(4, 32 + 0, &DeleteString_16x16[0][0], 2);
        GUI_DisplayString(4, 24 + 32, &SuccessString_16x16[0][0], 2);
    }
    else
    {
        GUI_DisplayString(4, 32 + 0, &DeleteString_16x16[0][0], 2);
        GUI_DisplayString(4, 32 + 32, &FailString_16x16[0][0], 2);
    }

    Delay(1000);

    GUI_ClearScreen();
}

void GUI_Finger_Compare(void)
{
    u8 Finger_ID;

    Voice_Play(VoiceCmd_Di);

    GUI_ClearScreen();

    GUI_ReversalEnable();
    GUI_DisplayPicture(32, 0, 64, 64, &Fingerprint_64px[0][0]);
    GUI_ReversalDisable();

    if (Finger_Compare(&Finger_ID) == 0)
    {
        Voice_Play(VoiceCmd_DOOROPEN_SUCCESS);

        SmartLock_OpenDoor();

        GUI_ClearScreen();
        GUI_DisplayPicture(32, 0, 64, 64, &Fingerprint_64px[0][0]);
        GUI_DisplayString(2, 48, &IdentifyString_16x16[0][0], 2);
        GUI_DisplayString(4, 48, &SuccessString_16x16[0][0], 2);
    }
    else
    {
        GUI_ClearScreen();
        GUI_DisplayPicture(32, 0, 64, 64, &Fingerprint_64px[0][0]);
        GUI_DisplayString(2, 48, &IdentifyString_16x16[0][0], 2);
        GUI_DisplayString(4, 48, &FailString_16x16[0][0], 2);

        SmartLock_CloseDoor();
        Delay(1000);
    }

    /* 等待手指松开 */
    while (MG200_DETECT_Status() != RESET)
    {
    }

    GUI_ClearScreen();

    return;
}

/**
  * @}
  */
