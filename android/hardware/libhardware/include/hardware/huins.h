#ifndef ANDROID_HUINS_INTERFACE_H
#define ANDROID_HUINS_INTERFACE_H
 
#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>
 
#include <hardware/hardware.h>

__BEGIN_DECLS


#define HUINS_API_VERSION_0_1 HARDWARE_MODULE_API_VERSION(0, 1)
#define HUINS_MODULE_ID "huins"


struct huins_device_t {
	struct hw_device_t common;

	void (*set_dot_matrix)(int m);
	void (*set_fnd)(int n);
	void (*set_lcd)(char *buf);
	void (*set_led)(int bm);
	void (*set_buzzer)(int buzz);
	void (*set_motor)(int action, int direction, int speed); 
}

/**
 * Every hardware module must have a data structure named HAL_MODULE_INFO_SYM
 * and the fields of this data structure must begin with hw_module_t
 * followed by module specific information.
 */
struct huins_module_t {
	struct hw_module_t common;
};


__END_DECLS




#endif
