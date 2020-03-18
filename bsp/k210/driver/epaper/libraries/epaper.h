/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-03-08     ZLW          first version
 */

#ifndef EPAPER_H__
#define EPAPER_H__

/********************************************************/
/* header includes */
#include <stdint.h>
#include <stddef.h>
#include "epapercfg.h"

int8_t epaper_init(struct epaper_device *epaper_dev);

#endif