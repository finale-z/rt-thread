#ifndef _EPAPERCFG_H__
#define _EPAPERCFG_H__

#if (!defined EPAPER_1IN54_V2) || (defined EPAPER_1IN54_B) || (defined EPAPER_1IN54_C)
#define EPAPER_X_MAX 200
#define EPAPER_Y_MAX 200
#endif

#if (defined EPAPER_2IN13) || (defined EPAPER_2IN13_B) || (defined EPAPER_2IN13_C)
#define EPAPER_X_MAX 212
#define EPAPER_Y_MAX 104
#endif

#if (defined EPAPER_2IN7) || (defined EPAPER_2IN7_B) || (defined EPAPER_2IN7_C)
#define EPAPER_X_MAX 264
#define EPAPER_Y_MAX 176
#endif

#if (defined EPAPER_2IN9) || (defined EPAPER_2IN9_B) || (defined EPAPER_2IN9_C)
#define EPAPER_X_MAX 296
#define EPAPER_Y_MAX 128
#endif

#if (defined EPAPER_4IN2) || (defined EPAPER_4IN2_B) || (defined EPAPER_4IN2_C)
#define EPAPER_X_MAX 400
#define EPAPER_Y_MAX 300
#endif

#if (defined EPAPER_5IN83) || (defined EPAPER_5IN83_B) || (defined EPAPER_5IN83_C)
#define EPAPER_X_MAX 600
#define EPAPER_Y_MAX 480
#endif

#ifdef EPAPER_6IN0
#define EPAPER_X_MAX 800
#define EPAPER_Y_MAX 600
#endif

#if (defined EPAPER_6IN0_HD) || (defined EPAPER_6IN0_HD_TOUCH)
#define EPAPER_X_MAX 1448
#define EPAPER_Y_MAX 1072
#endif

#if (defined EPAPER_7IN5) || (defined EPAPER_7IN5_B) || (defined EPAPER_7IN5_C)
#define EPAPER_X_MAX 800
#define EPAPER_Y_MAX 480
#endif

#ifdef EPAPER_7IN8
#define EPAPER_X_MAX 1872
#define EPAPER_Y_MAX 1404
#endif

#ifdef EPAPER_9IN7
#define EPAPER_X_MAX 1200
#define EPAPER_Y_MAX 825
#endif

#ifdef EPAPER_10IN3_D
#define EPAPER_X_MAX 1872
#define EPAPER_Y_MAX 1404
#endif

#ifdef EPAPER_1IN54_B
const unsigned char epd_1in54b_lut_vcom0[] = {0x0E, 0x14, 0x01, 0x0A, 0x06, 0x04, 0x0A, 0x0A, 0x0F, 0x03, 0x03, 0x0C, 0x06, 0x0A, 0x00};
const unsigned char epd_1in54b_lut_w[] = {0x0E, 0x14, 0x01, 0x0A, 0x46, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x86, 0x0A, 0x04};
const unsigned char epd_1in54b_lut_b[] = {0x0E, 0x14, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x06, 0x4A, 0x04};
const unsigned char epd_1in54b_lut_g1[] = {0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04};
const unsigned char epd_1in54b_lut_g2[] = {0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04};
const unsigned char epd_1in54b_lut_vcom1[] = {0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char epd_1in54b_lut_red0[] = {0x83, 0x5D, 0x01, 0x81, 0x48, 0x23, 0x77, 0x77, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char epd_1in54b_lut_red1[] = {0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif




enum EPAPER_CONFIG
{
    EPAPER_CLEAR = 0,
    EPAPER_DISPLAY_IMAGE,
    EPAPER_SLEEP,
#ifdef EPAPER_1IN54
    EPAPER_DISPLAY_PART_BASE_IMAGE,
    EPAPER_DISPLAY_PART_IMAGE,
#endif
    EPAPER_CONFIG_END
};
#endif