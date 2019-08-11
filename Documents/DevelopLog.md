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

    - UART
        USART1_TX PA9
        USART1_RX PA10

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

### Softwave
使用GPIO的输出功能检查LED2&LED3工作状态
为便于调试，添加串口功能


## Update
---
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