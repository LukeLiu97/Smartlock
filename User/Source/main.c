/**
******************************************************************************
  * @file       main.c
  * @brief      主程序源文件
  * @version    2.3
  * @date       Aug-10-2019 Sat
  * @update     添加指纹解锁功能
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);
static void NVIC_Config(void);
static void RCC_Config(void);

static void Task_MemoryCheck(void);
static void Task_AdminCkeck(void);
static void Task_FingerIdentify(void);
static void Task_RFIDIdentify(void);
static void Task_DoorMange(void);
static void Task_MemoryData(void);

static u8 Admin_Check(void);
static void IWWG_Init(void);

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
	
	/* 中断优先级配置 */
	NVIC_Config();
	
	/* 外设时钟使能 */
	RCC_Config();
	
	/* 实时时钟使能 */
	RTC_Init();
	
	/* 重映射JTAG */
	JTAG_Disable();// IIC 和 OLED 占用了JTAG引脚
	
	/* 初始化LED */
	LED_Init();
	
#ifdef DEBUG	
	/* 初始化串口2(仅调试用) */
	USART1_Init(115200);

	printf("\r\nDebug mode\r\n");
#endif		
    /* UART2 初始化 */
	USART2_Init(115200);

	/* 初始化声音生成模块 */
	VoiceModule_Init();
	
	/* 初始化SP2 */
	SPI2_Init();
	
	/* 初始化OLED模块并且清屏 */
	OLED_Init();
	OLED_Config();
	OLED_Clear();
	
	/* 初始化电容按键模块，以及初始化MPR121 IRQ引脚为输入浮空态 */
	Key_Init();
	MPR_IRQ_Init();
		
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
	
	/* EEPROM启用判断 */
	Task_MemoryCheck();
	
	/* 管理员判定 */
	Task_AdminCkeck();
	
	/* 初始化独立看门狗 */
	IWWG_Init();
	
	/* 允许全局中断 */
	__set_PRIMASK(0); 
	
	/* 等待系统稳定 */
	Delay(1000);
	
	while(1)
	{
		Task_MemoryData();
		Task_FingerIdentify();
		Task_RFIDIdentify();
		Task_WindowMain();
		Task_DoorMange();
		__WFI();
	}
	
	/* No Retval */
}

static void JTAG_Disable(void)
{
	/* 重映射 */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);// 复位后才可再次重映射
	
	return ;
}

static void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure two bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable and set EXTI3 Interrupt to the highest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	return ;
}

static void RCC_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	 
	PWR_BackupAccessCmd(ENABLE);
	
	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* GPIOB Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* GPIC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	/* USART1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* USART2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* SPI2 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	return ;
}

static void Task_MemoryCheck(void)
{
	u8 ReadData1,ReadData2;
	
	if(AT24C04_ReadBuff(EEPROM_ENABLE_BYTE,&ReadData1,1) != 0 || AT24C04_ReadBuff(EEPROM_ENABLE_BYTE + EEPROM_OFFSET,&ReadData2,1) != 0)
	{
		SmartLock.MemoryFlag = RESET;
	}
	else
	{
		/* 数据不一致 */
		if(ReadData1 != ReadData2 || ReadData1 != EEPROM_ENABLE_FLAG)
		{
			SmartLock.MemoryFlag = RESET;
		}
		else
		{
			SmartLock.MemoryFlag = SET;
		}
	}
	
#ifdef DEBUG
	printf("EEPROM Enable = %#x\r\n",SmartLock.MemoryFlag);
#endif
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

static void Task_DoorMange(void)
{
	/* 门状态控制 */
	static u8 LockFlag = 0x01;
	
	if(LockFlag != SmartLock.LockFlag)
	{
		LockFlag = SmartLock.LockFlag;
		
		if(SmartLock.LockFlag == 0x00)
		{
			Motor_OpenLock();
		}
		else
		{
			Motor_CloseLock();
		}
	}
	else
	{
		
	}
}

static void Task_MemoryData(void)
{
	
	u8 Data[16] = {0};
	
	if(SmartLock.MemoryFlag == SET)
	{
		if(AT24C04_ReadBuff(EEPROM_ENABLE_BYTE,Data,16) == 0)
		{
			if(HashCompare(Data,Data+EEPROM_OFFSET,EEPROM_OFFSET) == 0)
			{
				SmartLock.MuteMode = *(Data+EEPROM_MuteMode_Addr);
				memcpy(SmartLock.UserPassword,Data+EEPROM_UserPassword_Addr,sizeof(SmartLock.UserPassword[0]) * 6);
			}
			else
			{
				SmartLock.MemoryFlag = RESET;
			}
		}
		else
		{
		}
	}
	else
	{
		
	}
	
	return ;
}

static u8 Admin_Check(void)
{
	u32 i = 0;

	NVIC_DisableIRQ(EXTI3_IRQn);

	Delay(100);
#ifdef DEBUG
	printf("MPR_IRQ = %d\r\n",MPR_IRQ());
#endif	
	
	gTouchStatus = MPR_TouchStatus();
	
#ifdef DEBUG	
	printf("TouchStatus = %#x\r\n",gTouchStatus);
#endif	
	
	while(1)
	{
		Delay(100);
		gTouchStatus = MPR_TouchStatus();
#ifdef DEBUG
		printf("TouchStatus = %#x\r\n",gTouchStatus);
#endif	
		if(i>20 || (gTouchStatus != 0x80f && gTouchStatus != 0x8))
		{
			break;
		}
		i++;
	}
	
	// 等待MPR121的引脚恢复高电平以防止与中断按键读取的规则冲突
	do
	{
		gTouchStatus = MPR_TouchStatus();
#ifdef DEBUG
		printf("TouchStatus = %#x\r\n",gTouchStatus);
#endif	
	}while(MPR_IRQ() != SET);
	
	if(gTouchStatus != 0x80f)
	{
		gTouchStatus = MPR_TouchStatus();
	}
	else
	{
	}
	
	NVIC_EnableIRQ(EXTI3_IRQn);

#ifdef DEBUG	
	printf("TouchStatus = %#X\r\n",gTouchStatus);
#endif	

#ifdef DEBUG
	printf("Out the while,i = %d\r\n",i);
#endif	
	
	if(i < 15)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static void IWWG_Init(void)
{
	__IO uint32_t LsiFreq = 40000;
	
	/* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
		dispersion) */
	/* Enable write access to IWDG_PR and IWDG_RLR registers */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

	/* IWDG counter clock: LSI/256 */
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
  /* Set counter reload value to obtain 250ms IWDG TimeOut. */
	IWDG_SetReload(LsiFreq/32);

	/* Reload IWDG counter */
	IWDG_ReloadCounter();

	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();
}

/* Exported functions --------------------------------------------------------*/

#ifdef DEBUG
//Redirect fputc function
int fputc(int ch,FILE *stream)
{
	USART_SendData(USART1,ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}
	
	return ch;
}

#endif
    
/**
  * @}
  */
