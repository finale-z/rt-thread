/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-02-13     ZLW          first version
 */

#ifndef _DRV_EPAPER_H__
#define _DRV_EPAPER_H__

typedef struct epaper_image
{
    rt_uint8_t *black_image;
    rt_uint8_t *other_image;    // Red or yellow image
}*epaper_image_t;

enum EPAPER_CMD_E
{
    EPAPER_CLEAR = 0,
    EPAPER_DISPLAY_IMAGE,
    EPAPER_SLEEP,
#ifdef EPAPER_1IN54
    EPAPER_DISPLAY_PART_BASE_IMAGE,
    EPAPER_DISPLAY_PART_IMAGE,
#endif
    EPAPER_CMD_END
};

int rt_hw_epaper_init(void);

#endif