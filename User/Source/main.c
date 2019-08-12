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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern u8 Password[7]; // 初始密码
extern u8 StringBuff[BUFF_LENGTH];

/* Private function prototypes -----------------------------------------------*/
static void JTAG_Disable(void);

extern void OLED_ShowTextBox(u8 CowNumber,u8 CowHeight,u8 *Str);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main function
  * @param  NULL
  * @return NULL
  */
int main(void)
{
	/* 禁止外部中断 */
	NVIC_DisableIRQ(EXTI3_IRQn);
	
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
	
	/* 等待系统稳定 */
	
	while(1)
	{
		Window_MainTask(&CurrentWindowMode);
		Delay(1000);
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

void Password_Input(void)
{
	u32 i;
	u8 KeyValue;
	
	for(i = 0; i < (BUFF_LENGTH - 1) ; )
	{
		LED3_OFF();
		
		KeyValue = Key_Scan();
			
		if(KeyValue != 0)/* 电容模块检测到按键变化 */
		{
#ifdef DEBUG
			printf("KeyValue = %c\r\n",KeyValue);
#endif
			
			Voice_Play(VoiceCmd_Di);
			
			if(KeyValue == '#')
			{
				break;
			}
			else if(KeyValue == '*')
			{
				if(i == 0)
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
				StringBuff[i] = KeyValue;// 当前输入被记录至缓存区对应位置
				i++; // i 前进一位，为下次输入准备
			}
			
			
		}
		else
		{
		}
		
		OLED_ShowTextBox(4,2,StringBuff);
		OLED_ShowTextBox(6,2,StringBuff + 8);
	}
	StringBuff[i] = 0;
	
	return ;
}

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
