/**
******************************************************************************
  * @file       ui.c
  * @brief      User Interface
  * @version    1.0
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

u8 Password[7] = "201988"; // 初始密码
u8 StringBuff[BUFF_LENGTH] = {0}; // 输入缓冲区

/* Private function prototypes -----------------------------------------------*/

extern void Password_Input(void);

extern u8 Admin_Check(void);

static void Admin_Mode(void);
static void User_Mode(void);

static void Password_Input(void);
static u8 Password_Check(void);
static s32 String_ViolentMatch(const u8 *TargetString,const u8 *MatchingString);

void OLED_Show1stCow(u8 CowNumber,u8 CowHeight);// 显示文字 按#确认 按*取消
void OLED_Show2ndCow(u8 CowNumber,u8 CowHeight);// 显示文字 请输入密码
void OLED_ShowTextBox(u8 CowNumber,u8 CowHeight,u8 *Str);

void OLED_ShowMenuCow(u8 CowNumber,u8 CowHeight);

/* Private functions ---------------------------------------------------------*/

void OLED_Show1stCow(u8 CowNumber,u8 CowHeight)
{
	u32 j[] = {2,1,2,2,1,1,2,1,2,2}; // 记录字宽的序列
	u32 k = 0; // 记录对应字体位置
	
	for(u32 i = 0;i < 10 ;i++) // 显示10个字
	{
		OLED_Show_XxN8_Character(CowNumber,k * 8,CowHeight,j[i] * 8,&(String1_16xN[k][0]));
		k += j[i];
	}
	
	return ;
}

void OLED_Show2ndCow(u8 CowNumber,u8 CowHeight)
{
	for(u32 j = 0;j < 10;j+=2)
	{
		OLED_Show_XxN8_Character(CowNumber,(j + 3) * 8,CowHeight,16,&(String2_16xN[j][0]));
	}
	return ;
}

void OLED_ShowTextBox(u8 CowNumber,u8 CowHeight,u8 *Str)
{
	for(u32 i = 0; i < 8; i++)
	{
		if(Str[i] == 0) // 传入的字符序列对应字符值为0
		{
			// 显示‘_’
			OLED_Show_XxN8_Character(CowNumber,(i * 16) + 4,CowHeight,8,&Char_8x16[0][0]);
		}
		else // 传入的字符序列对应字符值非0
		{
			// 显示‘*’
			OLED_Show_XxN8_Character(CowNumber,(i * 16) + 4,CowHeight,8,&Char_8x16[1][0]);
		}
	}
	return ;
}

void OLED_ShowMenuCow(u8 CowNumber,u8 CowHeight)
{
	
	for(u32 i = 0;i < 4 ;i++) // 显示10个字
	{
		OLED_Show_XxN8_Character(CowNumber,(i + 2) * 16,CowHeight,16,&(String3_16xN[2*i][0]));
	}
	
	return ;
}



u8 Password_Check(void)
{
//	if(String_Compare(StringBuff,Password) == 0)
//	{
//		return 1;
//	}
//	
//	return 0;
	
	s32 Place;
	Place = String_ViolentMatch(StringBuff,Password);
	
#ifdef DEBUG
	printf("Place = %d\r\n",Place);
#endif
	
	if(Place == -1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	
}

s8 String_Compare(const u8 *String1,const u8 *String2)
{
	while(*String1 != '\0'){
        if(*String1 < *String2){
            return -1;
        }
        else if(*String1 > *String2){
            return 1;
        }
        else{}
        String1++;
        String2++;
    }
    if(*String2 != '\0'){
        return -1;
    }
    else{
        return 0;
    }
}

s32 String_ViolentMatch(const u8 *TargetString,const u8 *MatchingString)
{
	u32 T_Length = strlen((const char *)TargetString);
	u32 M_Length = strlen((const char *)MatchingString);
	
	u32 i,j;
	
	if((T_Length < M_Length) ||  (T_Length > 0xFFFFFFF) || (M_Length > 0xFFFFFFF))
	{
		return -1;// 目标字符串长度过短。
	}
	else
	{
		for(i = 0;i < (T_Length - M_Length + 1);i++)
		{
			for(j = 0;j < M_Length; j++)
			{
				if(TargetString[i + j] != MatchingString[j])//对应位置不匹配，从下一位置重新开始匹配
				{
					break;
				}
				else// 继续匹配
				{
				}
			}
			if(j == M_Length)// 完全匹配
			{
				break;
			}
			else// 失配
			{
			}
		}
		
		if(i > (T_Length - M_Length))// 最后位置依然不匹配
		{
			return -1;
		}
		else// 匹配成功，返回对应目标字符串匹配位置
		{
			return i;
		}
	}
}

void Admin_Mode(void)
{
	OLED_ShowMenuCow(0,2);
	OLED_Show2ndCow(2,2);
	
	Voice_Play(VoiceCmd_INPUT_ADMIN_PASSWORD);
	
	Password_Input();
	
	if(Password_Check() == 1)
	{
		printf("Password right\r\n");
		Voice_Play(VoiceCmd_SETTING_SUCCESS);
	}
	else
	{
		printf("Password wrong\r\n");
		Voice_Play(VoiceCmd_PASSWORD_INCONFORMITY);
	}

}

void User_Mode(void)
{
	// TODO 密码输入界面
	
	OLED_Show1stCow(0,2);
	OLED_Show2ndCow(2,2);
	
	// TODO 依据按键输入情况处理缓冲区 + 刷新密码输入区
	
	Password_Input(); // 内部调用了OLED_ShowTextBox()用以实时显示输入状况
	
	
#ifdef DEBUG
	printf("StringBuff = \"%s\"\r\n",StringBuff);
#endif		
	
	if(Password_Check() == 1)
	{
		printf("Password right\r\n");
		Voice_Play(VoiceCmd_DOOROPEN_SUCCESS);
	}
	else
	{
		printf("Password wrong\r\n");
		Voice_Play(VoiceCmd_PASSWORD_INCONFORMITY);
	}
}

void Window_AdminMode(void)
{
	Admin_Mode();
	
	return ;
}

void Window_UserMode(void)
{
	static u8 FirstFlag = 0;
	
	if(FirstFlag == 0)
	{
		OLED_Clear();
		OLED_ShowPicture(28,0,72,64,&(Logo_72x64[0][0]));
		Delay(1000);
		FirstFlag = 1;
	}
	else
	{
		OLED_Clear();
		User_Mode();
	}
	
	return ;
}


/* Exported functions --------------------------------------------------------*/

void Window_MainTask(u8 *WindowMode)
{
//	switch(*WindowMode)
//	{	
//		case WindowMode_Admin:
//			Window_AdminMode();
//			*WindowMode = WindowMode_User;
//			memset(StringBuff,0,sizeof(StringBuff));// 清空输入缓存区
//			OLED_Clear();
//			break;
//		case WindowMode_User:
//			User_Mode();
//			memset(StringBuff,0,sizeof(StringBuff));// 清空输入缓存区
//			OLED_Clear();
//			break;
//		default:
//			OLED_Clear();
//		
//	}
	
	Password_Input();

}

/**
  * @}
  */
