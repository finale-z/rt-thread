/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018/09/30     Bernard      The first version
 */

#include <rtthread.h>
#include <stdio.h>
#include <rtdevice.h>
 #include <drv_epaper.h>
#include "spi.h"
#include "drv_spi.h"
#include <fpioa.h>

rt_err_t epaper(void)
{
    rt_device_t epaper_dev;
    epaper_image_t my_image;

    epaper_dev = rt_device_find("epaper");
    if(epaper_dev == RT_NULL)
    {
        rt_kprintf("device err");
        return RT_ERROR;
    }
    rt_uint8_t *my_image_black = (rt_uint8_t *)malloc(200*200*sizeof(rt_uint8_t));
    if(my_image_black == RT_NULL)
    {
        rt_kprintf("error");
        return RT_ERROR;
    }
    rt_memset(my_image_black,0xFF,200*200*sizeof(rt_uint8_t));
    my_image->black_image = my_image_black;
    my_image->other_image = my_image_black;
    rt_device_init(epaper_dev);
    rt_kprintf("init done\n");
    //rt_device_control(epaper_dev,EPAPER_CLEAR,RT_NULL);
}
MSH_CMD_EXPORT_ALIAS(epaper,e,test epaper);

int main(void)
{
    fpioa_set_function(13, FUNC_SPI1_D0);
  fpioa_set_function(17, FUNC_SPI1_D1);
  fpioa_set_function(34, FUNC_SPI1_SCLK);
  fpioa_set_function(18, FUNC_SPI1_SS0);
  spi_init(SPI_DEVICE_1, SPI_WORK_MODE_1, SPI_FF_STANDARD, 8, 0);
    //rt_kprintf("Hello, world\n");

    return 0;
}

