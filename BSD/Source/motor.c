/**
******************************************************************************
  * @file       motor.c
  * @brief      电机驱动源文件
  * @version    1.1
  * @date       Aug-14-2019 Wed
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "motor.h"

/** @addtogroup Motor control drive
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
  * @brief  电机控制初始化函数
  * @param  NULL
  * @return NULL
  */
void Motor_Init(void)
{
    /* MOTOR_IA PA5 MOTOR_IB PA4 */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure PA4 and PA5 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始拉低
    MOTOR_IA_L();
    MOTOR_IB_L();

    return;
}

void Motor_CloseLock(void)
{
    MOTOR_IA_H();
    MOTOR_IB_L();

    Delay(500);

    MOTOR_IA_L();
    MOTOR_IB_L();
}

void Motor_OpenLock(void)
{
    MOTOR_IA_L();
    MOTOR_IB_H();

    Delay(500);

    MOTOR_IA_L();
    MOTOR_IB_L();
}

/**
  * @}
  */
