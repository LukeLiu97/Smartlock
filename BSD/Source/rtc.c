/**
******************************************************************************
  * @file       rtc.c
  * @brief      Real time drive
  * @version    1.1
  * @date       Aug-15-2019 Thu
******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/** @addtogroup Real Time Clock
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const u32 CommonYearSec = 365 * 24 * 60 * 60;
const u32 LeapYearSec = 365 * 24 * 60 * 60;
const u8 Months[12] =
    {
        31, 28, 31, 30,
        31, 30, 31, 31,
        30, 31, 30, 31};
const u32 DaySec = 24 * 60 * 60;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

// NVIC RTC秒中断配置

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval 
  */
u8 RTC_Config(void)
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE); // 备份区域存储RTC标志

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    BKP_DeInit();

    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
    }

    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    return 0;
}

#define RTC_FLAG 0x9797 //自定义

void RTC_Init(void)
{
    if (BKP_ReadBackupRegister(BKP_DR1) != RTC_FLAG)
    {
        /* Backup data register value is not correct or not yet programmed (when
		the first time the program is executed) */

        /* RTC 设定 */
        if (RTC_Config() != 0)
        {
            // 起振失败
        }
        else
        {
            // User adjust time task 缺省

            BKP_WriteBackupRegister(BKP_DR1, RTC_FLAG);
        }
    }
    else
    {
        /* Check if the Power On Reset flag is set */
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
        }
        /* Check if the Pin Reset flag is set */
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
        }
        else
        {
        }

        /* Wait for RTC registers synchronization */
        RTC_WaitForSynchro();

        /* Enable the RTC Second */
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    }
}

//void RTC_TimeCalulate(TimeStu *Time)
//{
////	TimeStu TimeTemp;
//	u32 RTC_Value;
//	RTC_Value = RTC_GetCounter();
//
//	Time->Hour = RTC_Value / 3600;
//	Time->Minute = (RTC_Value % 3600) / 60;
//	Time->Second = (RTC_Value % 3600) % 60;

//	return ;
//}

//u8 RTC_SetCountValue(TimeStu *Time)
//{
//	u32 ValueTemp = 0;
//
//	u32 i;
//
//	if(Time->Year < 1970 || Time->Year > 2099)
//	{
//		return 1;
//	}
//	else
//	{
//		for(i = 1970; i < Time->Year ; i++)
//		{
//			if(i / 400 == 0)
//			{
//				ValueTemp += LeapYearSec;
//			}
//			else if(i / 4 == 0 && i/ 100 != 0)
//			{
//				ValueTemp += LeapYearSec;
//			}
//			else
//			{
//				ValueTemp += CommonYearSec;
//			}
//		}
//
//		for(i = 1;i < Time->Month;i++)
//		{
//			ValueTemp += Months[i-1] * DaySec;
//
//			if(i == 2 && Time->Year / 4 == 0 && Time->Year / 100 != 0)
//			{
//				ValueTemp += DaySec;
//			}
//			else
//			{
//			}
//		}
//
//
//		ValueTemp += (Time->Date - 1) * DaySec;
//		ValueTemp += Time->Hour * 3600;
//		ValueTemp += Time->Minute * 60;
//		ValueTemp += Time->Second;
//	}
//
//	/* Enable PWR and BKP clocks */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
//	/* Allow access to BKP Domain */
//	PWR_BackupAccessCmd(ENABLE);
//
//	RTC_SetCounter(ValueTemp);
//	RTC_WaitForLastTask();
//
//	return 0;
//}

// 以下修改自网络源码

const unsigned char DateMaxTab[13] = {
    0,
    31,
    28,
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31,
};

/****************************************************************************
* 名    称：u8 Is_Leap_Year(u16 year)
* 功    能：判断是否是闰年
* 入口参数：year
* 出口参数：1:是闰年；0:非闰年
* 说    明：
* 调用方法：无 
****************************************************************************/
u8 Is_LeapYear(u16 year)
{
    if (year % 4 == 0) //必须能被4整除
    {
        if (year % 100 == 0)
        {
            if (year % 400 == 0)
                return 1; //如果以00结尾,还要能被400整除
            else
                return 0;
        }
        else
            return 1;
    }
    else
        return 0;
}

/****************************************************************************
* 名    称：u8 Get_Week(u16 Year,u16 Month,u16 Date)
* 功    能：用蔡勒（Zeller）公式计算星期几	

	w=y+[y/4]+[c/4]-2c+[26（m+1）/10]+d-1

	然后w对7取余。
	公式中的符号含义如下，w：星期；c：世纪；y：年（两位数）；m：月（m大于等于3，小
	于等于14，即在蔡勒公式中，某年的1、2月要看作上一年的13、14月来计算，比如2003年1月1
	日要看作2002年的13月1日来计算）；d：日；[ ]代表取整，即只要整数部分。

* 入口参数：Year:年; Month:月; Date:日。
* 出口参数：星期几 1~6代表星期一~星期六，0代表星期天
* 说    明：
* 调用方法：无
****************************************************************************/
u8 Get_Week(u16 Year, u16 Month, u16 Date)
{
    int W, Y, C, M, D;
    u8 day;

    C = Year / 100;
    Y = Year % 100;
    M = Month;
    if (M < 3)
    {
        M += 12;
        Y--;
    }
    D = Date;
    W = Y + Y / 4 + C / 4 - 2 * C + 26 * (M + 1) / 10 + D - 1;
    while (W < 0)
        W += 7;
    day = W % 7;
    return day;
}

u8 Get_DateMax(u16 Year, u16 Month)
{
    u8 DateMax;

    DateMax = DateMaxTab[Month];
    if (Month == 2)
    {
        if (Is_LeapYear(Year) == 1)
            DateMax++;
    }

    return DateMax;
}

/****************************************************************************
* 名    称：u8 Set_RTCTime(struct T_STRUCT *Time)
* 功    能：设置时钟:以1970-01-01 0:0:0为基准,把输入的时钟转换为秒钟,
* 					并写入RTC计数器，1970~2099年为合法年份。
* 入口参数：T_STRUCT* Time
* 出口参数：0:成功;
						1:错误代码，非有效时间(1970~2099)
* 说    明：
* 调用方法：Set_RTCTime(&Real_Time);
****************************************************************************/
u8 Set_RTCTime(TimeStu *Time)
{
    u32 Count = 0;
    u16 i;
    if (Time->Year < 1970 || Time->Year > 2099)
        return 1;
    for (i = 1970; i < Time->Year; i++)
    {
        if (Is_LeapYear(i))
            Count += 31622400; //闰年的秒数
        else
            Count += 31536000; //平年的秒数
    }
    for (i = 1; i < Time->Month; i++)
    {
        Count += (u32)DateMaxTab[i] * 86400;
        if (Is_LeapYear(Time->Year) && i == 2)
            Count += 86400;
    }
    Count += (u32)(Time->Date - 1) * 86400;
    Count += (u32)(Time->Hour) * 3600;
    Count += (u32)(Time->Minute) * 60;
    Count += (u32)(Time->Second);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    RTC_SetCounter(Count);
    RTC_WaitForLastTask();

    return 0;
}

/****************************************************************************
* 名    称：void Cal_Time(u32 Count,struct T_STRUCT *Time)
* 功    能：根据32位Count计数值计算日期和时间
* 入口参数：Count
* 出口参数：T_STRUCT* Time
* 说    明：
* 调用方法：Cal_Time(Count,&Real_Time);
****************************************************************************/
void Cal_Time(u32 Count, TimeStu *Time)
{
    u32 SecTemp = 0, DateTemp = 0, Temp = 0;

    DateTemp = Count / 86400; //天数
    if (DateTemp != 0)        //超过1天
    {
        //确定年
        Temp = 1970;
        while (DateTemp >= 365)
        {
            if (Is_LeapYear(Temp)) //闰年?
            {
                if (DateTemp >= 366)
                    DateTemp -= 366;
                else
                {
                    break;
                }
            }
            else
                DateTemp -= 365;
            Temp++;
        }
        Time->Year = Temp;

        //确定月
        Temp = 1;
        while (DateTemp >= 28)
        {
            if (Is_LeapYear(Time->Year) && Temp == 2)
            {
                if (DateTemp >= 29)
                    DateTemp -= 29; //闰年的二月天数
                else
                    break;
            }
            else
            {
                if (DateTemp >= DateMaxTab[Temp])
                    DateTemp -= DateMaxTab[Temp];
                else
                    break;
            }
            Temp++;
        }
        Time->Month = Temp;
        Time->Date = DateTemp + 1;
    }
    else
    {
        Time->Year = 1970;
        Time->Month = 1;
        Time->Date = 1;
    }
    SecTemp = Count % 86400;
    Time->Hour = SecTemp / 3600;
    Time->Minute = SecTemp % 3600 / 60;
    Time->Second = SecTemp % 60;
    Time->Day = Get_Week(Time->Year, Time->Month, Time->Date);
}

/****************************************************************************
* 名    称：void Read_RTCTime(struct T_STRUCT *Time)
* 功    能：从RTC计数器读取时间		
* 入口参数：T_STRUCT* Time
* 出口参数：无
* 说    明：
* 调用方法：Read_RTCTime(&Real_Time);
****************************************************************************/
void Read_RTCTime(TimeStu *Time)
{
    u32 Count = 0;
    Count = RTC_GetCounter();
    Cal_Time(Count, Time);
}

/**
  * @}
  */
