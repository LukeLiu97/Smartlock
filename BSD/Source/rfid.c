/**********************************************************************************************
Copyright 2008 - 2016 深圳市信盈达电子有限公司. All rights reserved.
描述   :     
作者   :       谢国卿
版本   :       V1.0
修改   :   
完成日期：     2017年12月12日
信盈达官网：http://www.edu118.com/
信盈达网校：http://www.edu118.cn/
信盈达技术论坛：http://bbs.edu118.com/
Notice    :本程序只供学习使用，未经作者许可，不得用于其它任何用途。版权所有，盗版必究
***********************************************************************************************/

/**********************************************************************************************
* INCLUDES
*/
#include "rfid.h"
#include "oled.h"
#include "time.h"
#include "string.h"
/**********************************************************************************************
* CONSTANTS
*/

/**********************************************************************************************
* TYPEDEFS
*/

/**********************************************************************************************
* LOCAL VARIABLES
*/

/**********************************************************************************************
* LOCAL FUNCTIONS  DECLARE
*/

/**********************************************************************************************
* LOCAL FUNCTIONS  
*/

u8 spi_read_write(u8 SendData)
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

/**********************************************************************************************
* PUBLIC FUNCTIONS
*/
/*******************************************************************
* 函数名	:		RFID_Init()
* 功能描述:		初始化RFID的使能脚（与屏幕共用SPI，函数需要放在OLED_INIT后）
* 作者:			谢国卿 
* 参数说明:		无
* 返回值说明:	无
* 修改记录:		移植标准库
* 修改人	:		LukeLiu
* 其他：

*******************************************************************/
/*************************************
*硬件连接说明：
*1--SDA (CS)<----->PA6
*2--SCK <----->PB13
*3--MOSI<----->PB15
*4--MISO<----->PB14
*5--IRQ<------>中断
*6--GND <----->GND
*7--RST <----->PA7
*8--3.3V<----->3.3V
**************************************/
void RFID_Init(void)
{
    // 由于RC522在较高速率通信不稳定，所以SPI时钟应稍慢一些

    /* GPIO ConfigureRFID_CS PA6  RFID_RST PA7 */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOA Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure PA6 and PA7 in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RFID_CS_H();
    RFID_RST_H();

    Reset_RC522(); //开启天线
    M500PcdConfigISOType('A');
}

/*******************************************************************
* 函数名：     Reset_RC522()
* 功能描述 ：   复位RC522
* 作者：         谢国卿 
* 参数说明：     无
* 返回值说明：   无
* 修改记录：
* 其他：
*******************************************************************/
void Reset_RC522(void)
{
    PcdReset();      //功    能：复位RC522
    PcdAntennaOff(); //关闭天线
    PcdAntennaOn();  //开启天线
}

/*******************************************************************
* 函数名：     PcdRequest()
* 功能描述 ：   	寻卡
* 作者：         谢国卿 
*参数说明: req_code[IN]:寻卡方式
                0x52 = 寻感应区内所有符合14443A标准的卡
                0x26 = 寻未进入休眠状态的卡
           pTagType[OUT]：卡片类型代码
                0x4400 = Mifare_UltraLight
                0x0400 = Mifare_One(S50)
                0x0200 = Mifare_One(S70)
                0x0800 = Mifare_Pro(X)
                0x4403 = Mifare_DESFire
* 返回值说明：成功返回MI_OK
* 修改记录：
* 其他：
*******************************************************************/
char PcdRequest(u8 req_code, u8 *pTagType)
{
    char status;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg, 0x08);
    WriteRawRC(BitFramingReg, 0x07);
    SetBitMask(TxControlReg, 0x03);

    ucComMF522Buf[0] = req_code;

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen);

    if ((status == MI_OK) && (unLen == 0x10))
    {
        *pTagType = ucComMF522Buf[0];
        *(pTagType + 1) = ucComMF522Buf[1];
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}

/*******************************************************************
* 函数名：     PcdAnticoll()
* 功能描述 ：   防冲撞
* 作者：         谢国卿 
* 参数说明：     pSnr[OUT]:卡片序列号，4字节
* 返回值说明：   成功返回MI_OK
* 修改记录：
* 其他：
*******************************************************************/
char PcdAnticoll(u8 *pSnr)
{
    char status;
    u8 i, snr_check = 0;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg, 0x08);
    WriteRawRC(BitFramingReg, 0x00);
    ClearBitMask(CollReg, 0x80);

    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);

    if (status == MI_OK)
    {
        for (i = 0; i < 4; i++)
        {
            *(pSnr + i) = ucComMF522Buf[i];
            snr_check ^= ucComMF522Buf[i];
        }
        if (snr_check != ucComMF522Buf[i])
        {
            status = MI_ERR;
        }
    }

    SetBitMask(CollReg, 0x80);
    return status;
}

/*******************************************************************
* 函数名：     PcdSelect()
* 功能描述 ：   选定卡片
* 作者：         谢国卿 
* 参数说明：     pSnr[IN]:卡片序列号，4字节
* 返回值说明：   成功返回MI_OK
* 修改记录：
* 其他：
*******************************************************************/
char PcdSelect(u8 *pSnr)
{
    char status;
    u8 i;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i = 0; i < 4; i++)
    {
        ucComMF522Buf[i + 2] = *(pSnr + i);
        ucComMF522Buf[6] ^= *(pSnr + i);
    }
    CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]);

    ClearBitMask(Status2Reg, 0x08);

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, &unLen);

    if ((status == MI_OK) && (unLen == 0x18))
    {
        status = MI_OK;
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}

/*******************************************************************
* 函数名：     PcdAuthState()
* 功能描述 ：   验证卡片密码
* 作者：         谢国卿 
*参数说明: auth_mode[IN]: 密码验证模式
                 0x60 = 验证A密钥
                 0x61 = 验证B密钥 
								addr[IN]：块地址
								pKey[IN]：密码
								pSnr[IN]：卡片序列号，4字节
* 返回值说明：   成功返回MI_OK
* 修改记录：
* 其他：
*******************************************************************/
char PcdAuthState(u8 auth_mode, u8 addr, u8 *pKey, u8 *pSnr)
{
    char status;
    u8 unLen;
    //     u8   i;
    u8 ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    //    for (i=0; i<6; i++)
    //    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    //    for (i=0; i<6; i++)
    //    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    memcpy(&ucComMF522Buf[2], pKey, 6);
    memcpy(&ucComMF522Buf[8], pSnr, 4);

    status = PcdComMF522(PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, &unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {
        status = MI_ERR;
    }

    return status;
}

/*******************************************************************
* 函数名：     PcdRead()
* 功能描述 ：   读取M1卡一块数据
* 作者：         谢国卿 
*参数说明: addr[IN]：块地址
           p [OUT]：读出的数据，16字节
* 返回值说明：   成功返回MI_OK
* 修改记录：
* 其他：
*******************************************************************/
char PcdRead(u8 addr, u8 *p)
{
    char status;
    u8 unLen;
    u8 i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    //   {   memcpy(p , ucComMF522Buf, 16);   }
    {
        for (i = 0; i < 16; i++)
        {
            *(p + i) = ucComMF522Buf[i];
        }
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}

/*******************************************************************
* 函数名：     PcdWrite()
* 功能描述 ：   写数据到M1卡一块
* 作者：         谢国卿 
*参数说明: addr[IN]：块地址
           p [OUT]：读出的数据，16字节
* 返回值说明：   成功返回MI_OK
* 修改记录：
* 其他：
*******************************************************************/
char PcdWrite(u8 addr, u8 *p)
{
    char status;
    u8 unLen;
    u8 i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {
        status = MI_ERR;
    }

    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, p , 16);
        for (i = 0; i < 16; i++)
        {
            ucComMF522Buf[i] = *(p + i);
        }
        CalulateCRC(ucComMF522Buf, 16, &ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, &unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {
            status = MI_ERR;
        }
    }

    return status;
}

/*******************************************************************
* 函数名：     PcdHalt()
* 功能描述 ：   命令卡片进入休眠状态
* 作者：         谢国卿 
*参数说明: 
* 返回值说明：   成功返回MI_OK
* 修改记录：
* 其他：
*******************************************************************/
char PcdHalt(void)
{
    //    u8   status;
    u8 unLen;
    u8 ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    //    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);
    return MI_OK;
}

/*******************************************************************
* 函数名：     CalulateCRC()
* 功能描述 ：  用MF522计算CRC16函数
* 作者：         谢国卿 
*参数说明: 
* 返回值说明： 
* 修改记录：
* 其他：
*******************************************************************/
void CalulateCRC(u8 *pIn, u8 len, u8 *pOut)
{
    u8 i, n;
    ClearBitMask(DivIrqReg, 0x04);
    WriteRawRC(CommandReg, PCD_IDLE);
    SetBitMask(FIFOLevelReg, 0x80);
    for (i = 0; i < len; i++)
    {
        WriteRawRC(FIFODataReg, *(pIn + i));
    }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x04));
    pOut[0] = ReadRawRC(CRCResultRegL);
    pOut[1] = ReadRawRC(CRCResultRegM);
}

/*******************************************************************
* 函数名：     PcdReset()
* 功能描述 ：  复位RC522
* 作者：         谢国卿 
* 参数说明: 
* 返回值说明： 成功返回MI_OK
* 修改记录：
* 其他：
*******************************************************************/
char PcdReset(void)
{
    //PORTD|=(1<<RC522RST);
    RFID_RSTH;
    delay_us(2);

    //PORTD&=~(1<<RC522RST);
    RFID_RSTL;
    delay_us(2);

    //PORTD|=(1<<RC522RST);
    RFID_RSTH;
    delay_us(2);

    WriteRawRC(CommandReg, PCD_RESETPHASE);
    WriteRawRC(CommandReg, PCD_RESETPHASE);
    delay_us(2);

    WriteRawRC(ModeReg, 0x3D); //和Mifare卡通讯，CRC初始值0x6363
    WriteRawRC(TReloadRegL, 30);
    WriteRawRC(TReloadRegH, 0);
    WriteRawRC(TModeReg, 0x8D);
    WriteRawRC(TPrescalerReg, 0x3E);

    WriteRawRC(TxAutoReg, 0x40); //必须要

    return MI_OK;
}

/*******************************************************************
* 函数名：     M500PcdConfigISOType()
* 功能描述 ：  设置RC632的工作方式 
* 作者：         谢国卿 
* 参数说明: 
* 返回值说明：
* 修改记录：
* 其他：
*******************************************************************/
char M500PcdConfigISOType(u8 type)
{
    if (type == 'A') //ISO14443_A
    {
        ClearBitMask(Status2Reg, 0x08);
        WriteRawRC(ModeReg, 0x3D);   //3F
        WriteRawRC(RxSelReg, 0x86);  //84
        WriteRawRC(RFCfgReg, 0x7F);  //4F
        WriteRawRC(TReloadRegL, 30); //tmoLength);// TReloadVal = 'h6a =tmoLength(dec)
        WriteRawRC(TReloadRegH, 0);
        WriteRawRC(TModeReg, 0x8D);
        WriteRawRC(TPrescalerReg, 0x3E);
        delay_us(2);
        PcdAntennaOn();
    }
    else
    {
        return 1;
    }

    return MI_OK;
}

/*******************************************************************
* 函数名：     ReadRawRC()
* 功能描述 ：  读RC632寄存器
* 作者：         谢国卿 
* 参数说明:    Address[IN]:寄存器地址
* 返回值说明： 读出的值
* 修改记录：
* 其他：
*******************************************************************/
u8 ReadRawRC(u8 Address)
{
    u8 ucAddr;
    u8 ucResult = 0;
    RFID_CSL;
    ucAddr = ((Address << 1) & 0x7E) | 0x80;

    spi_read_write(ucAddr);
    ucResult = spi_read_write(0);
    RFID_CSH;
    return ucResult;
}

/*******************************************************************
* 函数名：     WriteRawRC()
* 功能描述 ：  写RC632寄存器
* 作者：         谢国卿 
* 参数说明:    Address[IN]:寄存器地址
							 value[IN]:写入的值
* 返回值说明：
* 修改记录：
* 其他：
*******************************************************************/
void WriteRawRC(u8 Address, u8 value)
{
    u8 ucAddr;
    //	u8 tmp;

    RFID_CSL;
    ucAddr = ((Address << 1) & 0x7E);

    spi_read_write(ucAddr);
    spi_read_write(value);
    RFID_CSH;

    // 		tmp=ReadRawRC(Address);

    // 		if(value!=tmp)
    // 		printf("wrong\n");
}

/*******************************************************************
* 函数名：     SetBitMask()
* 功能描述 ：  置RC522寄存器位
* 作者：         谢国卿 
* 参数说明:    reg[IN]:寄存器地址
							 mask[IN]:置位值
* 返回值说明：
* 修改记录：
* 其他：
*******************************************************************/
void SetBitMask(u8 reg, u8 mask)
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp | mask); // set bit mask
}

/*******************************************************************
* 函数名：     ClearBitMask()
* 功能描述 ：  清RC522寄存器位
* 作者：         谢国卿 
* 参数说明:    reg[IN]:寄存器地址
							 mask[IN]:清位值
* 返回值说明：
* 修改记录：
* 其他：
*******************************************************************/
void ClearBitMask(u8 reg, u8 mask)
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask); // clear bit mask
}

/*******************************************************************
* 函数名：     PcdComMF522()
* 功能描述 ：  通过RC522和ISO14443卡通讯
* 作者：         谢国卿 
* 参数说明:   Command[IN]:RC522命令字
							pIn [IN]:通过RC522发送到卡片的数据
							InLenByte[IN]:发送数据的字节长度
							pOut [OUT]:接收到的卡片返回数据
							*pOutLenBit[OUT]:返回数据的位长度
* 返回值说明：
* 修改记录：
* 其他：
*******************************************************************/
char PcdComMF522(u8 Command,
                 u8 *pIn,
                 u8 InLenByte,
                 u8 *pOut,
                 u8 *pOutLenBit)
{
    char status = MI_ERR;
    u8 irqEn = 0x00;
    u8 waitFor = 0x00;
    u8 lastBits;
    u8 n;
    u16 i;
    switch (Command)
    {
    case PCD_AUTHENT:
        irqEn = 0x12;
        waitFor = 0x10;
        break;
    case PCD_TRANSCEIVE:
        irqEn = 0x77;
        waitFor = 0x30;
        break;
    default:
        break;
    }

    WriteRawRC(ComIEnReg, irqEn | 0x80);
    ClearBitMask(ComIrqReg, 0x80); //清所有中断位
    WriteRawRC(CommandReg, PCD_IDLE);
    SetBitMask(FIFOLevelReg, 0x80); //清FIFO缓存

    for (i = 0; i < InLenByte; i++)
    {
        WriteRawRC(FIFODataReg, pIn[i]);
    }
    WriteRawRC(CommandReg, Command);
    //   	 n = ReadRawRC(CommandReg);

    if (Command == PCD_TRANSCEIVE)
    {
        SetBitMask(BitFramingReg, 0x80);
    } //开始传送

    i = 600; //根据时钟频率调整，操作M1卡最大等待时间25ms
             // 	  i = 100000;
    do
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitFor));
    ClearBitMask(BitFramingReg, 0x80);

    if (i != 0)
    {
        if (!(ReadRawRC(ErrorReg) & 0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {
                status = MI_NOTAGERR;
            }
            if (Command == PCD_TRANSCEIVE)
            {
                n = ReadRawRC(FIFOLevelReg);
                lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                }
                else
                {
                    *pOutLenBit = n * 8;
                }
                if (n == 0)
                {
                    n = 1;
                }
                if (n > MAXRLEN)
                {
                    n = MAXRLEN;
                }
                for (i = 0; i < n; i++)
                {
                    pOut[i] = ReadRawRC(FIFODataReg);
                }
            }
        }
        else
        {
            status = MI_ERR;
        }
    }

    SetBitMask(ControlReg, 0x80); // stop timer now
    WriteRawRC(CommandReg, PCD_IDLE);
    return status;
}

/*******************************************************************
* 函数名：     PcdAntennaOn()
* 功能描述 ：  开启天线 
* 作者：         谢国卿 
* 参数说明:    
* 返回值说明：
* 修改记录：
* 其他：
*******************************************************************/
void PcdAntennaOn(void)
{
    u8 i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

/*******************************************************************
* 函数名：     PcdAntennaOff()
* 功能描述 ：  关闭天线 
* 作者：         谢国卿 
* 参数说明:    
* 返回值说明：
* 修改记录：
* 其他：
*******************************************************************/
void PcdAntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}

char Read_IC_Card_ID(u8 *id)
{
    unsigned char CT[2];                      //卡类型
    if (PcdRequest(PICC_REQALL, CT) == MI_OK) //寻卡成功
    {

        if (PcdAnticoll(id) == MI_OK) /*防冲撞成功*/
        {

            if (PcdSelect(id) == MI_OK) //选卡成功
            {
                return MI_OK;
            }
        }
    }
    return MI_ERR;
}
/***********************************************************************************************
***********************************************************************************************/
