/**
******************************************************************************
  * @file       voice.c
  * @brief      声音模块相关操作源文件
  * @version    1.1
  * @date       Wed 07-08-2019
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "voice.h"

/** @addtogroup voice module
  * @{
  */

/* Global varaiables ---------------------------------------------------------*/
//u8 MuteMode = 1; // 记录每次由MPR121读取到的按键状态

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void VoiceModule_Init(void)
{
    /* GPIO 初始化 VOICE_DATA PB0 VOICE_BUSY PB1 */
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_StructInit(&GPIO_InitStruct);

    /* BUSY PB1 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* PB0 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* 拉低VOICE_DATA，清除状态 */
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);

    return;
}

void Voice_Play(VoiceCmd_TypeDef VoiceCmd)
{
    if (SmartLock.MuteMode == 1)
    {
    }
    else
    {
        /* 等待空闲总线 */
        while ((GPIOB->IDR & (0x1 << 1)) != 0)
        {
        }

        /* 同步头 */
        GPIO_SetBits(GPIOB, GPIO_Pin_0);
        delay_ms(8);
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);
        delay_ms(1);

        /* 发送数据 */
        for (u32 i = 0; i < 8; i++)
        {
            // MSB
            if ((VoiceCmd & (0x1 << (7 - i))) != 0)
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_0);
                delay_us(1500);
                GPIO_ResetBits(GPIOB, GPIO_Pin_0);
                delay_us(500);
            }
            else
            {
                GPIO_SetBits(GPIOB, GPIO_Pin_0);
                delay_us(500);
                GPIO_ResetBits(GPIOB, GPIO_Pin_0);
                delay_us(1500);
            }
        }
    }

    return;
}

/**
  * @}
  */
