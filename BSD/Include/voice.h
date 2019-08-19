/**
******************************************************************************
  * @file       voice.h
  * @brief      声音模块操作头文件
  * @version    1.0
  * @date       Wed 07-08-2019
******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VOICE_H
#define __VOICE_H

#ifdef __cplusplus
extern "C"
{
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include "global.h"

#include "time.h"

    /* Global varaiables ---------------------------------------------------------*/
    extern u8 MuteMode;
    extern SmartLockStu SmartLock;

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    typedef enum
    {
        VoiceCmd_CHANGE_ADMIN_PASSWORD = 1,     //修改管理员密码
        VoiceCmd_SET_DOOR_PASSWORD = 2,         //设置开门密码
        VoiceCmd_REGISTER_FINGER = 3,           //登记指纹
        VoiceCmd_REGISTER_CARD = 4,             //登记卡片
        VoiceCmd_CHANGE_TIME = 5,               //修改时间
        VoiceCmd_DELETE_ALLFINGER = 6,          //删除所有指纹
        VoiceCmd_DELETE_ASSIGN_FINGER = 7,      //删除指定指纹
        VoiceCmd_RECOVER_ALL = 8,               //恢复出厂设置
        VoiceCmd_CHANGE_VOLUME = 9,             //设置音量
        VoiceCmd_DELETE_ALL_CARD = 10,          //删除所有门卡
        VoiceCmd_DELETE_ASSIGN_CARD = 11,       //删除指定门卡
        VoiceCmd_INPUT_ADMIN_PASSWORD = 12,     //请输入管理员密码
        VoiceCmd_INPUT_NEW_PASSWORD = 13,       //请输入新密码
        VoiceCmd_INPUT_NEW_PASSWORD_AGAIN = 14, //请再次输入新密码
        VoiceCmd_PASSWORD_ERROR = 15,           //验证失败
        VoiceCmd_PASSWORD_INCONFORMITY = 16,    //密码不一致
        VoiceCmd_SETTING_SUCCESS = 17,          //操作成功
        VoiceCmd_PASSWORD_REPETITION = 18,      //密码重复
        VoiceCmd_PUTCARD = 19,                  //请放置卡片
        VoiceCmd_CARD_REGISTERED = 20,          //卡重复
        VoiceCmd_DELETE_ALL_FIGNER = 21,        //是否删除
        VoiceCmd_DELETE_ASSIGNFIGNER = 22,      //请放手指
        VoiceCmd_FINGER_REPETITION = 23,        //指纹重复
        VoiceCmd_DOOROPEN_SUCCESS = 24,         //欢迎回家
        VoiceCmd_DOOROPEN_FAIL = 25,            //开门失败
        VoiceCmd_CONNECT_SUCCESS = 26,          //蓝牙连接成功
        VoiceCmd_TIPS = 27,                     //按#号确认，按*号退出
        VoiceCmd_REPETE = 28,                   //请重新设置
        VoiceCmd_Di = 29,                       //嘟
        VoiceCmd_MenLing = 30,                  //叮咚 叮咚
        VoiceCmd_Warm = 31                      //报警声
    } VoiceCmd_TypeDef;

    /* Exported defines ----------------------------------------------------------*/
    //#define CHANGE_ADMIN_PASSWORD		1 //修改管理员密码
    //#define SET_DOOR_PASSWORD			2 //设置开门密码
    //#define REGISTER_FINGER				3 //登记指纹
    //#define REGISTER_CARD				4 //登记卡片
    //#define CHANGE_TIME					5 //修改时间
    //#define DELETE_ALLFINGER			6 //删除所有指纹
    //#define DELETE_ASSIGN_FINGER		7 //删除指定指纹
    //#define RECOVER_ALL					8 //恢复出厂设置
    //#define CHANGE_VOLUME				9 //设置音量
    //#define DELETE_ALL_CARD				10 //删除所有门卡
    //#define DELETE_ASSIGN_CARD			11//删除指定门卡
    //#define INPUT_ADMIN_PASSWORD		12//请输入管理员密码
    //#define INPUT_NEW_PASSWORD			13//请输入新密码
    //#define INPUT_NEW_PASSWORD_AGAIN	14//请再次输入新密码
    //#define PASSWORD_ERROR				15//验证失败
    //#define PASSWORD_INCONFORMITY		16//密码不一致
    //#define SETTING_SUCCESS				17//操作成功
    //#define PASSWORD_REPETITION			18//密码重复
    //#define PUTCARD						19//请放置卡片
    //#define CARD_REGISTERED				20//卡重复
    //#define DELETE_ALL_FIGNER			21//是否删除
    //#define DELETE_ASSIGNFIGNER			22//请放手指
    //#define FINGER_REPETITION			23//指纹重复
    //#define DOOROPEN_SUCCESS			24//欢迎回家
    //#define DOOROPEN_FAIL				25//开门失败
    //#define CONNECT_SUCCESS				26//蓝牙连接成功
    //#define TIPS						27//按#号确认，按*号退出
    //#define REPETE						28//请重新设置
    //#define Di							29//嘟
    //#define MenLing						30//叮咚 叮咚
    //#define Warm						31//报警声

    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */
    void VoiceModule_Init(void);
    void Voice_Play(VoiceCmd_TypeDef VoiceCmd);

#ifdef __cplusplus
}
#endif

#endif
