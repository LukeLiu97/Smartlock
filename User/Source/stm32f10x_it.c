/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Extern variables ----------------------------------------------------------*/
//extern u16 gTouchStatus;
//extern u8 CurrentWindowMode;
//extern u32 UnBusy_Count;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SHOW_TOUCHSTATUS

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		
		/* Interrupt task */
		LED3_ON();
		
		if(CurrentWindowMode == WindowMode_AllClear)
		{
			CurrentWindowMode = WindowMode_User;
		}
		else
		{
		}
		UnBusy_Count = 0;
		
		gTouchStatus = MPR_TouchStatus();

#ifdef DEBUG
		printf("gTouchStatus = %#X \r\n",gTouchStatus);
#endif
		
		if(gTouchStatus == 0)
		{
#if (defined DEBUG) && (defined SHOW_TOUCHSTATUS)
			printf("gTouchStatus = %#X\r\n",gTouchStatus);
			printf("Key release\r\n");
#endif
		}
		else
		{
#if (defined DEBUG) && (defined SHOW_TOUCHSTATUS)		
			printf("gTouchStatus = %#X\r\n",gTouchStatus);
#endif
		}
		
		MPR_TouchStatus();
#ifdef DEBUG		
		printf("gTouchStatus = %#X\r\n",gTouchStatus);
#endif
		/* Clear the  EXTI line 3 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line3);
		
	}
	else
	{
	}
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == SET)
	{
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);
		
		/* Intertupt task */
		LED2_OR();
		
		if(UnBusy_Count > 12 && CurrentWindowMode != WindowMode_Setting)
		{
			CurrentWindowMode = WindowMode_AllClear;
						
			if(UnBusy_Count > 30)
			{
#ifdef DEBUG
				printf("Free status\r\n");
#endif
				UnBusy_Count = 20;
			}
#ifdef DEBUG
			else if(UnBusy_Count > 15)
			{

				printf("Free status\r\n");

			}
#endif
			else
			{
			}
		}
		else
		{
			UnBusy_Count ++;
		}
		
		
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == SET)
	{
		FingerPack.Data[FingerPack.Count++] = USART_ReceiveData(USART2);
		
		if(FingerPack.Data[0] != 0x6C)// 接受起始码不吻合
		{
			FingerPack.Count = 0;
		}
		else
		{
			 if(FingerPack.Count == 8)
			 {
				 FingerPack.Count = 0;
				 FingerPack.Over = 1;
			 }
		}
	}
	else
	{
		
	}
}

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
	static u32 Count = 0;
	
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
	/* Clear the RTC Second interrupt */
	RTC_ClearITPendingBit(RTC_IT_SEC);

	/* Interrupt Task */
		
	if(Count >= 3)
	{
		Count = 0;
	}
	else if(Count >= 2)
	{
		TimeDisplay = 1;
		Count++;
	}
	else
	{
		TimeDisplay = 0;
		Count++;
	}

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	}
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
