#ifndef RT_CONFIG_H__
#define RT_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Project Configuration */

/* RT-Thread Kernel */

#define RT_NAME_MAX 8
#define RT_USING_SMP
#define RT_CPUS_NR 2
#define RT_ALIGN_SIZE 8
#define RT_THREAD_PRIORITY_32
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 100
#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_USING_IDLE_HOOK
#define RT_IDLE_HOOK_LIST_SIZE 4
#define IDLE_THREAD_STACK_SIZE 8192
#define RT_DEBUG
#define RT_DEBUG_COLOR
#define RT_DEBUG_INIT_CONFIG
#define RT_DEBUG_INIT 1

/* Inter-Thread communication */

#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE

/* Memory Management */

#define RT_USING_MEMPOOL
#define RT_USING_SLAB
#define RT_USING_HEAP

/* Kernel Device Object */

#define RT_USING_DEVICE
#define RT_USING_DEVICE_OPS
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 128
#define RT_CONSOLE_DEVICE_NAME "uarths"
#define RT_VER_NUM 0x40003
#define ARCH_CPU_64BIT
#define ARCH_RISCV
#define ARCH_RISCV64

/* RT-Thread Components */

#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE 8192
#define RT_MAIN_THREAD_PRIORITY 10

/* C++ features */


/* Command shell */

#define RT_USING_FINSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#define FINSH_THREAD_PRIORITY 20
#define FINSH_THREAD_STACK_SIZE 16384
#define FINSH_CMD_SIZE 80
#define FINSH_USING_MSH
#define FINSH_USING_MSH_DEFAULT
#define FINSH_ARG_MAX 10

/* Device virtual file system */

#define RT_USING_DFS
#define DFS_USING_WORKDIR
#define DFS_FILESYSTEMS_MAX 16
#define DFS_FILESYSTEM_TYPES_MAX 16
#define DFS_FD_MAX 64
#define RT_USING_DFS_ELMFAT

/* elm-chan's FatFs, Generic FAT Filesystem Module */

#define RT_DFS_ELM_CODE_PAGE 437
#define RT_DFS_ELM_WORD_ACCESS
#define RT_DFS_ELM_USE_LFN_3
#define RT_DFS_ELM_USE_LFN 3
#define RT_DFS_ELM_MAX_LFN 255
#define RT_DFS_ELM_DRIVES 2
#define RT_DFS_ELM_MAX_SECTOR_SIZE 4096
#define RT_DFS_ELM_REENTRANT
#define RT_USING_DFS_DEVFS

/* Device Drivers */

#define RT_USING_DEVICE_IPC
#define RT_PIPE_BUFSZ 512
#define RT_USING_SERIAL
#define RT_SERIAL_USING_DMA
#define RT_SERIAL_RB_BUFSZ 64
#define RT_USING_I2C
#define RT_USING_I2C_BITOPS
#define RT_USING_PIN
#define RT_USING_SDIO
#define RT_SDIO_STACK_SIZE 512
#define RT_SDIO_THREAD_PRIORITY 15
#define RT_MMCSD_STACK_SIZE 1024
#define RT_MMCSD_THREAD_PREORITY 22
#define RT_MMCSD_MAX_PARTITION 16
#define RT_USING_SPI
#define RT_USING_QSPI
#define RT_USING_SPI_MSD
#define RT_USING_SFUD
#define RT_SFUD_USING_SFDP
#define RT_SFUD_USING_FLASH_INFO_TABLE
#define RT_SFUD_USING_QSPI
#define RT_SFUD_SPI_MAX_HZ 50000000
#define RT_DEBUG_SFUD
#define RT_USING_SENSOR
#define RT_USING_SENSOR_CMD
#define RT_USING_HWCRYPTO
#define RT_HWCRYPTO_DEFAULT_NAME "hwcryto"
#define RT_HWCRYPTO_IV_MAX_SIZE 16
#define RT_HWCRYPTO_KEYBIT_MAX_SIZE 256
#define RT_HWCRYPTO_USING_AES
#define RT_HWCRYPTO_USING_AES_ECB
#define RT_HWCRYPTO_USING_AES_CBC
#define RT_HWCRYPTO_USING_AES_CFB
#define RT_HWCRYPTO_USING_AES_CTR
#define RT_HWCRYPTO_USING_AES_OFB

/* Using USB */


/* POSIX layer and C standard library */

#define RT_USING_LIBC
#define RT_USING_POSIX

/* Network */

/* Socket abstraction layer */


/* Network interface device */


/* light weight TCP/IP stack */


/* AT commands */


/* VBUS(Virtual Software BUS) */


/* Utilities */


/* RT-Thread online packages */

/* IoT - internet of things */


/* Wi-Fi */

/* Marvell WiFi */


/* Wiced WiFi */


/* IoT Cloud */


/* security packages */


/* language packages */


/* multimedia packages */

#define PKG_USING_MUPDF
#define PKG_USING_MUPDF_V120

/* tools packages */


/* system packages */

#define PKG_USING_GUIENGINE
#define PKG_USING_GUIENGINE_LATEST_VERSION
#define PKG_USING_RGB888_PIXEL_BITS_32
#define PKG_USING_RGB888_PIXEL_BITS 32
#define GUIENGINE_CMD_STRING_MAX 16
#define GUIENGINE_USING_FONT16
#define GUIENGINE_USING_FONT12
#define GUIENGINE_USING_PNG
#define GUIENGINE_IMAGE_LODEPNG
#define GUIENGINE_IMAGE_CONTAINER
#define GUIENGINE_NAME_MAX 16

/* peripheral libraries and drivers */

#define PKG_USING_SENSORS_DRIVERS
#define PKG_USING_BME280
#define PKG_USING_BME280_LATEST_VERSION
#define PKG_USING_BMI160_BMX160
#define PKG_USING_BMI160_BMX160_LATEST_VERSION
#define PKG_USING_KENDRYTE_SDK
#define PKG_USING_KENDRYTE_SDK_V055
#define PKG_KENDRYTE_SDK_VERNUM 0x0055
#define PKG_USING_ROSSERIAL
#define ROSSERIAL_USE_SERIAL
#define ROSSERIAL_UART_NAME "uart2"

/* roserial examples */


/* You may only choose one example at a time */

#define PKG_USING_ROSSERIAL_MLATEST_VERSION
#define PKG_ROSSERIAL_VER_NUM 0x29999
#define PKG_USING_I2C_TOOLS
#define PKG_USING_I2C_TOOLS_LATEST_VERSION

/* miscellaneous packages */


/* samples: kernel and components samples */

#define PKG_USING_ULAPACK
#define ULAPACK_USE_DYNAMIC_ALLOC
#define PKG_USING_ULAPACK_LATEST_VERSION
#define PKG_ULAPACK_VER_NUM 0x99999
#define PKG_USING_UKAL
#define UKAL_MAX_STATE_VECTOR_SIZE 5
#define UKAL_MAX_MEASUREMENT_VECTOR_SIZE 5

/* Dymamic memory allocation selected from ulapack */

#define PKG_USING_UKAL_LATEST_VERSION
#define PKG_UKAL_VER_NUM 0x99999
#define BOARD_K210_EVB

/* WaveShare */

#define USING_EPAPER
#define EPAPER_1IN54_C
#define EPAPER_USE_SS0
#define EPAPER_DC_PIN 19
#define EPAPER_DC_GPIO 1
#define EPAPER_RST_PIN 33
#define EPAPER_RST_GPIO 2
#define EPAPER_BUSY_PIN 28
#define EPAPER_BUSY_GPIO 3
#define BSP_USING_UART_HS
#define BSP_USING_I2C
#define BSP_USING_I2C0
#define BSP_I2C0_CLK_PIN 0
#define BSP_I2C0_SDA_PIN 1
#define I2C0_SLAVE_ADDR 0x00
#define I2C0_ADDR_WIDTH 7
#define I2C0_CLK_FREQ 200000
#define BSP_SPI1_USING_SS0
#define __STACKSIZE__ 4096

#endif
