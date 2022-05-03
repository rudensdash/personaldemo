该项目使用 8051单片机 外接 DS18B20 获取环境温度，通过串口发送到ESP8266， ESP8266通过MQTT协议上传到阿里云平台。

8051部分使用 KEIL + C语言开发 | ESP8266 使用 RTOS SDK + C语言开发。

你可以在代码里学到 DS18B20 驱动方法以及 ESP8266 RTOS 对于 MQTT 的使用。

开发板 STC15L2K60S2 + ESP8266NODE MCU
