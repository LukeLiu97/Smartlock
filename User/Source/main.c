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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);

//extern void OLED_ShowTextBox(u8 CowNumber,u8 CowHeight,u8 *Str);


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
	USART_Cmd(USART1,ENABLE);
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
	
	/* 按键模块 IRQ Pin Init */
	EXTI3_Init();
	
	/* 定时器3初始化并使能其上溢中断 */
	TIM3_Interrupt_Init();

	/* 允许全局中断 */
	__set_PRIMASK(0); 
	
	/* 等待系统稳定 */
	
	while(1)
	{
		Window_MainTask();
		// Delay(1000);
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
