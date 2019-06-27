/*
 * Copyright (c) 2019 Sanggu Han
 */
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/gpio.h>
#include <asm/uaccess.h>
#include "huins.h"

#define GPIO_HOME       IMX_GPIO_NR(1, 11)
#define GPIO_BACK       IMX_GPIO_NR(1, 12)
#define GPIO_VOLUP      IMX_GPIO_NR(2, 15)
#define GPIO_VOLDOWN    IMX_GPIO_NR(5, 14)

static int device_open = 0;
static int cover_screen;
DECLARE_WAIT_QUEUE_HEAD(wait_queue);

static unsigned char *dot_addr;
static unsigned char *fnd_addr;
static unsigned char *led_addr;
static unsigned char *lcd_addr;

static int huins_open(struct node *, struct file *);
static int huins_release(struct node *, struct file *);
static long huins_ioctl(struct node *, unsigned int, unsigned long);

irqreturn_t home_handler(int, void *, struct pt_regs *);
irqreturn_t back_handler(int, void *, struct pt_regs *);
irqreturn_t volume_up_handler(int, void *, struct pt_regs *);
irqreturn_t volume_down_handler(int, void *, struct pt_regs *);

irqreturn_t home_handler(int irq, void *dev_id, struct pt_regs *regs)
{
        return IRQ_HANDLED;
}

irqreturn_t back_handler(int irq, void *dev_id, struct pt_regs *regs)
{
        __wake_up(&wait_queue, 1, 1, NULL);
        return IRQ_HANDLED;
}

irqreturn_t volume_up_handler(int irq, void *dev_id, struct pt_regs *regs)
{
        return IRQ_HANDLED;
}

irqreturn_t volume_down_handler(int irq, void *dev_id, struct pt_regs *regs)
{
        return IRQ_HANDLED;
}

static int huins_open(struct inode *inode,
                struct file *file)
{
	unsigned long irq_flag = IRQF_TRIGGER_RISING;

        if (device_open)
                return -EBUSY;

        init_waitqueue_head(&wait_queue);

        gpio_direction_input(GPIO_HOME);
        irq = gpio_to_irq(GPIO_HOME);
        ret = request_irq(irq, home_handler,
                        irq_flag, "HOME_BTN", NULL);
        if (ret)
                printk("ERROR: Cannot request IRQ %d\n - code %d\n", irq, ret);

        gpio_direction_input(GPIO_BACK);
        irq = gpio_to_irq(GPIO_BACK);
        ret = request_irq(irq, back_handler,
                        irq_flag, "BACK_BTN", NULL);
        if (ret)
                printk("ERROR: Cannot request IRQ %d\n - code %d\n", irq, ret);

        gpio_direction_input(GPIO_VOLUP);
        irq = gpio_to_irq(GPIO_VOLUP);
        ret = request_irq(irq, volume_up_handler,
                        irq_flag, "VOLUP_BTN", NULL);
        if (ret)
                printk("ERROR: Cannot request IRQ %d\n - code %d\n", irq, ret);

        gpio_direction_input(GPIO_VOLDOWN);
        irq = gpio_to_irq(GPIO_VOLDOWN);
        ret = request_irq(irq, volume_down_handler,
                        irq_flag, "VOLDOWN_BTN", NULL);
        if (ret)
                printk("ERROR: Cannot request IRQ %d\n - code %d\n", irq, ret);

        device_open++;
        try_module_get(THIS_MODULE);
        return SUCCESS;
}

static int huins_release(struct inode *inode,
                struct file *file)
{
        device_open--;

        free_irq(gpio_to_irq(GPIO_HOME), NULL);
        free_irq(gpio_to_irq(GPIO_BACK), NULL);
        free_irq(gpio_to_irq(GPIO_VOLUP), NULL);
        free_irq(gpio_to_irq(GPIO_VOLDOWN), NULL);

        module_put(THIS_MODULE);
        return SUCCESS;
}

static long huins_ioctl(struct file *file,
                unsigned int ioctl_num,
                unsigned long ioctl_param)
{
        int dummy = 139;
        switch (ioctl_num) {
        case IOCTL_SAMPLE:
                interruptible_sleep_on(&wait_queue);
                copy_to_user((void __user *)ioctl_param,
                                &dummy,
                                sizeof(dummy));
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
