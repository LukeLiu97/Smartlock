/**
******************************************************************************
  * @file       ui.c
  * @brief      User Interface
  * @version    2.0
  * @date       Aug-12-2019 Mon
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ui.h"

/** @addtogroup User Interface
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static u8 Password_Input(void);

static void Admin_Mode(void);

/* Private functions ---------------------------------------------------------*/

void OLED_ShowWaitRow(u8 RowNumber, u8 RowHeight)
{

    for (u32 i = 0; i < 5; i++) // 显示5个字
    {
        OLED_Show_XxN8_Character(RowNumber, i * 16 + 24, RowHeight, 16, &(String4_16xN[2 * i][0]));
    }
    return;
}

u8 Password_Input(void)
{
    static u32 i = 0;
    u8 KeyValue;

    LED3_OFF();

    KeyValue = Key_Scan();

    if (KeyValue != 0) /* 电容模块检测到按键变化 */
    {
#ifdef DEBUG
        printf("KeyValue = %c\r\n", KeyValue);
#endif

        Voice_Play(VoiceCmd_Di);

        if (KeyValue == '#' || i > BUFF_LENGTH - 2)
        {
            StringBuff[i] = 0;
            i = 0;
            return 0;
        }
        else if (KeyValue == '*')
        {
            if (i == 0)
            {
                // Don't do anything
            }
            else
            {
                i -= 1;
            }
            StringBuff[i] = 0; // 现在i向后退一位
        }
        else
        {
            StringBuff[i] = KeyValue; // 当前输入被记录至缓存区对应位置
            i++;                      // i 前进一位，为下次输入准备
        }
    }
    else
    {
    }

    GUI_Show8StrTextBox(4, 2, StringBuff);
    GUI_Show8StrTextBox(6, 2, StringBuff + 8);

    return 1;
}

void Admin_Mode(void)
{
    Voice_Play(VoiceCmd_INPUT_ADMIN_PASSWORD);

    GUI_DisplayString(2, 8 + 0, &EnterString_16x16[0][0], 2);     // “输入”
    GUI_DisplayString(2, 8 + 32, &AdminString_16x16[0][0], 3);    // “管理员”
    GUI_DisplayString(2, 8 + 80, &PasswordString_16x16[0][0], 2); // “密码”

    if (GUI_Password_Check(SmartLock.AdminPassword, 6) == 0)
    {
        CurrentWindowMode = WindowMode_Setting;
    }
    else
    {
        CurrentWindowMode = WindowMode_User;
    }

    GUI_ClearScreen();

    return;
}

void User_PasswordMode(void)
{
    static u8 ErrorCount = 0;

    if (TimeDisplay == 0)
    {
        GUI_ShowOperationTipRow(0, 2); // 显示"按#确认 按*删除"
    }
    else
    {
        GUI_DispClock();
    }

    GUI_DisplayString(2, 24, &String2_16xN[0][0], 5); // “请输入密码

    if (CurrentWindowMode == WindowMode_AllClear)
    {
        return;
    }
    else
    {
    }

    if (Password_Input() == 0) // 内部调用了OLED_ShowTextBox()用以实时显示输入状况
    {
        if (FakePassword_Check(StringBuff, SmartLock.UserPassword) == 1)
        {
#ifdef DEBUG
            printf("Password right\r\n");
#endif

            Voice_Play(VoiceCmd_DOOROPEN_SUCCESS);
            SmartLock_OpenDoor();
            ErrorCount = 0;
        }
        else
        {
#ifdef DEBUG
            printf("Password wrong\r\n");
#endif

            Voice_Play(VoiceCmd_PASSWORD_INCONFORMITY);
            SmartLock_CloseDoor();
            ErrorCount++;
        }

        memset(StringBuff, 0, sizeof(StringBuff)); // 清空输入缓存区

        if (ErrorCount > 2)
        {
            for (u32 i = 0; i < 10; i++)
            {
                OLED_ShowWaitRow(2, 2);
                OLED_Show_XxN8_Character(2, 80, 2, 8, &(Number_8x16[9 - i][0]));
                TIM2_Delay_ms(1000);
            }
            ErrorCount = 1;
        }
        else
            memset(StringBuff, 0, sizeof(StringBuff)); // 清空输入缓存
    }
    else
    {
    }

    return;
}

void SubMenu_Change(u32 *NextSubMenu, u8 MenuCurrentPlace)
{
    switch (MenuCurrentPlace)
    {
    case SubMenu_PasswordChange - 1:
        *NextSubMenu = SubMenu_PasswordChange;
        break;
    case SubMenu_MuteSetting - 1:
        *NextSubMenu = SubMenu_MuteSetting;
        break;
    case SubMenu_FingerMange - 1:
        *NextSubMenu = SubMenu_FingerMange;
        break;
    case SubMenu_IDCardMange - 1:
        *NextSubMenu = SubMenu_IDCardMange;
        break;
    case SubMenu_ClockSetting - 1:
        *NextSubMenu = SubMenu_ClockSetting;
        break;
    case SubMenu_MemorySetting - 1:
        *NextSubMenu = SubMenu_MemorySetting;
        break;
    default:
        *NextSubMenu = SubMenu_Start;
    }
}

void Menu_Start(u32 *LastMenu)
{
    // 字符串顺序关联用户设置菜单顺序，这里使用的是设置菜单子菜单枚举结构体的顺序
    const u8 *Str[6] =
        {
            &(MenuString1_16x16[0][0]), // “修改用户密码”
            &(MenuString2_16x16[0][0]), // “声音模式”
            &(MenuString3_16x16[0][0]), // “登记指纹”
            &(MenuString4_16x16[0][0]), // “登记卡片”
            &(MenuString5_16x16[0][0]), // “时钟设置”
            &(MenuString6_16x16[0][0]), // “存储管理”
        };
    const u8 StrLenArray[6] = {6, 4, 4, 4, 4, 4};

    static u8 CurrentPlace[6] = {0, 1, 2, 3, 4, 5};

    switch (Menu_Move(CurrentPlace, 6))
    {
    case MenuPlace_Check:
        SubMenu_Change(LastMenu, CurrentPlace[1]);
        GUI_ClearScreen();
        break;
    case MenuPlace_Back:
        GUI_ClearScreen();
        CurrentWindowMode = WindowMode_User;
        break;
    case MenuPlace_Shift:
        GUI_ClearScreen();
        break;
    default:
        break;
    }

    GUI_DisplayString(0, 32, &String3_16xN[0][0], 4); // 居中显示“设置菜单”
    OLED_ShowString(2, 2, 0, Str[CurrentPlace[0]], StrLenArray[CurrentPlace[0]]);
    ReversalFlag = 1;
    OLED_ShowString(4, 2, 0, Str[CurrentPlace[1]], StrLenArray[CurrentPlace[1]]);
    ReversalFlag = 0;
    OLED_ShowString(6, 2, 0, Str[CurrentPlace[2]], StrLenArray[CurrentPlace[2]]);

    if (*LastMenu != SubMenu_Start)
    {
        GUI_ClearScreen();
    }
    else
    {
    }

    return;
}

void Menu_PasswordChange(u32 *LastMenu)
{

    GUI_DisplayString(0, 16, &MenuString1_16x16[0][0], 6);         // 居中显示“修改用户密码”
    GUI_DisplayString(2, 16 + 0, &EnterString_16x16[0][0], 2);     // “输入”
    GUI_DisplayString(2, 16 + 32, &OldString_16x16[0][0], 2);      // “新的”
    GUI_DisplayString(2, 16 + 64, &PasswordString_16x16[0][0], 2); // “密码”

    if (GUI_Password_Check(SmartLock.UserPassword, 6) == 0)
    {
        if (GUI_Password_Enroll(SmartLock.UserPassword, 6) == 0)
        {
            AT24C04_SaveSmartLockData();
        }
        else
        {
        }
    }
    else
    {
    }
    Delay(1000);

    GUI_ClearScreen();

    *LastMenu = SubMenu_Start;
}

void Mute_Setting(u8 MenuElmt)
{
    // 菜单第一项是静音
    if (MenuElmt == 0)
    {
        SmartLock.MuteMode = 1;
        AT24C04_SaveSmartLockData();
    }
    else
    {
        SmartLock.MuteMode = 0;
        AT24C04_SaveSmartLockData();
    }
}

void Menu_MuteSetting(u32 *LastMenu)
{
    const u8 *Str[2] =
        {
            &(MuteString1_16x16[0][0]), // 静音
            &(MuteString2_16x16[0][0])  // 语音
        };
    const u8 StrLenArray[2] = {2, 2};

    static u8 CurrentPlace[2] = {0, 1};

    switch (Menu_Move(CurrentPlace, 2))
    {
    case MenuPlace_Check:
        Mute_Setting(CurrentPlace[0]);
        GUI_ClearScreen();
        break;
    case MenuPlace_Back:
        *LastMenu = SubMenu_Start;
        GUI_ClearScreen();
        break;
    case MenuPlace_Shift:
        GUI_ClearScreen();
        break;
    default:
        break;
    }

    OLED_ShowString(0, 2, 32, &MenuString2_16x16[0][0], 4); // 居中显示“声音模式”

    ReversalFlag = 1;
    OLED_ShowString(CurrentPlace[0] * 2 + 2, 2, 0, Str[CurrentPlace[0]], StrLenArray[CurrentPlace[0]]);
    ReversalFlag = 0;
    OLED_ShowString(CurrentPlace[1] * 2 + 2, 2, 0, Str[CurrentPlace[1]], StrLenArray[CurrentPlace[1]]);

    if (*LastMenu != SubMenu_MuteSetting)
    {
        GUI_ClearScreen();
    }
    else
    {
    }

    return;
}

void Clock_Setting(u8 MenuElmt)
{
    static u32 ClockSubMenu = ClockSubMenu_Father;

    GUI_ClearScreen();
    do
    {
        switch (MenuElmt)
        {
        case ClockSubMenu_Clock - 1: // 菜单第一项是 "修改时间"
            GUI_CLOCK_SettingTime(&ClockSubMenu);
            break;
        case ClockSubMenu_Date - 1:
            GUI_CLOCK_SettingDate(&ClockSubMenu);
            break;
        default:
            ClockSubMenu = ClockSubMenu_Father;
        }
    } while (ClockSubMenu != ClockSubMenu_Father);

    return;
}

void Menu_ClockSetting(u32 *LastMenu)
{
    const u8 *Str[2] =
        {
            &(ClockSettingTimeString_16x16[0][0]), // 修改时间
            &(ClockSettingDateString_16x16[0][0])  // 修改日期
        };
    const u8 StrLenArray[2] = {4, 4};

    static u8 CurrentPlace[2] = {0, 1};

    switch (Menu_Move(CurrentPlace, 2))
    {
    case MenuPlace_Check:
        Clock_Setting(CurrentPlace[0]);
        GUI_ClearScreen();
        break;
    case MenuPlace_Back:
        *LastMenu = SubMenu_Start;
        GUI_ClearScreen();
        break;
    case MenuPlace_Shift:
        GUI_ClearScreen();
        break;
    default:
        break;
    }

    OLED_ShowString(0, 2, 32, &MenuString5_16x16[0][0], 4); // 居中显示“时钟设置”

    ReversalFlag = 1;
    OLED_ShowString(CurrentPlace[0] * 2 + 2, 2, 0, Str[CurrentPlace[0]], StrLenArray[CurrentPlace[0]]);
    ReversalFlag = 0;
    OLED_ShowString(CurrentPlace[1] * 2 + 2, 2, 0, Str[CurrentPlace[1]], StrLenArray[CurrentPlace[1]]);

    if (*LastMenu != SubMenu_ClockSetting)
    {
        GUI_ClearScreen();
    }
    else
    {
    }

    return;
}

void Finger_Mange(u8 MenuElmt)
{
    // 菜单第一项是新增用户
    if (MenuElmt == 0)
    {
        // 新增用户任务
        GUI_Finger_EnrollNewUser();
    }
    else
    {
        // 删除用户任务
        GUI_Finger_EraseAllUser();
    }
}

void Menu_FingerMange(u32 *LastMenu)
{
    const u8 *Str[2] =
        {
            &(UserMangeString1_16x16[0][0]), // 新增用户
            &(UserMangeString2_16x16[0][0])  // 删除用户
        };
    const u8 StrLenArray[2] = {4, 4}; // 记录字符串长度

    static u8 CurrentPlace[2] = {0, 1};

    switch (Menu_Move(CurrentPlace, 2))
    {
    case MenuPlace_Check:
        Finger_Mange(CurrentPlace[0]); // 执行菜单位置记录数组对应选项任务
        GUI_ClearScreen();
        break;
    case MenuPlace_Back:
        *LastMenu = SubMenu_Start;
        GUI_ClearScreen();
        break;
    case MenuPlace_Shift:
        GUI_ClearScreen();
        break;
    default:
        break;
    }

    GUI_DisplayString(0, 32, &MenuString3_16x16[0][0], 4); // 居中显示“登记指纹”

    ReversalFlag = 1;
    OLED_ShowString(CurrentPlace[0] * 2 + 2, 2, 0, Str[CurrentPlace[0]], StrLenArray[CurrentPlace[0]]);
    ReversalFlag = 0;
    OLED_ShowString(CurrentPlace[1] * 2 + 2, 2, 0, Str[CurrentPlace[1]], StrLenArray[CurrentPlace[1]]);

    if (*LastMenu != SubMenu_FingerMange)
    {
        GUI_ClearScreen();
    }
    else
    {
    }

    return;
}

void IDCard_Mange(u8 MenuElmt)
{
    // 菜单第一项是新增用户
    if (MenuElmt == 0)
    {
        // 新增用户任务
        GUI_RFID_AuthorizeNewCard();
    }
    else
    {
        // TODO 删除用户任务
    }
}

void Menu_IDCardMange(u32 *LastMenu)
{
    const u8 *Str[2] =
        {
            &(UserMangeString1_16x16[0][0]), // 新增用户
            &(UserMangeString2_16x16[0][0])  // 删除用户
        };
    const u8 StrLenArray[2] = {4, 4}; // 记录字符串长度

    static u8 CurrentPlace[2] = {0, 1};

    switch (Menu_Move(CurrentPlace, 2))
    {
    case MenuPlace_Check:
        IDCard_Mange(CurrentPlace[0]); // 执行菜单位置记录数组对应选项任务
        break;
    case MenuPlace_Back:
        *LastMenu = SubMenu_Start;
        break;
    case MenuPlace_Shift:
        break;
    default:
        break;
    }

    GUI_DisplayString(0, 32, &MenuString4_16x16[0][0], 4); // 居中显示“登记卡片”

    ReversalFlag = 1;
    OLED_ShowString(CurrentPlace[0] * 2 + 2, 2, 0, Str[CurrentPlace[0]], StrLenArray[CurrentPlace[0]]);
    ReversalFlag = 0;
    OLED_ShowString(CurrentPlace[1] * 2 + 2, 2, 0, Str[CurrentPlace[1]], StrLenArray[CurrentPlace[1]]);

    if (*LastMenu != SubMenu_IDCardMange)
    {
        GUI_ClearScreen();
    }
    else
    {
    }

    return;
}

void Memory_Setting(u8 MenuElmt)
{
    // 菜单第一项是删除
    if (MenuElmt == 0)
    {
        GUI_Memory_Clear();
    }
    else
    {
        GUI_Memory_Enable();
    }
}

void Menu_MemorySetting(u32 *LastMenu)
{
    const u8 *Str[2] =
        {
            &(DeleteString_16x16[0][0]), // “删除”
            &(EnableString_16x16[0][0])  // “启用”
        };
    const u8 StrLenArray[2] = {2, 2};

    static u8 CurrentPlace[2] = {0, 1};

    switch (Menu_Move(CurrentPlace, 2))
    {
    case MenuPlace_Check:
        Memory_Setting(CurrentPlace[0]);
        GUI_ClearScreen();
        break;
    case MenuPlace_Back:
        *LastMenu = SubMenu_Start;
        GUI_ClearScreen();
        break;
    case MenuPlace_Shift:
        GUI_ClearScreen();
        break;
    default:
        break;
    }

    OLED_ShowString(0, 2, 32, &MenuString6_16x16[0][0], 4); // 居中显示“存储管理”

    ReversalFlag = 1;
    OLED_ShowString(CurrentPlace[0] * 2 + 2, 2, 0, Str[CurrentPlace[0]], StrLenArray[CurrentPlace[0]]);
    ReversalFlag = 0;
    OLED_ShowString(CurrentPlace[1] * 2 + 2, 2, 0, Str[CurrentPlace[1]], StrLenArray[CurrentPlace[1]]);

    if (*LastMenu != SubMenu_MemorySetting)
    {
        GUI_ClearScreen();
    }
    else
    {
    }

    return;
}

void Window_AdminMode(void)
{

    Admin_Mode();
    memset(StringBuff, 0, sizeof(StringBuff)); // 清空输入缓存区

    //	GUI_ClearScreen();

    //	CurrentWindowMode = WindowMode_Setting;

    return;
}

void Window_UserMode(void)
{
    // 默认用户子模式 CurrentUserMode = UserSubMode_Password;
    static u8 FirstFlag = 0;

    if (FirstFlag == 0)
    {
        GUI_ClearScreen();
        OLED_ShowPicture(28, 0, 72, 64, &(Logo_72x64[0][0]));
        Delay(1000);
        FirstFlag = 1;
        GUI_ClearScreen();
    }
    else
    {
        switch (CurrentUserMode)
        {
        case UserSubMode_Password:
            User_PasswordMode();
            break;
        case UserSubMode_Finger:
            GUI_Finger_Compare();
            break;
            //			case UserSubMode_RFID:
            //				GUI_RFID_CompareCard();
            //				break;
        default:
            CurrentUserMode = UserSubMode_Password;
        }
    }

    return;
}

void Window_SettingMode(void)
{
    static u32 CurrentMenu = SubMenu_Start;

    switch (CurrentMenu)
    {
    case SubMenu_Start:
        Menu_Start(&CurrentMenu);
        break;
    case SubMenu_PasswordChange:
        Menu_PasswordChange(&CurrentMenu);
        break;
    case SubMenu_MuteSetting:
        Menu_MuteSetting(&CurrentMenu);
        break;
    case SubMenu_FingerMange:
        Menu_FingerMange(&CurrentMenu);
        break;
    case SubMenu_IDCardMange:
        Menu_IDCardMange(&CurrentMenu);
        break;
    case SubMenu_ClockSetting:
        Menu_ClockSetting(&CurrentMenu);
        break;
    case SubMenu_MemorySetting:
        Menu_MemorySetting(&CurrentMenu);
        break;
    default:
        CurrentMenu = SubMenu_Start;
    }

    return;
}

/* Exported functions --------------------------------------------------------*/

void Task_WindowMain(void)
{
    switch (CurrentWindowMode)
    {
    case WindowMode_Admin:
        Window_AdminMode();
        break;
    case WindowMode_User:
        Window_UserMode();
        break;
    case WindowMode_Setting:
        Window_SettingMode();
        break;
    default:
        GUI_ClearScreen();
    }
}

/**
  * @}
  */
