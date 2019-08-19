# Smart lock project
====================

## Description
---

### Hardwave
Smartlock_v1.3 
- STM32F103C8T6
    - GPIO
        LED2 PB8
        LED3 PB6

        VOICE_DATA PB0
        VOICE_BUSY PB1

        IIC_SCK PB4
        IIC_SDA PB5
        > 与JTAG功能冲突，需要重映射

        MPR_IRQ PB3
        > 与JTAG功能冲突，需要重映射
        MPR_SCK IIC_SCK
        MPR_SDA IIC_SDA

        OLED_DC PA11
        OLED_RES PA12
        OLED_CS PA15
        > 与JTAG功能冲突，需要重映射

        MG200_PWR PB9
        MG200_DETECT PC13

        RFID_CS PA6  
        RFID_RST PA7

        MOTOR_IA PA5 
        MOTOR_IB PA4

    - UART
        USART1_TX PA9
        USART1_RX PA10

        USART2_TX PA2
        USART2_RX PA3

    - SPI
        SPI2_SCLK PB13
        SPI2_MISO PB14
        SPI2_MOSI PB15

- 思泽语音模块
    单总线

- MPR121
    Proximity Capacitive Touch Sensor Controller
    使用模拟IIC进行通信
    当芯片检测到按键变化会使MPR_IRQ引脚产生一个下降沿，当对寄存器读取后改引脚才会恢复高电平

- 128x64 OLED
    驱动为SSD1306,支持6800、8080、三线或四线SPI以及I2C通行
    此处使用了四线SPI

- MG200
    指纹模块，使用UART二次封装后的数据包。PWR引脚控制使能，DETECT在手指按下输出低电平。

- RC522
    RFID 模块，使用SPI2通讯，

- EEPROM AT24C04
    容量为512Byte,使用模拟IIC

### Softwave
使用GPIO的输出功能检查LED2&LED3工作状态
为便于调试，添加串口功能


## Update
---
- ver 2.3
    添加看门狗以及低功耗
- ver 2.2
    添加用户数据掉电不丢失功能
- ver 2.1
    添加实时时钟功能
- ver 2.0
    优化程序
- ver 1.1
    添加RFID模块实现门卡解锁
- ver 1.0
    添加电机驱动模块实现门锁管理
- ver 0.8
    添加指纹模块驱动
- ver 0.7
    添加简易的GUI实现图形化人机交互
- ver 0.6
    添加了OLED驱动程序，使用SPI通讯
    OLED_CS引脚与JTAG引脚冲突，同样进行了重映射
- ver 0.5
    添加了MPR121电容按键驱动相关程序
    该模块使用了软件IIC通信且该模块的激活中断引脚MPR_IRQ与JTAG功能冲突，同样进行了重映射
    PB3的中断连接至EXTI_Line3。

    BUG:由于使用了软件IIC会导致外中断中多次调用systick生成的延时不能正常工作。
- ver 0.4
    添加软件模拟IIC程序
    使用了Systick实现IIC通信的时序生成
    同时由于JTAG引脚冲突进行了重映射
- ver 0.3
    添加声音发生模块驱动程序
    使用了Systick实现单总线通信的时序生成
- ver 0.2
    添加串口功能
- ver 0.1
    创建工程，检查硬件