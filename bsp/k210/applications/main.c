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

int main(void)
{
    rt_system_scheduler_init();
    rt_system_scheduler_start();
    rt_kprintf("Hello, world\n");

    return 0;
}
