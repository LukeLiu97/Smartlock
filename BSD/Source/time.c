/**
******************************************************************************
  * @file       time.c
  * @brief      简单计时函数源文件
  * @version    2.0
  * @date       Sun 18-08-2019
  * @update     为systick定时任务们添加共享锁定以及中断现场保存
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "time.h"

/** @addtogroup 
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static u8 SystickLock = 0;
static u8 SystickValueSave = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  简单延时函数
  * @note   72MHz
  * @param  延时时间
  * @return NULL
  */
void Delay(u32 t)
{
    while (t--)
    {
        for (u32 i = 0; i < 100; i++)
        {
            for (u32 j = 0; j < 100; j++)
            {
            }
        }
    }

    return;
}

void delay_ms(u32 xms)
{
    if (SystickLock == 1)
    {
        if (SystickValueSave == 0)
        {
            SystickValueSave = SysTick->LOAD - SysTick->VAL;
        }
        else
        {
        }

        SysTick->VAL = 0;
        SysTick->LOAD = (72000 / 8) * xms;
        SysTick->CTRL |= (1 << 0); //开启计时
        while (!(SysTick->CTRL & (1 << 16)))
            ;
        SysTick->CTRL &= ~(1 << 0); //禁止计时

        SysTick->VAL = 0;
        SysTick->LOAD = SystickValueSave + 1;
        SysTick->CTRL |= (1 << 0); //开启计时

        return;
    }
    else
    {
        SystickLock = 1;

        SysTick->VAL = 0;
        SysTick->LOAD = (72000 / 8) * xms;
        SysTick->CTRL |= (1 << 0); //开启计时
        while (!(SysTick->CTRL & (1 << 16)))
            ;
        SysTick->CTRL &= ~(1 << 0); //禁止计时

        SystickValueSave = 0;

        SystickLock = 0;
    }

    return;
}

void delay_us(u32 xus)
{
    //

    if (SystickLock == 1)
    {
        if (SystickValueSave == 0)
        {
            SystickValueSave = SysTick->LOAD - SysTick->VAL;
        }
        else
        {
        }

        SysTick->VAL = 0;
        SysTick->LOAD = (72 / 8) * xus;
        SysTick->CTRL |= (1 << 0); //开启计时
        while (!(SysTick->CTRL & (1 << 16)))
            ;
        SysTick->CTRL &= ~(1 << 0); //禁止计时

        SysTick->VAL = 0;
        SysTick->LOAD = SystickValueSave + 1;
        SysTick->CTRL |= (1 << 0); //开启计时

        return;
    }
    else
    {
        SystickLock = 1;

        SysTick->VAL = 0;
        SysTick->LOAD = (72 / 8) * xus;
        SysTick->CTRL |= (1 << 0); //开启计时
        while (!(SysTick->CTRL & (1 << 16)))
            ;
        SysTick->CTRL &= ~(1 << 0); //禁止计时

        SystickValueSave = 0;

        SystickLock = 0;

        return;
    }
}

/**
  * @}
  */
