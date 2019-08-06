/**
******************************************************************************
  * @file       uart.c
  * @brief      串口相关操作函数源文件
  * @version    1.0
  * @date       Tue 06-08-2019
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "uart.h"

/** @addtogroup UART
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  USART1初始化
  * @param  BaudRate
  * @return NULL
  */
void USART1_Init(u32 BaudRate)
{
	/* GPIO配置 PA9 USART1_TX PA10 USART1_RX */
	//时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	/* USART1配置 */
	//时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate				= BaudRate;
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity				= USART_Parity_No;
	USART_InitStruct.USART_StopBits				= USART_StopBits_1;
	USART_InitStruct.USART_WordLength			= USART_WordLength_8b;
	
	USART_Init(USART1,&USART_InitStruct);
}

/**
  * @}
  */
