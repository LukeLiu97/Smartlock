/**
******************************************************************************
  * @file       main.c
  * @brief      主程序源文件
  * @version    1.0
  * @date       Aug-10-2019 Sat
  * @update     添加指纹解锁功能
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup 
  * @{
  */

/* Global varaiables ---------------------------------------------------------*/

u16 gTouchStatus = 0; // 记录每次由MPR121读取到的按键状态

u8 FingerPack[8] 	= {0};
u8 FingerPackCount = 0;
u8 FingerPackOver = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);
static void NVIC_Config(void);
static void RCC_Config(void);

static void Task_AdminCkeck(void);
static void Task_FingerIdentify(void);
static void Task_RFIDIdentify(void);
// static void Task_DoorMange(void);

void AT24C04_CheckWrite0(void);
void AT24C04_CheckWrite1(void);
void AT24C04_CheckWriteHash(void);

void OLED_DrawX(void);
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
	
	NVIC_Config();
	
	RCC_Config();
	
	RTC_Init();
	
	/* IIC 和 OLED 使用JTAG引脚需要重映射 */
	JTAG_Disable();
	
	/* 初始化LED */
	LED_Init();
	
	/* 初始化串口2(仅调试用) */
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
	
	/* 初始化电容按键模块，以及初始化MPR121 IRQ引脚为输入浮空态 */
	Key_Init();
	MPR_IRQ_Init();
	
	/* 管理员判定 */
	Task_AdminCkeck();
	
	/* 指纹模块初始化 */
	Fingerprint_Init();
	
	/* 门锁电机模块初始化 */
	Motor_Init();
	
	/* 射频识别模块使能 */
	RFID_Init();// SPI2 需要提前初始化
	
	/* 电容 按键MPR121 IRQ 引脚 关联外中断3 初始化*/
	EXTI3_Init();
	
	/* 定时器3初始化并使能其上溢中断 */
	TIM3_Interrupt_Init();

	/* 允许全局中断 */
	__set_PRIMASK(0); 
	
	/* 等待系统稳定 */
	Delay(1000);
	
//	AT24C04_CheckWrite0();
//	AT24C04_CheckWrite1();
//	AT24C04_CheckWriteHash();
	
	while(1)
	{
		Task_FingerIdentify();
		Task_RFIDIdentify();
		Task_WindowMain();
	}
	
	/* No Retval */
}


void AT24C04_CheckWrite0(void)
{
	printf("AT24C04_CheckWrite0\r\n");
	u8 ReadTemp;
	for(u32 i = 0;i <= 255 ;i++)
	{
			printf("|%4d W0 %d ",i,AT24C04_WriteByte(i,0x00));
			printf("R0 %d %#0.2x|",AT24C04_ReadByte(i,&ReadTemp),ReadTemp);
			printf("\r\n");
	}
	
	return ;
}
void AT24C04_CheckWrite1(void)
{
	u8 ReadTemp;
	printf("AT24C04_CheckWrite1\r\n");
	for(u32 i = 0;i <= 255 ;i++)
	{
			printf("|%4d W1 %d ",i,AT24C04_WriteByte(i,0xFF));
			printf("R1 %d %#0.2x|",AT24C04_ReadByte(i,&ReadTemp),ReadTemp);
			printf("\r\n");

	}
	
	return ;
}

void AT24C04_CheckWriteHash(void)
{
	u8 ReadTemp;
	printf("AT24C04_CheckWriteHash\r\n");
	for(u32 i = 0;i <= 255 ;i++)
	{
			printf("|%4d WN %d ",i,AT24C04_WriteByte(i,i));
			printf("RN %d %#0.2x|",AT24C04_ReadByte(i,&ReadTemp),ReadTemp);
			printf("\r\n");
		
	}
	
	return ;
}

void RTC_DispClock(void)
{
	static TimeStu ZeroTime = {2019,8,4,8,0,0,0};
	
	TimeStu Time = {0};
	
	if(TimeDisplay == 1)
	{
		Read_RTCTime(&Time);
		
		if(Time.Year != ZeroTime.Year)
		{
			Set_RTCTime(&ZeroTime);
		}
		else
		{
		}
		GUI_ClearPart(0,2,0,128);
		
		GUI_DisplayNumber(0,0,Time.Year,2,2);
		GUI_DisplayNumber(0,16,Time.Month,2,2);
		GUI_DisplayNumber(0,32,Time.Date,2,2);
		GUI_DisplayNumber(0,64,Time.Hour,2,2);
		GUI_DisplayNumber(0,88,Time.Minute,2,2);
		GUI_DisplayNumber(0,112,Time.Second,2,2);
		
		TimeDisplay = 0;
		
		Delay(500);
	}
	else
	{
		
	}
	
}

static void JTAG_Disable(void)
{
	/* 重映射 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);// 复位后才可再次重映射
	
	return ;
}

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

//	/* Configure one bit for preemption priority */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	return ;
}

void RCC_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	 
	PWR_BackupAccessCmd(ENABLE);
	
	return ;
}

static void Task_AdminCkeck(void)
{
	if(Admin_Check() == 1)
	{
		CurrentWindowMode = WindowMode_Admin;
	}
	else
	{
		CurrentWindowMode = WindowMode_User;
	}
}

static void Task_FingerIdentify(void)
{
	if(MG200_DETECT_Status() == SET && CurrentWindowMode != WindowMode_Setting)
	{
		CurrentWindowMode = WindowMode_User;
		CurrentUserMode = UserSubMode_Finger;
		UnBusy_Count = 0;
	}
	else
	{
		CurrentUserMode = UserSubMode_Password;
	}
}

static void Task_RFIDIdentify(void)
{
	if(CurrentWindowMode != WindowMode_Setting)
	{
		GUI_RFID_CompareCard();
	}
	else
	{
		
	}
	
}

//static void Task_DoorMange(void)
//{
//	/* 门状态控制 */
//}

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
