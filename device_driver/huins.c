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
#include <linux/workqueue.h>
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
static unsigned char *buzzer_addr;
static unsigned char *motor_addr;

static void update_dot_matrix(int);
static void update_fnd(int);
static void update_lcd(char *);
static void update_led(int);
static void update_buzzer(int);
static void update_motor(int);
static int huins_open(struct inode *, struct file *);
static int huins_release(struct inode *, struct file *);
static long huins_ioctl(struct file *, unsigned int, unsigned long);

irqreturn_t home_handler(int, void *, struct pt_regs *);
irqreturn_t back_handler(int, void *, struct pt_regs *);
irqreturn_t volume_up_handler(int, void *, struct pt_regs *);
irqreturn_t volume_down_handler(int, void *, struct pt_regs *);

static work_func_t work_queue_func(void *);

DECLARE_WORK(work_queue, work_queue_func);

static work_func_t work_queue_func(void *data)
{
        return SUCCESS;
}

irqreturn_t home_handler(int irq, void *dev_id, struct pt_regs *regs)
{
        return IRQ_HANDLED;
}

irqreturn_t back_handler(int irq, void *dev_id, struct pt_regs *regs)
{
        schedule_work(&work_queue);
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

static void update_dot_matrix(int data)
{
	//TODO: implement this
}

static void update_fnd(int data)
{
	int i, state = 0;
	data %= 10000;
	for (i = 0; i < 4; i++) {
		state |= (data % 10) << (i * 4);
		data /= 10;
	}
	outw(state, (unsigned int)fnd_addr);
}

static void update_lcd(char *buf)
{
	int i;
	unsigned short int state;
	for (i = 0; i < 32; i += 2) {
		if (!buf[i])
			buf[i] = ' ';
		if (!buf[i + 1])
			buf[i + 1] = ' ';
		state = (buf[i] << 8) | buf[i + 1];
		outw(state, (unsigned int)lcd_addr + i);
	}
}

static void update_led(int data)
{
	int i, state = 0;
	for (i = 0; i < 8; i++) {
		state |= (data & 1) << (7 - i);
		data >>= 1;
	}
	outw(state, (unsigned int)led_addr);
}

static void update_buzzer(int data)
{
	char state = (char) data & 0xF;
	outw(state, (unsigned int)buzzer_addr);
}

static void update_motor(int data)
{
	int i, state = data & 0x0F0FFF;
	for (i = 4; i >= 0; i -= 2) {
		outw(state & 0xFF, (unsigned int)motor_addr + i);
		state >>= 1;
	}
}

static int huins_open(struct inode *inode,
                struct file *file)
{
	int irq, ret;
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

        cancel_work_sync(&work_queue);

        module_put(THIS_MODULE);
        return SUCCESS;
}

static long huins_ioctl(struct file *file,
                unsigned int ioctl_num,
                unsigned long ioctl_param)
{
	int data;
	char buf[32];

        switch (ioctl_num) {
        case IOCTL_SET_DOT_MATRIX:
                break;
        case IOCTL_SET_FND:
		copy_from_user(&data,
				(void __user *)ioctl_param,
				sizeof(data));
		update_fnd(data);
                break;
        case IOCTL_SET_LCD:
		copy_from_user(buf,
				(void __user *)ioctl_param,
				sizeof(char) * 32);
		update_lcd(buf);
		break;
        case IOCTL_SET_LED:
		copy_from_user(&data,
				(void __user *)ioctl_param,
				sizeof(data));
		update_led(data);
                break;
        case IOCTL_SET_BUZZER:
		copy_from_user(&data,
				(void __user *)ioctl_param,
				sizeof(data));
		update_buzzer(data);
                break;
        case IOCTL_SET_MOTOR:
		copy_from_user(&data,
				(void __user *)ioctl_param,
				sizeof(data));
		update_motor(data);
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
	buzzer_addr = ioremap(BUZZER_ADDRESS, 0x1);
	motor_addr = ioremap(MOTOR_ADDRESS, 0x4);
        return SUCCESS;
}

void cleanup_module()
{
	iounmap(fnd_addr);
	iounmap(led_addr);
	iounmap(lcd_addr);
	iounmap(dot_addr);
	iounmap(buzzer_addr);
	iounmap(motor_addr);
        unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sanggu Han");
