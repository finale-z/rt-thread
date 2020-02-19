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
#include "epapercfg.h"
#include <spi.h>
#include <fpioa.h>
#include <gpiohs.h>
#include <rtdbg.h>
#include <drv_io_config.h>

typedef struct epaper_device
{
    struct rt_device parent;
    struct rt_device_graphic_info epaper_info;
    int spi_channel;
    int cs;
    int dc_io;
    int rst_io;
    int busy_io;
    int dma_channel;
    rt_uint8_t width;
    rt_uint8_t height;
} * epaper_device_t;

static void drv_epaper_cmd(epaper_device_t epaper, rt_uint8_t cmd)
{
    gpiohs_set_pin(epaper->dc_io, GPIO_PV_LOW);
    spi_init(epaper->spi_channel, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 1);
    spi_send_data_normal_dma(epaper->dma_channel, epaper->spi_channel, epaper->cs, &cmd, 1, SPI_TRANS_CHAR);
}

static void drv_epaper_data_byte(epaper_device_t epaper, rt_uint8_t *data_buf, rt_uint8_t length)
{
    gpiohs_set_pin(epaper->dc_io, GPIO_PV_HIGH);
    spi_init(epaper->spi_channel, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 1);
    spi_send_data_normal_dma(epaper->dma_channel, epaper->spi_channel, epaper->cs, data_buf, length, SPI_TRANS_CHAR);
}

static void drv_epaper_data_half_word(epaper_device_t epaper, rt_uint16_t *data_buf, rt_uint8_t length)
{
    gpiohs_set_pin(epaper->dc_io, GPIO_PV_HIGH);
    spi_init(epaper->spi_channel, SPI_WORK_MODE_0, SPI_FF_STANDARD, 16, 1);
    spi_send_data_normal_dma(epaper->dma_channel, epaper->spi_channel, epaper->cs, data_buf, length, SPI_TRANS_SHORT);
}

static void drv_epaper_data_word(epaper_device_t epaper, rt_uint32_t *data_buf, rt_uint8_t length)
{
    gpiohs_set_pin(epaper->dc_io, GPIO_PV_HIGH);
    spi_init(epaper->spi_channel, SPI_WORK_MODE_0, SPI_FF_STANDARD, 32, 1);
    spi_send_data_normal_dma(epaper->dma_channel, epaper->spi_channel, epaper->cs, data_buf, length, SPI_TRANS_INT);
}

static void drv_epaper_hw_init(epaper_device_t epaper_io)
{
    gpiohs_set_drive_mode(epaper_io->dc_io,GPIO_DM_OUTPUT);
    gpiohs_set_drive_mode(epaper_io->rst_io,GPIO_DM_OUTPUT);
    gpiohs_set_drive_mode(epaper_io->busy_io,GPIO_DM_INPUT);
    spi_init(epaper_io->spi_channel, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 1);
    spi_set_clk_rate(epaper_io->spi_channel, 25000000);
}

static void drv_epaper_reset(epaper_device_t epaper_io)
{
    gpiohs_set_pin(epaper_io->rst_io,GPIO_PV_HIGH);
    rt_thread_mdelay(200);
    gpiohs_set_pin(epaper_io->rst_io,GPIO_PV_LOW);
    rt_thread_mdelay(10);
    gpiohs_set_pin(epaper_io->rst_io,GPIO_PV_HIGH);
    rt_thread_mdelay(200);
}

static void drv_epaper_read_busy(epaper_device_t epaper_io)
{
    while(gpiohs_get_pin(epaper_io->busy_io))
    {
        rt_thread_mdelay(100);
    }
}

#if (defined EPAPER_1IN54)
static void drv_epaper_display_turn_on(epaper_device_t epaper_dev)
{
    drv_epaper_cmd(epaper_dev,0x22);
    drv_epaper_data_byte(epaper_dev,0xF7,1);
    drv_epaper_cmd(epaper_dev,0x20);
    drv_epaper_read_busy(epaper_dev);   
}

static void drv_epaper_display_part_turn_on(epaper_device_t epaper_dev)
{
    drv_epaper_cmd(epaper_dev,0x22);
    drv_epaper_data_byte(epaper_dev,0xFF,1);
    drv_epaper_cmd(epaper_dev,0x20);
    drv_epaper_read_busy(epaper_dev);   
}
#endif

#ifdef EPAPER_1IN54_B
    static void drv_epaper_set_lut_bw(epaper_device_t epaper_dev)
    {
        drv_epaper_cmd(epaper_dev,0x20);
        drv_epaper_data_byte(epaper_dev,epd_1in54b_lut_vcom0,15);
        drv_epaper_data_byte(epaper_dev,epd_1in54b_lut_w,15);
        drv_epaper_data_byte(epaper_dev,epd_1in54b_lut_b,15);
        drv_epaper_data_byte(epaper_dev,epd_1in54b_lut_g1,15);
        drv_epaper_data_byte(epaper_dev,epd_1in54b_lut_g2,15);
    }

    static void drv_epaper_set_lut_red(epaper_device_t epaper_dev)
    {
        drv_epaper_cmd(epaper_dev,0x25);
        drv_epaper_data_byte(epaper_dev,epd_1in54b_lut_vcom1,15);
        drv_epaper_data_byte(epaper_dev,epd_1in54b_lut_red0,15);
        drv_epaper_data_byte(epaper_dev,epd_1in54b_lut_red1,15);
    }
#endif

static rt_err_t drv_epaper_init(rt_device_t dev)
{
    rt_err_t ret = RT_EOK;
    epaper_device_t epaper_dev = (epaper_device_t)dev;
    drv_epaper_reset(epaper_dev);

#ifdef EPAPER_1IN54_V2
    drv_epaper_read_busy(epaper_dev);
    drv_epaper_cmd(epaper_dev,0x12);      //SWRESET
    drv_epaper_read_busy(epaper_dev);

    drv_epaper_cmd(epaper_dev,0x01);        //Driver output control
    drv_epaper_data_byte(epaper_dev,0xC7,1);
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_data_byte(epaper_dev,0x01,1);

    drv_epaper_cmd(epaper_dev,0x11);        //data entry mode
    drv_epaper_data_byte(epaper_dev,0x01,1);

    drv_epaper_cmd(epaper_dev,0x44);        //set Ram-X address start/end position
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_data_byte(epaper_dev,0x18,1);      //0x0C-->(18+1)*8=200

    drv_epaper_cmd(epaper_dev,0x45);        //set Ram-Y address start/end position
    drv_epaper_data_byte(epaper_dev,0xC7,1);        //0xC7-->(199+1)=200
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_data_byte(epaper_dev,0x00,1);

    drv_epaper_cmd(epaper_dev,0x3C);        //BorderWavefrom
    drv_epaper_data_byte(epaper_dev,0x01,1);

    drv_epaper_cmd(epaper_dev,0x18);
    drv_epaper_data_byte(epaper_dev,0x80,1);

    drv_epaper_cmd(epaper_dev,0x22);        //Load Temperature and waveform setting.
    drv_epaper_data_byte(epaper_dev,0XB1,1);
    drv_epaper_cmd(epaper_dev,0x20);

    drv_epaper_cmd(epaper_dev,0x4E);        // set RAM x address count to 0;
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_cmd(epaper_dev,0x4F);        // set RAM y address count to 0X199;
    drv_epaper_data_byte(epaper_dev,0xC7,1);
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_read_busy(epaper_dev);
#endif

#ifdef EPAPER_1IN54_B
    drv_epaper_cmd(epaper_dev,0x01);        // POWER_SETTING
    drv_epaper_data_byte(epaper_dev,0x07,1);
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_data_byte(epaper_dev,0x08,1);
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_cmd(epaper_dev,0x06);        // BOOSTER_SOFT_START
    drv_epaper_data_byte(epaper_dev,0x07,1);
    drv_epaper_data_byte(epaper_dev,0x07,1);
    drv_epaper_data_byte(epaper_dev,0x07,1);
    drv_epaper_cmd(epaper_dev,0x04);        // POWER_ON

    drv_epaper_read_busy(epaper_dev);

    drv_epaper_cmd(epaper_dev,0X00);        // PANEL_SETTING
    drv_epaper_data_byte(epaper_dev,0xcf,1);
    drv_epaper_cmd(epaper_dev,0X50);        // VCOM_AND_DATA_INTERVAL_SETTING
    drv_epaper_data_byte(epaper_dev,0x37,1);// 0xF0
    drv_epaper_cmd(epaper_dev,0x30);        // PLL_CONTROL
    drv_epaper_data_byte(epaper_dev,0x39,1);
    drv_epaper_cmd(epaper_dev,0x61);        // TCON_RESOLUTION set x and y
    drv_epaper_data_byte(epaper_dev,0xC8,1);        // 200
    drv_epaper_data_byte(epaper_dev,0x00,1);        // y High eight: 0
    drv_epaper_data_byte(epaper_dev,0xC8,1);        // y Low eight: 200
    drv_epaper_cmd(epaper_dev,0x82);        // VCM_DC_SETTING_REGISTER
    drv_epaper_data_byte(epaper_dev,0x0E,1);

    drv_epaper_set_lut_bw(epaper_dev);;
    drv_epaper_set_lut_red(epaper_dev);
#endif

#ifdef EPAPER_1IN54_C
    drv_epaper_cmd(epaper_dev,0x06);       //boost soft start
    drv_epaper_data_byte(epaper_dev,0x17,1);
    drv_epaper_data_byte(epaper_dev,0x17,1);
    drv_epaper_data_byte(epaper_dev,0x17,1);
    drv_epaper_cmd(epaper_dev,0x04);

    drv_epaper_read_busy(epaper_dev);

    drv_epaper_cmd(epaper_dev,0x00);       //panel setting
    drv_epaper_data_byte(epaper_dev,0x0f,1);        //LUT from OTP£¬160x296
    drv_epaper_data_byte(epaper_dev,0x0d,1);        //VCOM to 0V fast

    drv_epaper_cmd(epaper_dev,0x61);       //resolution setting
    drv_epaper_data_byte(epaper_dev,0x98,1);        //152
    drv_epaper_data_byte(epaper_dev,0x00,1);         //152
    drv_epaper_data_byte(epaper_dev,0x98,1);

    drv_epaper_cmd(epaper_dev,0X50);       //VCOM AND DATA INTERVAL SETTING
    drv_epaper_data_byte(epaper_dev,0x77,1);         //WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

#endif
    return ret;
}

static rt_err_t drv_epaper_clear(epaper_device_t epaper_dev)
{
    rt_uint8_t *buff = (rt_uint8_t *)rt_malloc((epaper_dev->width) * (epaper_dev->height) * sizeof(rt_uint8_t));
    rt_uint8_t cmd[3];
    rt_memset(buff,0xFF,(epaper_dev->width) * (epaper_dev->height) * sizeof(rt_uint8_t));

    if(buff == RT_NULL)
    {
        LOG_E("Failed to request cache");
        return RT_ERROR;
    }
#ifdef EPAPER_1IN54_V2
    cmd[0] = 0x24;
#endif
#if (defined EPAPER_1IN54_B) || (defined EPAPER_1IN54_C)
    cmd[0] = 0x10;
    cmd[1] = 0x13;
    cmd[2] = 0x12;
#endif

    drv_epaper_cmd(epaper_dev,cmd[0]);
    drv_epaper_data_byte(epaper_dev,buff,(epaper_dev->width) * (epaper_dev->height));
#ifdef EPAPER_1IN54_B
    drv_epaper_data_byte(epaper_dev,buff,(epaper_dev->width) * (epaper_dev->height));
#endif
#if (defined EPAPER_1IN54_B) || (defined EPAPER_1IN54_C)
    drv_epaper_cmd(epaper_dev,cmd[1]);
    drv_epaper_data_byte(epaper_dev,buff,(epaper_dev->width) * (epaper_dev->height));
    rt_thread_mdelay(2);
    drv_epaper_cmd(epaper_dev,cmd[2]);
    drv_epaper_read_busy(epaper_dev);
#endif
#ifdef EPAPER_1IN54_V2
    drv_epaper_display_turn_on(epaper_dev);
#endif
}

static void drv_epaper_display_image(epaper_device_t epaper_dev,void *image)
{
#if (defined EPAPER_1IN54)
    drv_epaper_cmd(epaper_dev,0x24);
    drv_epaper_data_byte(epaper_dev,(rt_uint8_t*)image,(epaper_dev->width)*(epaper_dev->height));
    drv_epaper_display_turn_on(epaper_dev);
#endif

#if (defined EPAPER_1IN54_B) || (defined EPAPER_1IN54_C)
    epaper_image_t image_buff = (epaper_image_t*)image;
    drv_epaper_cmd(epaper_dev,0x10);
    #if (defined EPAPER_1IN54_B)
        drv_epaper_data_byte(epaper_dev,image_buff->black_image,(epaper_dev->width)*(epaper_dev->height)*2);
        rt_thread_mdelay(2);
    #endif
    #if (defined EPAPER_1IN54_C)
        drv_epaper_data_byte(epaper_dev,image_buff->black_image,(epaper_dev->width)*(epaper_dev->height));
    #endif
    drv_epaper_cmd(epaper_dev,0x13);
    drv_epaper_data_byte(epaper_dev,image_buff->other_image,(epaper_dev->width)*(epaper_dev->height));
    rt_thread_mdelay(2);
    drv_epaper_cmd(epaper_dev,0x12);
    drv_epaper_read_busy(epaper_dev);
#endif
}

static void drv_epaper_sleep(epaper_device_t epaper_dev)
{
#ifdef EPAPER_1IN54
    drv_epaper_cmd(0x10);       //enter deep sleep
    drv_epaper_data_byte(epaper_dev,0x01,1);
    rt_thread_mdelay(100);
#endif

#ifdef EPAPER_1IN54_B
    drv_epaper_cmd(epaper_dev,0x50);        // VCOM_AND_DATA_INTERVAL_SETTING
    drv_epaper_data_byte(epaper_dev,0x17,1);
    drv_epaper_cmd(epaper_dev,0x82);        // VCM_DC_SETTING_REGISTER
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_cmd(epaper_dev,0x01);        // POWER_SETTING
    drv_epaper_data_byte(epaper_dev,0x02,1);
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_data_byte(epaper_dev,0x00,1);
    drv_epaper_read_busy(epaper_dev);
    rt_thread_mdelay(1000);
    drv_epaper_cmd(epaper_dev,0x02);        // POWER_OFF
#endif

#ifdef EPAPER_1IN54_C
    drv_epaper_cmd(epaper_dev,0X02);    	//power off
    drv_epaper_read_busy(epaper_dev);
    drv_epaper_cmd(epaper_dev,0X07);    	//deep sleep
    drv_epaper_data_byte(epaper_dev,0xA5,1);
#endif
}
#if (defined EPAPER_1IN54)
static void drv_epaper_display_part_base_image(epaper_device_t epaper_dev,rt_uint8_t * image)
{
    drv_epaper_cmd(epaper_dev,0x24);
    drv_epaper_data_byte(epaper_dev,image,(epaper_dev->width)*(epaper_dev->height));

    drv_epaper_cmd(epaper_dev,0x26);
    drv_epaper_data_byte(epaper_dev,image,(epaper_dev->width)*(epaper_dev->height));
    drv_epaper_display_part_turn_on(epaper_dev);
}

static void drv_epaper_display_part_image(epaper_device_t epaper_dev,rt_uint8_t *image)
{
    drv_epaper_cmd(epaper_dev,0x24);
    drv_epaper_data_byte(epaper_dev,image,(epaper_dev->width)*(epaper_dev->height));
    drv_epaper_display_part_turn_on(epaper_dev);
}
#endif

static rt_err_t drv_epaper_open(rt_device_t dev, rt_uint16_t oflag)
{

    /* Not need */

    return RT_EOK;
}

static rt_err_t drv_epaper_close(rt_device_t dev)
{

    /* Not need */

    return RT_EOK;
}

static rt_size_t drv_epaper_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    
    /* Not need */

    return 0;
}

static rt_size_t drv_epaper_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    
    /* Not need */

    return 0;
}

static rt_err_t drv_epaper_control(rt_device_t dev, int cmd, void *args)
{
    rt_err_t ret = RT_EOK;
    epaper_device_t epaper = (epaper_device_t)dev;
    rt_base_t level;
    struct rt_device_rect_info* rect_info = (struct rt_device_rect_info*)args;

    RT_ASSERT(dev != RT_NULL);

    switch (cmd)
    {
    case EPAPER_CLEAR:
        drv_epaper_clear(epaper);
        break;

    case EPAPER_DISPLAY_IMAGE:
        drv_epaper_display_image(epaper,args);
        break;

    case EPAPER_SLEEP:
        drv_epaper_sleep(epaper);
        break;

#ifdef EPAPER_1IN54
    case EPAPER_DISPLAY_PART_BASE_IMAGE:
        drv_epaper_display_part_base_image(epaper,(rt_uint8_t*)args);
        break;
    case EPAPER_DISPLAY_PART_IMAGE:
        drv_epaper_display_part_image(epaper,(rt_uint8_t*)args);
        break;
#endif
    default:
        LOG_E("drv_lcd_control cmd: %d", cmd);
        break;
    }
    return ret;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops drv_epaper_ops =
{
    drv_epaper_init,
    drv_epaper_open,
    drv_epaper_close,
    drv_epaper_read,
    drv_epaper_write,
    drv_epaper_control
};
#endif

int rt_hw_epaper_init(void)
{
    rt_err_t ret = RT_EOK;
    epaper_device_t epaper_dev = (epaper_device_t)rt_malloc(sizeof(struct epaper_device));
    if (!epaper_dev)
    {
        return -1;
    }
    
#ifdef EPAPER_USE_SS0
    epaper_dev -> cs = SPI_CHIP_SELECT_0;
#endif
#ifdef EPAPER_USE_SS1
    epaper_dev -> cs = SPI_CHIP_SELECT_1;
#endif
#ifdef EPAPER_USE_SS2
    epaper_dev -> cs = SPI_CHIP_SELECT_2;
#endif
#ifdef EPAPER_USE_SS3
    epaper_dev -> cs = SPI_CHIP_SELECT_3;
#endif

    epaper_dev -> dc_io = EPAPER_DC_GPIO;
    epaper_dev -> rst_io = EPAPER_RST_GPIO;
    epaper_dev -> busy_io = EPAPER_BUSY_GPIO;
    epaper_dev -> spi_channel = SPI_DEVICE_1;
    epaper_dev->parent.type        = RT_Device_Class_Graphic;
    epaper_dev->parent.rx_indicate = RT_NULL;
    epaper_dev->parent.tx_complete = RT_NULL;
    epaper_dev -> width = (EPAPER_X_MAX % 8 ==0)?(EPAPER_X_MAX / 8):(EPAPER_X_MAX / 8 +1);
    epaper_dev -> height = EPAPER_Y_MAX;
#ifdef RT_USING_DEVICE_OPS
    epaper_dev->parent.ops        = &drv_epaper_ops;
#else
    epaper_dev->parent.init    = drv_epaper_init;
    epaper_dev->parent.open    = drv_epaper_open;
    epaper_dev->parent.close   = drv_epaper_close;
    epaper_dev->parent.read    = drv_epaper_read;
    epaper_dev->parent.write   = drv_epaper_write;
    epaper_dev->parent.control = drv_epaper_control;
#endif
    epaper_dev->parent.user_data = RT_NULL;

    ret = rt_device_register(&epaper_dev->parent, "epaper", RT_DEVICE_FLAG_STANDALONE);
    return ret;
}

INIT_DEVICE_EXPORT(rt_hw_epaper_init);
#endif