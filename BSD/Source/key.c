/**
******************************************************************************
  * @file       key.c
  * @brief      按键相关函数源文件
  * @version    1.1
  * @date       Thu 08-08-2019
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "key.h"

/** @addtogroup key module
  * @{
  */

/* Extern variables ----------------------------------------------------------*/
extern u16 gTouchStatus;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TK0 0x0001  //ELEO
#define TK1 0x0002  //ELE1
#define TK2 0x0004  //ELE2
#define TK3 0x0008  //ELE3
#define TK4 0x0010  //ELE4
#define TK5 0x0020  //ELE5
#define TK6 0x0040  //ELE6
#define TK7 0x0080  //ELE7
#define TK8 0x0100  //ELE8
#define TK9 0x0200  //ELE9
#define TK10 0x0400 //ELE10
#define TK11 0x0800 //ELE11

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void EXTI3_Init(void)
{
    /*  */
    EXTI_InitTypeDef EXTI_InitStructure;

    /* Connect EXTI3 Line to PB.03 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);

    /* Configure EXTI3 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    return;
}

/* Exported functions --------------------------------------------------------*/
void EXTI3_Disable(void)
{
    /*  */
    EXTI_InitTypeDef EXTI_InitStructure;

    /* Enable AFIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* Connect EXTI3 Line to PB.03 pin */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);

    /* Configure EXTI3 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*  */
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable and set EXTI3 Interrupt to the highest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);

    return;
}

void Key_Init(void)
{
    /* 初始化软件IIC */
    IIC_Software_Init();

    /* 按键模块初始化 */
    MPR121_Init();

    return;
}

u8 Key_Scan(void)
{
    u8 KeyValue = 0;

    switch (gTouchStatus)
    {
    case TK0:
        KeyValue = '1';
        break;
    case TK1:
        KeyValue = '4';
        break;
    case TK2:
        KeyValue = '7';
        break;
    case TK3:
        KeyValue = '*';
        break;
    case TK4:
        KeyValue = '5';
        break;
    case TK5:
        KeyValue = '2';
        break;
    case TK6:
        KeyValue = '8';
        break;
    case TK7:
        KeyValue = '0';
        break;
    case TK8:
        KeyValue = '#';
        break;
    case TK9:
        KeyValue = '9';
        break;
    case TK10:
        KeyValue = '6';
        break;
    case TK11:
        KeyValue = '3';
        break;
    default:
        KeyValue = 0;
    }
    gTouchStatus = 0;

    return KeyValue;
}

/**
  * @}
  */
