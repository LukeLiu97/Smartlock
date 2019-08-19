/**
******************************************************************************
  * @file       spi.c
  * @brief      STM32SPI≈‰÷√
  * @version    1.1
  * @date       Aug-18-2019 Sun
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/** @addtogroup SPI Drive
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /* 
		GPIO ∏¥”√Œ™ SPI2
		SPI_MOSI PB15 SPI_MISO PB14 SPI_SCLK PB13 
	*/

    /* Configure PB13 and PB14 , PB15 in PP mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure PB14 in IF mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* SPI2 mode Setting */
    SPI_InitTypeDef SPI_InitStructure;

    /* SPI2 configuration ------------------------------------------------------*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    /* SPI2 enable -------------------------------------------------------------*/
    SPI_Cmd(SPI2, ENABLE);
}

/**
  * @}
  */
