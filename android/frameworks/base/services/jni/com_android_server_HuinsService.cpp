#define LOG_TAG "huins"
 
#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"
 
#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware/hardware.h>
#include <hardware/huins.h>
 
#include <stdio.h>
 
 
namespace android
{
 
    huins_device_t *huins_dev = NULL;
 
 
    static jlong init_native(JNIEnv *env, jobject clazz)
    {
        int err;
        huins_module_t *module;
 
        err = hw_get_module(HUINS_MODULE_ID, (hw_module_t const**)&module);
        if(err == 0)
        {
            if(module->common.methods->open((hw_module_t*)module, "", ((hw_device_t**)&huins_dev)) != 0)
                return 0;
        }
 
        long tmp = reinterpret_cast<long>(huins_dev);
 
        return tmp;
    }
 
    static void finalize_native(JNIEnv *env, jobject clazz, jlong ptr)
    {
        huins_device_t *dev = reinterpret_cast<huins_device_t*>(ptr);
        if(dev == NULL)
            return;
 
        free(dev);
    }
 
    static void set_dot_matrix_native(JNIEnv *env, jobject clazz, jint m)
    {
        if(huins_dev == NULL)
            return;
 
        huins_dev->set_dot_matrix(m);
    }
 
    static void set_fnd_native(JNIEnv *env, jobject clazz, jint n)
    {
        if(huins_dev == NULL)
            return;
 
        huins_dev->set_fnd(n);
    }
 
    static void set_lcd_native(JNIEnv *env, jobject clazz, jbyteArray _buf)
    {
        jbyte *buf;

        buf = env->GetByteArrayElements(_buf, NULL);

        if(huins_dev == NULL)
            return;
 
        huins_dev->set_lcd((char *)buf);

        env->ReleaseByteArrayElements(_buf, buf, 0);
    }
 
    static void set_led_native(JNIEnv *env, jobject clazz, jint bm)
    {
        if(huins_dev == NULL)
            return;
 
        huins_dev->set_led(bm);
    }
 
    static void set_buzzer_native(JNIEnv *env, jobject clazz, jint buzz)
    {
        if(huins_dev == NULL)
            return;
 
        huins_dev->set_buzzer(buzz);
    }
 
    static void set_motor_native(JNIEnv *env,
                        jobject clazz,
                        jint action,
                        jint direction,
                        jint speed)
    {
        if(huins_dev == NULL)
            return;
 
        huins_dev->set_motor(action, direction, speed);
    }
 
    static JNINativeMethod method_table[] =
    {
        {"init_native", "()J", (void*)init_native},
        {"finalize_native", "(J)V", (void*)finalize_native},
        {"set_dot_matrix_native", "(I)V", (void *)set_dot_matrix_native},
        {"set_fnd_native", "(I)V", (void *)set_fnd_native},
        {"set_lcd_native", "([B)V", (void *)set_lcd_native},
        {"set_led_native", "(I)V", (void *)set_led_native},
        {"set_buzzer_native", "(I)V", (void *)set_buzzer_native},
        {"set_motor_native", "(III)V", (void *)set_motor_native},
    };
 
    int register_android_server_HuinsService(JNIEnv *env)
    {
        return jniRegisterNativeMethods(env, "com/android/server/HuinsService",
                                        method_table, NELEM(method_table));
    }
 
};
