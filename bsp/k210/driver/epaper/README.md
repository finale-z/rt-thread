# 微雪电子墨水屏驱动说明

## 1.简介

见微雪电子官方 [WiKi][1]

## 2.编译说明

按照 [编译板级支持包](../../README.md) 的说明，在 menuconfig 中的 WaveShare 菜单中勾选 Using Epaper，在 Using Epaper 的子菜单中选择墨水屏型号，配置相关引脚( SPI 及 CS 引脚的配置在主菜单中的 Enable SPI1 中)。然后正常编译即可，驱动会自动注册名为 epaper 的独立设备节点。

menuconfig device path：

    WaveShare --->
        [*]Using Epaper --->
            e-Paper Module (1.54inch epaper) --->
                The CS pin (SS0) --->
        (11)    The DC pin
        (1)     The DC GPIO
        (12)    The RST pin
        (2)     The RST GPIO
        (13)    The BUSY pin
        (3)     The BUSY GPIO
    
    [*]Enable SPI1  --->
        (29)  spi1 clk pin number
        (30)  spi1 d0 pin number
        (31)  spi1 d1 pin number
        -*-   SPI1 Enable SS0  ---> 
        [*]   SPI1 Enable SS1  --->
        [ ]   SPI1 Enable SS2  ---> 
        [ ]   SPI1 Enable SS3  ----

## 3.设备管理接口

## 3.1 初始化设备

通过如下接口函数：

    rt_err_t rt_device_init(rt_device_t dev)
函数参数

-----------------------------------------------------------------------
          参数  描述
--------------  -------------------------------------------------------
           dev  设备句柄；
-----------------------------------------------------------------------

## 3.2 控制设备

根据设备控制块来控制设备，可以通过下面的接口函数来完成：

    rt_err_t rt_device_control(rt_device_t dev, rt_uint8_t cmd, void* arg)

**函数参数**

-----------------------------------------------------------------------
          参数  描述
--------------  -------------------------------------------------------
           dev  设备句柄；

           cmd  命令控制字，这个参数通常与设备驱动程序相关；

           arg  控制的参数
-----------------------------------------------------------------------

其中与墨水屏相关的命令控制字在[drv_epaper.h](drv_epaper.h)中：

|命令控制字|作用|
| ------ | -----|
|EPAPER_CLEAR|墨水屏清屏|
|EPAPER_DISPLAY_IMAGE|墨水屏显示图像|
|EPAPER_SLEEP|墨水屏休眠|
|EPAPER_DISPLAY_PART_BASE_IMAGE|局刷(仅部分屏幕支持)|
|EPAPER_DISPLAY_PART_IMAGE|局刷(仅部分屏幕支持)|

**由于三色墨水屏一幅图像需拆分成两张图像去刷新，因此在三色墨水屏中控制墨水屏显示图像时控制参数与单色屏不同。单色墨水屏仅需直接传入图像地址，三色墨水屏需将拆分的两张图像的地址放入结构体 epaper_image_t 中，然后将结构体指针作为控制参数传入。**

三色墨水屏显示图像示例代码：

```
#include <drv_epaper.h>

rt_device_t epaper_dev;
epaper_image_t my_image;

epaper_dev = rt_device_find("epaper);
my_image->black_image = (rt_uint8_t*)my_image_black;
my_image->other_image = (rt_uint8_t*)my_image_other;

rt_device_control(epaper_dev,EPAPER_DISPLAY_IMAGE,my_image);
```


[1]:http://www.waveshare.net/wiki/1.54inch_e-Paper_Module