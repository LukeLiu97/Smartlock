/**
******************************************************************************
  * @file       oled.c
  * @brief      
  * @version    1.1
  * @date       Aug-09-2019 Fri
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "oled.h"

/** @addtogroup SPI peri
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

#define OLED_DC_H() (GPIO_SetBits(GPIOA, GPIO_Pin_11))
#define OLED_DC_L() (GPIO_ResetBits(GPIOA, GPIO_Pin_11))
#define OLED_RES_H() (GPIO_SetBits(GPIOA, GPIO_Pin_12))
#define OLED_RES_L() (GPIO_ResetBits(GPIOA, GPIO_Pin_12))
#define OLED_CS_H() (GPIO_SetBits(GPIOA, GPIO_Pin_15))
#define OLED_CS_L() (GPIO_ResetBits(GPIOA, GPIO_Pin_15))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void OLED_Init(void)
{
    /* SPI2 Init */
    // 缺省

    /* OLED_CS PA15 (需要重映射) OLED_DC PA11 OLED_RES PA12 */

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure PA11 and PA12 , PA15 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* GPIO准备 */
    OLED_CS_H();
    OLED_RES_H();

    return;
}

u8 SPI_Write_Read(u8 SendData)
{
    u8 RecvData;

    /* 等待发送寄存器为空 */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
    {
    }
    /* 发送数据 */
    SPI_I2S_SendData(SPI2, SendData);

    /* 等待接受寄存器非空 */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
    {
    }
    /* 接收数据 */
    RecvData = SPI_I2S_ReceiveData(SPI2);

    return RecvData;
}

void OLED_SendCmd(u8 Cmd)
{
    /* 选择OLED 为从设备 */
    OLED_CS_L();

    /* 指示OLED读取命令 */
    OLED_DC_L();

    SPI_Write_Read(Cmd);

    /* 放弃总线控制权 */
    OLED_CS_H();

    return;
}

void OLED_SendData(u8 Data)
{
    /* 选择OLED 为从设备 */
    OLED_CS_L();

    /* 指示OLED读取数据 */
    OLED_DC_H();

    SPI_Write_Read(Data);

    /* 放弃总线控制权 */
    OLED_CS_H();

    return;
}

void OLED_Config(void)
{
    OLED_RES_L();
    delay_ms(100);
    OLED_RES_H();
    delay_ms(100); //复位

    OLED_SendCmd(0xAE); //关闭显示
    OLED_SendCmd(0xD5); //设置时钟分频因子,震荡频率
    OLED_SendCmd(80);   //[3:0],分频因子;[7:4],震荡频率
    OLED_SendCmd(0xA8); //设置驱动路数
    OLED_SendCmd(0X3F); //默认0X3F(1/64)
    OLED_SendCmd(0xD3); //设置显示偏移
    OLED_SendCmd(0X00); //默认为0

    OLED_SendCmd(0x40); //设置显示开始行 [5:0],行数.

    OLED_SendCmd(0x8D); //电荷泵设置
    OLED_SendCmd(0x14); //bit2，开启/关闭
    OLED_SendCmd(0x20); //设置内存地址模式
    OLED_SendCmd(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    OLED_SendCmd(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
    OLED_SendCmd(0xC8); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    OLED_SendCmd(0xDA); //设置COM硬件引脚配置
    OLED_SendCmd(0x12); //[5:4]配置

    OLED_SendCmd(0x81); //对比度设置
    OLED_SendCmd(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
    OLED_SendCmd(0xD9); //设置预充电周期
    OLED_SendCmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
    OLED_SendCmd(0xDB); //设置VCOMH 电压倍率
    OLED_SendCmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_SendCmd(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    OLED_SendCmd(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示
    OLED_SendCmd(0xAF); //开启显示

    return;
}

void OLED_SetLocation(u8 x, u8 y) //设置显示位置 x 时列（0-127列）  y是行（0-7）一行8个点
{
    OLED_SendCmd(0XB0 + y); //设置行Y  0-7 行

    OLED_SendCmd(0X00 + (x & 0X0F));        //低4位
    OLED_SendCmd(0X10 + ((x & 0XF0) >> 4)); //高4位
}

//OLED画点
void OLED_Draw_Point(u8 x, u8 y) //x : 0-127  y : 0-63
{
    OLED_SendCmd(0x00 + x);                  //发送命令，设置Y轴坐标
    OLED_SendData(0x10 + ((x & 0x0F) >> 4)); //发送坐标高4位
    OLED_SendData(0x00 + (x & 0x0F));        //发送坐标低4位

    OLED_SendCmd(0xB0 + y);                  //发送命令，设置Y轴坐标
    OLED_SendData(0x10 + ((y & 0x0F) >> 4)); //发送坐标高4位
    OLED_SendData(0x00 + (y & 0x0F));        //发送坐标低4位

    return;
}

//清屏
void OLED_Clear(void)
{
    u8 i, j;
    for (j = 0; j < 8; j++)
    {
        OLED_SetLocation(0, j);

        for (i = 0; i < 128; i++)
        {
            OLED_SendData(0x00);
        }
    }

    return;
}

void OLED_ClearPart(u8 StartRowNum, u8 Height, u8 StartColumn, u8 Width)
{
    u8 Blank;

    if (ReversalFlag == 0)
    {
        Blank = 0x00;
    }
    else
    {
        Blank = 0xFF;
    }

    for (u8 i = StartRowNum; i < StartRowNum + Height; i++)
    {
        OLED_SetLocation(StartColumn, i);

        for (u8 j = StartColumn; j < StartColumn + Width; j++)
        {
            OLED_SendData(Blank);
        }
    }

    return;
}

// 每行占8的像素，视为屏幕行 Row 0~7
u8 OLED_Show_XxN8_Character(u8 Row, u8 Column, u8 RowHeight, u8 Width, const u8 *FontArray)
{
    u8 i;
    u8 CurrentRow = 0;

    while (CurrentRow < RowHeight)
    {
        OLED_SetLocation(Column, Row + CurrentRow);
        for (i = 0; i < Width; i++)
        {
            if (ReversalFlag == 0)
            {
                OLED_SendData(((u8(*)[Width])FontArray)[CurrentRow][i]);
            }
            else
            {
                OLED_SendData(~((u8(*)[Width])FontArray)[CurrentRow][i]);
            }
        }
        CurrentRow++;
    }

    return 0;
}

void OLED_ShowString(u8 RowNumber, u8 RowHeight, u8 Column, const u8 *StringFont, u8 StringLength)
{
    for (u32 j = 0; j < StringLength; j += 1)
    {
        OLED_Show_XxN8_Character(RowNumber, (j * 16 + Column), RowHeight, 16, StringFont + (32 * j));
    }
    return;
}

void OLED_ShowPicture(u8 x, u8 y, u8 px, u8 py, const u8 *Picture)
{
    u8 i;
    u8 CurrentRow = 0;

    while (CurrentRow < (py / 8))
    {
        OLED_SetLocation(x, (y / 8) + CurrentRow);
        for (i = 0; i < px; i++)
        {
            if (ReversalFlag == 0)
            {
                //				OLED_SendData(((u8 (*)[px])Picture)[CurrentRow][i]);// 强转常量类型操作不合适
                OLED_SendData(*(Picture + CurrentRow * px + i));
            }
            else
            {
                //				OLED_SendData(~((u8 (*)[px])Picture)[CurrentRow][i]);// 强转常量类型操作不合适
                OLED_SendData(*(Picture + CurrentRow * px + i));
            }
        }
        CurrentRow++;
    }

    return;
}

/**
  * @}
  */
