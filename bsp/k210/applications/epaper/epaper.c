#include <rthw.h>
#include <rtthread.h>

#ifdef USING_EPAPER
#include "epaper.h"
#include <drv_io_config.h>
#include <spi.h>
#include <gpiohs.h>

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
} * epaper_device_t;

static void drv_epaper_cmd(epaper_device_t epaper, rt_uint8_t cmd)
{
    gpiohs_set_io(epaper->dc_io, GPIO_PV_LOW);
    spi_init(epaper->spi_channel, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 1);
    spi_send_data_normal_dma(epaper->dma_channel, epaper->spi_channel, epaper->cs, &cmd, 1, SPI_TRANS_CHAR);
}

static void drv_epaper_data_byte(epaper_device_t epaper, rt_uint8_t *data_buf, rt_uint32_t length)
{
    gpiohs_set_pin(epaper->dc_io, GPIO_PV_HIGH);
    spi_init(epaper->spi_channel, SPI_WORK_MODE_0, SPI_FF_STANDARD, 8, 1);
    spi_send_data_normal_dma(epaper->dma_channel, epaper->spi_channel, epaper->cs, data_buf, length, SPI_TRANS_CHAR);
}

static void drv_epaper_data_half_word(epaper_device_t epaper, rt_uint16_t *data_buf, rt_uint32_t length)
{
    gpiohs_set_pin(epaper->dc_io, GPIO_PV_HIGH);
    spi_init(epaper->spi_channel, SPI_WORK_MODE_0, SPI_FF_STANDARD, 16, 1);
    spi_send_data_normal_dma(epaper->dma_channel, epaper->spi_channel, epaper->cs, data_buf, length, SPI_TRANS_SHORT);
}

static void drv_epaper_data_word(epaper_device_t epaper, rt_uint32_t *data_buf, rt_uint32_t length)
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
    while(drv_pin_read(epaper_io->parent,epaper_io->busy_io))
    {
        rt_thread_mdelay(100);
    }
}

static void epaper_display_turn_on(epaper_device_t epaper_dev)
{
    drv_epaper_cmd(epaper_dev,0x22);
    drv_epaper_data_byte(epaper_dev,0xF7);
    drv_epaper_cmd(epaper_dev,0x20);
    drv_epaper_read_busy(epaper_dev);   
}

static void epaper_display_part_turn_on(epaper_device_t epaper_dev)
{
    drv_epaper_cmd(epaper_dev,0x22);
    drv_epaper_data_byte(epaper_dev,0xFF);
    drv_epaper_cmd(epaper_dev,0x20);
    drv_epaper_read_busy(epaper_dev);   
}

static drv_epaper_init(epaper_device_t epaper_dev)
{
    drv_epaper_reset(epaper_dev);

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
}

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

    ret = rt_device_register(&epaper_dev->parent, "epaper", RT_DEVICE_FLAG_RDWR);
    return ret;
}

INIT_DEVICE_EXPORT(rt_hw_epaper_init);
#endif