/**
******************************************************************************
  * @file       led.c
  * @brief      LED相关函数源文件
  * @version    1.0
  * @date       Tue 06-08-2019
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "led.h"
 
/** @addtogroup 
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
  * @brief  LED初始化函数
  * @param  NULL
  * @return NULL
  */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOB Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* Configure PB6 and PB8 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Reset pin */
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	
	return ;
}


/**
  * @}
  */
