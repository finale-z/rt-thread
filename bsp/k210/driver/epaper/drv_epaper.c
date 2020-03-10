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
#include <rtdevice.h>

#ifdef USING_EPAPER
#include "drv_epaper.h"
#include "epaper.h"
#include <drv_io_config.h>
#include <spi.h>
#include <dmac.h>
#include <sysctl.h>
#include <gpiohs.h>
#include <string.h>

static struct rt_spi_device *spi_dev;
static struct epaper_device _epaper_dev;

static rt_err_t _epaper_init(struct epapet_intf *intf)
{
    rt_uint8_t ret;
    spi_dev = (struct rt_spi_device *)rt_device_find(intf->dev_name);
    if (spi_dev == RT_NULL)
    {
        LOG_E("can not find device %s", intf->dev_name);
        return -RT_ERROR;
    }
    ret = epaper_init(&_epaper_dev);
    return ret;
}

int rt_hw_epaper_init(const char *name, struct epaper_config *cfg)
{
    if (_epaper_init(&cfg->intf) != RT_EOK)
    {
        return RT_ERROR;
    }
}

#endif