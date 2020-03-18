/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-02-13     ZLW          first version
 */

#include <rthw.h>
#include <rtthread.h>

#ifdef USING_EPAPER
#include "drv_epaper.h"

#include <drv_io_config.h>
#include <spi.h>
#include <dmac.h>
#include <sysctl.h>
#include <gpiohs.h>
#include <string.h>

static struct epaper_device _epaper_dev;

static rt_err_t _epaper_init(struct epapet_intf *intf)
{
    rt_uint8_t ret;
    _epaper_dev.epaper_spi  = (struct rt_spi_device *)rt_device_find(intf->dev_name);
    if (_epaper_dev.epaper_spi  == RT_NULL)
    {
        rt_kprintf("can not find device %s", intf->dev_name);
        return -RT_ERROR;
    }

    _epaper_dev.epaper_spi->config.mode = RT_SPI_MASTER | RT_SPI_MODE_1 | RT_SPI_MSB;
    _epaper_dev.epaper_spi->config.data_width = 8;
    _epaper_dev.epaper_spi->config.max_hz = 20 * 1000 *1000; ;
    rt_spi_configure(_epaper_dev.epaper_spi,&(_epaper_dev.epaper_spi->config));;

    ret = epaper_init(&_epaper_dev);
    return ret;
}

static rt_err_t epaper_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

static rt_err_t epaper_close(rt_device_t dev)
{
    return RT_EOK;
}


static void epaper_reset(struct epaper_device dev)
{
    rt_pin_write(dev.epaper_gpio.rst_io,PIN_HIGH);
    rt_delayms(10);
    rt_pin_write(dev.epaper_gpio.rst_io,PIN_LOW);
    rt_delayms(10);
    rt_pin_write(dev.epaper_gpio.rst_io,PIN_HIGH);
}

int rt_hw_epaper_init(const char *name, struct rt_device_t *dev)
{
    struct epaper_device *epaper = (struct epaper_device*)dev;
    _epaper_dev.epaper_gpio.busy_io = EPAPER_BUSY_GPIO;
    _epaper_dev.epaper_gpio.dc_io = EPAPER_DC_GPIO;
    _epaper_dev.epaper_gpio.rst_io = EPAPER_RST_GPIO;
    if (_epaper_init(&epaper->intf) != RT_EOK)
    {
        return RT_ERROR;
    }
}

#endif