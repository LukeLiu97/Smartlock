/**
******************************************************************************
  * @file       main.c
  * @brief      主程序源文件
  * @version    1.1
  * @date       Tue 06-08-2019
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
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main function
  * @param  NULL
  * @return NULL
  */
int main(void)
{
	LED_Init();
	
	USART1_Init(115200);
	USART_Cmd(USART1,ENABLE);
	
	Voicemodule_Init();
	delay_ms(500);//等待稳定
	
	LED2_OFF();
	LED3_ON();
	
	Voice_SendData(VoiceCmd_CHANGE_ADMIN_PASSWORD);
	
	while(1)
	{
		Delay(500);
		LED2_ON();
		LED3_ON();
		Delay(500);
		LED2_OFF();
		LED3_OFF();
		
		printf("Hello World!\r\n");
		
		Delay(5000);
		Voice_SendData(VoiceCmd_CHANGE_ADMIN_PASSWORD);
	}
}

/* Exported functions --------------------------------------------------------*/

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
