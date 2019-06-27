/*
 * Copyright (c) 2019 Sanggu Han
 */
#ifndef _HUINS_H_
#define _HUINS_H_

#include <linux/ioctl.h>

#define DEVICE_NAME     "/dev/huins_driver"
#define MAJOR_NUM       242

#define SUCCESS 0

#define FND_ADDRESS 0x08000004
#define LED_ADDRESS 0x08000016
#define LCD_ADDRESS 0x08000090
#define DOT_ADDRESS 0x08000210

#define IOCTL_SAMPLE _IOR(MAJOR_NUM, 0, int)
#define IOCTL_SET_SCREEN_COVER _IO(MAJOR_NUM, 1)
#define IOCTL_UNSET_SCREEN_COVER _IO(MAJOR_NUM, 2)
#define IOCTL_GET_SCREEN_COVER _IOR(MAJOR_NUM, 3, int)

#endif
