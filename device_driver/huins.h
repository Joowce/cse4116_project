/*
 * Copyright (c) 2019 Sanggu Han
 */
#ifndef _HUINS_H_
#define _HUINS_H_

#include <linux/ioctl.h>

#define DEVICE_NAME     "/dev/huins_board"
#define MAJOR_NUM       242

#define SUCCESS 0

#define FND_ADDRESS 0x08000004
#define LED_ADDRESS 0x08000016
#define LCD_ADDRESS 0x08000090
#define DOT_ADDRESS 0x08000210

#define IOCTL_RUN_DEVICE _IOR(MAJOR_NUM, 0, int)

#endif
