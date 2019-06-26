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
#include "huins.h"

static int device_open = 0;
static int cover_screen;

static unsigned char *dot_addr;
static unsigned char *fnd_addr;
static unsigned char *led_addr;
static unsigned char *lcd_addr;

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
