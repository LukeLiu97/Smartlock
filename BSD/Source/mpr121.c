/**
******************************************************************************
  * @file       mpr121.c
  * @brief      电容按键传感器驱动源文件
  * @version    1.1
  * @date       Wed 07-08-2019
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mpr121.h"

/** @addtogroup MPR121 driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MHD_R 0x2B
#define NHD_R 0x2C
#define NCL_R 0x2D
#define FDL_R 0x2E
#define MHD_F 0x2F
#define NHD_F 0x30
#define NCL_F 0x31
#define FDL_F 0x32
#define ELE0_T 0x41
#define ELE0_R 0x42
#define ELE1_T 0x43
#define ELE1_R 0x44
#define ELE2_T 0x45
#define ELE2_R 0x46
#define ELE3_T 0x47
#define ELE3_R 0x48
#define ELE4_T 0x49
#define ELE4_R 0x4A
#define ELE5_T 0x4B
#define ELE5_R 0x4C
#define ELE6_T 0x4D
#define ELE6_R 0x4E
#define ELE7_T 0x4F
#define ELE7_R 0x50
#define ELE8_T 0x51
#define ELE8_R 0x52
#define ELE9_T 0x53
#define ELE9_R 0x54
#define ELE10_T 0x55
#define ELE10_R 0x56
#define ELE11_T 0x57
#define ELE11_R 0x58
#define FIL_CFG 0x5D
#define ELE_CFG 0x5E
#define GPIO_CTRL0 0x73
#define GPIO_CTRL1 0x74
#define GPIO_DATA 0x75
#define GPIO_DIR 0x76
#define GPIO_EN 0x77
#define GPIO_SET 0x78
#define GPIO_CLEAR 0x79
#define GPIO_TOGGLE 0x7A
#define ATO_CFG0 0x7B
#define ATO_CFGU 0x7D
#define ATO_CFGL 0x7E
#define ATO_CFGT 0x7F

#define TOU_THRESH 0x0F
#define REL_THRESH 0x0A

/* Private macro -------------------------------------------------------------*/
#define MPR_write MPR_Write
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void MPR_Write(u8 SlaveAddress, u8 RegisterAddress, u8 Data);

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void MPR121_Init(void)
{
    // Section A
    // This group controls filtering when data is > baseline.
    MPR_write(MPR_ADD, MHD_R, 0x01); //根据快速使用数据手册来配置
    MPR_write(MPR_ADD, NHD_R, 0x01);
    MPR_write(MPR_ADD, NCL_R, 0x00);
    MPR_write(MPR_ADD, FDL_R, 0x00);

    // Section B
    // This group controls filtering when data is < baseline.
    MPR_write(MPR_ADD, MHD_F, 0x01); //第一个是器件地址  访问的寄存器地址   写入的值
    MPR_write(MPR_ADD, NHD_F, 0x01);
    MPR_write(MPR_ADD, NCL_F, 0xFF);
    MPR_write(MPR_ADD, FDL_F, 0x02);

    // Section C
    // This group sets touch and release thresholds for each electrode
    MPR_write(MPR_ADD, ELE0_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE0_R, REL_THRESH);
    MPR_write(MPR_ADD, ELE1_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE1_R, REL_THRESH);
    MPR_write(MPR_ADD, ELE2_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE2_R, REL_THRESH);
    MPR_write(MPR_ADD, ELE3_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE3_R, REL_THRESH);
    MPR_write(MPR_ADD, ELE4_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE4_R, REL_THRESH);
    MPR_write(MPR_ADD, ELE5_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE5_R, REL_THRESH);
    MPR_write(MPR_ADD, ELE6_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE6_R, REL_THRESH);
    MPR_write(MPR_ADD, ELE7_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE7_R, REL_THRESH);
    MPR_write(MPR_ADD, ELE8_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE8_R, REL_THRESH);
    MPR_write(MPR_ADD, ELE9_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE9_R, REL_THRESH);
    MPR_write(MPR_ADD, ELE10_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE10_R, REL_THRESH);
    MPR_write(MPR_ADD, ELE11_T, TOU_THRESH);
    MPR_write(MPR_ADD, ELE11_R, REL_THRESH);
    // Section D
    // Set the Filter Configuration
    // Set ESI2
    MPR_write(MPR_ADD, FIL_CFG, 0x04);

    // Section E
    // Electrode Configuration
    // Enable 6 Electrodes and set to run mode
    // Set ELE_CFG to 0x00 to return to standby mode
    MPR_write(MPR_ADD, ELE_CFG, 0x0C); // Enables all 12 Electrodes
}

void MPR_Write(u8 SlaveAddress, u8 RegisterAddress, u8 Data)
{
    IIC_Start();
    IIC_SendByte((SlaveAddress << 1) + 0);
    IIC_CheckACK();
    IIC_SendByte(RegisterAddress);
    IIC_CheckACK();
    IIC_SendByte(Data);
    IIC_CheckACK();
    IIC_Stop();

    return;
}

u8 MPR_Read(u8 SlaveAddress, u8 RegisterAddress)
{
    u8 Data;

    IIC_Start();
    IIC_SendByte((SlaveAddress << 1) + 0);
    IIC_CheckACK();
    IIC_SendByte(RegisterAddress);
    IIC_CheckACK();

    IIC_Start();
    IIC_SendByte((SlaveAddress << 1) + 1);
    IIC_CheckACK();
    Data = IIC_RecvByte(0);
    IIC_Stop();

    return Data;
}

u16 MPR_TouchStatus(void)
{
    u16 TouchStatus = 0;
    TouchStatus = MPR_Read(MPR_ADD, 0x01) << 8;
    TouchStatus |= MPR_Read(MPR_ADD, 0x00);

    return TouchStatus;
}

void MPR_IRQ_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOB Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* Configure PB3 in  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    return;
}

/**
  * @}
  */
