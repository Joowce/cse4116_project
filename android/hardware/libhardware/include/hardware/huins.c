#include <hardware/huins.h>
#include <malloc.h>
#include <cutils/log.h>
#include <string.h>
#include <sys/ioctl.h>
 
#define UNUSED(x) (void)(x)
 
static int open_huins(const struct hw_module_t *module,
                char const *name,
                struct hw_device_t **device);
static int close_huins(struct huins_device_t *dev);
void huins_set_dot_matrix(int m);
void huins_set_fnd(int n);
void huins_set_lcd(char *buf);
void huins_set_led(int bm);
void huins_set_buzzer(int buzz);
void huins_set_motor(int action, int direction, int speed);

void huins_set_dot_matrix(int m)
{
        int fd = open("/dev/huins_driver", O_RDWR);
        if (fd < 0) {
                ALOGI("Failed to open /dev/huins_driver\n");
                return;
        }

        ioctl(fd, IOCTL_SET_DOT_MATRIX, &m);

        close(fd);
}
 
void huins_set_fnd(int n)
{
        int fd = open("/dev/huins_driver", O_RDWR);
        if (fd < 0) {
                ALOGI("Failed to open /dev/huins_driver\n");
                return;
        }

        ioctl(fd, IOCTL_SET_FND, &n);

        close(fd);
}
 
void huins_set_lcd(char *buf)
{
        int fd = open("/dev/huins_driver", O_RDWR);
        if (fd < 0) {
                ALOGI("Failed to open /dev/huins_driver\n");
                return;
        }

        ioctl(fd, IOCTL_SET_LCD, buf);

        close(fd);
}
 
void huins_set_led(int bm)
{
        int fd = open("/dev/huins_driver", O_RDWR);
        if (fd < 0) {
                ALOGI("Failed to open /dev/huins_driver\n");
                return;
        }

        ioctl(fd, IOCTL_SET_LED, &bm);

        close(fd);
}
 
void huins_set_buzzer(int buzz)
{
        int fd = open("/dev/huins_driver", O_RDWR);
        if (fd < 0) {
                ALOGI("Failed to open /dev/huins_driver\n");
                return;
        }

        ioctl(fd, IOCTL_SET_BUZZER, &buzz);

        close(fd);
}
 
void huins_set_motor(int action, int direction, int speed)
{
        int fd = open("/dev/huins_driver", O_RDWR);
        int op[3];
        if (fd < 0) {
                ALOGI("Failed to open /dev/huins_driver\n");
                return;
        }

        op[0] = action;
        op[1] = direction;
        op[2] = speed;

        ioctl(fd, IOCTL_SET_MOTOR, op);

        close(fd);
}
 
static int close_huins(struct huins_device_t *dev)
{
    if(dev)
        free(dev);
 
    return 0;
}
 
static int open_huins(const struct hw_module_t *module,
                char const *name,
                struct hw_device_t **device)
{
    UNUSED(name);
 
    struct huins_device_t *dev = calloc(1, sizeof(struct huins_device_t));
    if (!dev)
            return -ENOMEM;
 
    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*)module;
    dev->common.close = (int (*)(struct hw_device_t*))close_huins;
 
    dev->set_dot_matrix = huins_set_dot_matrix;
    dev->set_fnd = huins_set_fnd;
    dev->set_lcd = huins_set_fcd;
    dev->set_led = huins_set_led;
    dev->set_buzzer = huins_set_buzzer;
    dev->set_motor = huins_set_motor;
    
    *device = (struct hw_device_t*)dev;
    puts("HAL for huins board initializes");
 
    return 0;
}
 
static struct hw_module_methods_t huins_module_methods = {
    .open = open_huins,
};
 
struct huins_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = HUINS_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = HUINS_MODULE_ID,
        .name = "HAL for control huins board",
        .author = "Sanggu Han",
        .methods = &huins_module_methods,
    }
};
