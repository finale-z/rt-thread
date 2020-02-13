#include <rthw.h>
#include <rtthread.h>

#ifdef USING_EPAPER
#include "epaper.h"
#include <gpiohs.h>

static struct epaper_config
{
    rt_uint8_t dc_pin;
    rt_uint8_t rst_pin;
};

static void epaper_gpio_init(struct  epaper_config *epaper_io)
{
    gpiohs_set_drive_mode(epaper_io->dc_pin,GPIO_DM_OUTPUT);
    gpiohs_set_drive_mode(epaper_io->rst_pin,GPIO_DM_OUTPUT);
}

static void epaper_reset(struct  epaper_config *epaper_io)
{
    gpiohs_set_pin(epaper_io->rst_pin,GPIO_PV_HIGH);
    rt_thread_delay(20);
    gpiohs_set_pin(epaper_io->rst_pin,GPIO_PV_LOW);
    rt_thread_delay(1);
    gpiohs_set_pin(epaper_io->rst_pin,GPIO_PV_HIGH);
    rt_thread_delay(20);
}

static void epaper_send_command()
{
    
}
int rt_hw_epaper_init(void)
{
    rt_err_t ret = RT_EOK;



    return ret;
}

#endif