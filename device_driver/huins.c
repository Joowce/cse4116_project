/*
 * Copyright (c) 2019 Sanggu Han
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/videodev2.h>
#include "huins.h"

#define ipu_fourcc(a,b,c,d)\
        (((__u32)(a)<<0)|((__u32)(b)<<8)|((__u32)(c)<<16)|((__u32)(d)<<24))
#define IPU_PIX_FMT_RGB565  ipu_fourcc('R','G','B','P') /*!< 16 RGB-5-6-5 */

static int device_open = 0;
static int cover_screen;

static unsigned char *dot_addr;
static unsigned char *fnd_addr;
static unsigned char *led_addr;
static unsigned char *lcd_addr;

static int huins_open(struct inode *, struct file *);
static int huins_release(struct inode *, struct file *);
static long huins_ioctl(struct file*, unsigned int, unsigned long);
static int huins_setup_capture(int *);
static int huins_capture(int, char *);
static int huins_cleanup_capture(int *);

static int huins_setup_capture(int *fd_v4l)
{
        struct v4l2_streamparm parm;
        struct v4l2_format fmt;
        struct v4l2_crop crop;
        int ret = 0;

        if ((*fd_v4l = open("/dev/video0", O_RDWR, 0)) < 0) {
                printk("Unable to open /dev/video0\n");
                return -1;
        }

        parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	parm.parm.capture.timeperframe.numerator = 1;
	parm.parm.capture.timeperframe.denominator = 30;
	parm.parm.capture.capturemode = 0;
	if ((ret = ioctl(*fd_v4l, VIDIOC_S_PARM, &parm)) < 0) {
		printk("VIDIOC_S_PARM failed\n");
		return ret;
	}

	crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	crop.c.left = 0;
	crop.c.top = 0;
	crop.c.width = 640;
	crop.c.height = 480;
	if ((ret = ioctl(*fd_v4l, VIDIOC_S_CROP, &crop)) < 0) {
		printk("set cropping failed\n");
		return ret;
	}

	memset(&fmt, 0, sizeof(fmt));
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.pixelformat = IPU_PIX_FMT_RGB565;
        fmt.fmt.pix.width = 640;
        fmt.fmt.pix.height = 480;
        fmt.fmt.pix.sizeimage = fmt.fmt.pix.width * fmt.fmt.pix.height * 16 / 8;
        fmt.fmt.pix.bytesperline = 640 * 2;

        if ((ret = ioctl(*fd_v4l, VIDIOC_S_FMT, &fmt)) < 0) {
                printk("set format failed\n");
                return ret;
        }

        return ret;
}

static int huins_capture(int fd_v4l, char *buf)
{
        struct v4l2_format fmt;
        int ret = 0;
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if ((ret = ioctl(fd_v4l, VIDIOC_G_FMT, &fmt)) < 0) {
                printk("get format failed\n");
                return ret;
        } else {
                printk("\t Width = %d\n", fmt.fmt.pix.width);
                printk("\t Height = %d\n", fmt.fmt.pix.height);
                printk("\t Image size = %d\n", fmt.fmt.pix.sizeimage);
                printk("\t Pixel format = %c%c%c%c\n",
                                (char)(fmt.fmt.pix.pixelformat & 0xFF),
                                (char)((fmt.fmt.pix.pixelformat & 0xFF00) >> 8),
                                (char)((fmt.fmt.pix.pixelformat & 0xFF0000) >> 16),
                                (char)((fmt.fmt.pix.pixelformat & 0xFF000000) >> 24));
        }

        memset(buf, 0, fmt.fmt.pix.sizeimage);

        if (read(fd_v4l, buf, fmt.fmt.pix.sizeimage) != fmt.fmt.pix.sizeimage) {
                printf("v4l2 read error.\n");
                return ret;
        }

	return ret;
}

static void huins_cleanup_capture(int *fd_v4l)
{
        close(*fd_v4l);
}

static int huins_open(struct inode *inode,
                struct file *file)
{
        if (device_open)
                return -EBUSY;

        device_open++;

        huins_clear_device();
        
        try_module_get(THIS_MODULE);
        return SUCCESS;
}

static int huins_release(struct inode *inode,
                struct file *file)
{
        device_open--;

        module_put(THIS_MODULE);
        return SUCCESS;
}

static long huins_ioctl(struct file *file,
                unsigned int ioctl_num,
                unsigned long ioctl_param)
{
	int op;
        switch (ioctl_num) {
        case IOCTL_RUN_DEVICE:
                copy_from_user(&op,
                                (void __user *)ioctl_param,
                                sizeof(op));
                break;
        case IOCTL_SET_SCREEN_COVER:
                cover_screen = 1;
                break;
        case IOCTL_UNSET_SCREEN_COVER:
                cover_screen = 0;
                break;
        case IOCTL_GET_SCREEN_COVER:
                copy_to_user((void __user *)ioctl_param,
                                &cover_screen,
                                sizeof(cover_screen));
                break;
        }

        return SUCCESS;
}

struct file_operations fops = {
        .unlocked_ioctl = huins_ioctl,
        .open = huins_open,
        .release = huins_release,
};

int init_module()
{
        int ret;
        ret = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);
        if (ret < 0) {
                printk(KERN_ALERT "%s failed with %d\n",
                                "Sorry, registering the character device ",
                                ret);
                return ret;
        }
	fnd_addr = ioremap(FND_ADDRESS, 0x4);
	led_addr = ioremap(LED_ADDRESS, 0x1);
	lcd_addr = ioremap(LCD_ADDRESS, 0x32);
	dot_addr = ioremap(DOT_ADDRESS, 0x10);
        return SUCCESS;
}

void cleanup_module()
{
	iounmap(fnd_addr);
	iounmap(led_addr);
	iounmap(lcd_addr);
	iounmap(dot_addr);
        unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sanggu Han");
