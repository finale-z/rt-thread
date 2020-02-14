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