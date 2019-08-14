/**
******************************************************************************
  * @file       main.c
  * @brief      主程序源文件
  * @version    0.6
  * @date       Aug-10-2019 Sat
  * @update     添加OLED Drive
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup 
  * @{
  */

/* Global varaiables ---------------------------------------------------------*/
u16 gTouchStatus = 0; // 记录每次由MPR121读取到的按键状态
u8 CurrentWindowMode = WindowMode_AllClear;

u32 UnBusy_Count = 0;

u8 FingerPack[8] 	= {0};
u8 FingerPackCount = 0;
u8 FingerPackOver = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);

void Fingerprint_NewTask(void);
	
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main function
  * @param  NULL
  * @return NULL
  */
int main(void)
{
	/* 屏蔽全局中断 */
	__set_PRIMASK(1); 
	
	/* IIC 和 OLED 使用JTAG引脚需要重映射 */
	JTAG_Disable();
	
	/* 初始化LED */
	LED_Init();
	
	/* 初始化串口，便于调试 */
	USART1_Init(115200);
#ifdef DEBUG
		printf("\r\nDebug mode\r\n");
#endif		
	
	/* 初始化声音生成模块 */
	VoiceModule_Init();
	
	/* 初始化OLED模块并且清屏 */
	OLED_Init();
	OLED_Config();
	OLED_Clear();
	
	/* 初始化电容按键模块 */
	Key_Init();
	MPR_IRQ_Init();
	
	/* 管理员判定 */
	if(Admin_Check() == 1)
	{
		CurrentWindowMode = WindowMode_Admin;
	}
	else
	{
		CurrentWindowMode = WindowMode_User;
	}
	
	Fingerprint_Init();
	
	/* 按键模块 IRQ Pin Init */
	EXTI3_Init();
	
	/* 定时器3初始化并使能其上溢中断 */
	TIM3_Interrupt_Init();

	/* 允许全局中断 */
	__set_PRIMASK(0); 
	
	/* 等待系统稳定 */
	
	while(1)
	{
//		Fingerprint_NewTask();
		Window_MainTask();
	}
	
	/* No Retval */
}

void JTAG_Disable(void)
{
	/* 重映射 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);// 复位后才可再次重映射
	
	return ;
}



void Fingerprint_NewTask(void)
{
	u8 IDTemp,Result;
	
	if(MG200_DETECT_Status() == SET)
	{
		Finger_EraseAllUser(&Result);
		
		Voice_Play(VoiceCmd_Di);
		IDTemp = 0x00;// 使用自动分配
		if(Finger_EnrollNewUser(IDTemp) == 0)
		{
			printf("Add new user success\r\n");
		}
		else
		{
			printf("Could new a user\r\n");
		}
		
		while(MG200_DETECT_Status() != RESET)
		{
		}
		
		while(MG200_DETECT_Status() != SET)
		{
		}
		
		if(Finger_Compare(&IDTemp) != 0)
		{
			printf("No find user\r\n");
		}
		else
		{
			printf("User ID is %d\r\n",IDTemp);
		}
		
		while(MG200_DETECT_Status() != RESET)
		{
		}
		
//		if(Finger_CaptureAndExtract(3) == 0)
//		{
//			Voice_Play(VoiceCmd_Di);
//			
//			
//		}
//		else
//		{
//		}
		
	}
	else
	{
		
	}
}


/* Exported functions --------------------------------------------------------*/

u8 Admin_Check(void)
{
	u32 i = 0;

	Delay(100);
	printf("MPR_IRQ = %d\r\n",MPR_IRQ());
	gTouchStatus = MPR_TouchStatus();
	printf("TouchStatus = %#x\r\n",gTouchStatus);
	
	while(1)
	{
		Delay(100);
		gTouchStatus = MPR_TouchStatus();
		printf("TouchStatus = %#x\r\n",gTouchStatus);
		if(i>25 || (gTouchStatus != 0x80f && gTouchStatus != 0x8))
		{
			break;
		}
		i++;
	}
	
	while(MPR_IRQ() == RESET)
	{
		gTouchStatus = MPR_TouchStatus();
	}
	
	printf("Out the while,i = %d\r\n",i);
	
	if(i < 20)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


//Redirect fputc function
int fputc(int ch,FILE *stream)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}
	
	return ch;
}
    
/**
  * @}
  */
