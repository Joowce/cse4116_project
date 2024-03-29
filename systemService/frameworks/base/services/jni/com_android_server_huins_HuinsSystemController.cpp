/*
 * Created by 민지우 on 2019-06-27.
 *
 * for Huins System FPGA
 *
 * format
 * ioctl(fd, IOCTL_COMMAND, &data)
 */




#define LOG_TAG "HuinsSystemService"
#include "jni.h"
#include "JNIHelp.h"
#include "com_android_server_huins_HuinsSystemController.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

namespace android{

#define MATRIX_ROW 10
#define MATRIX_COL 7
#define LCD_MAX_LEN 32
#define LED_NUM 8
#define FPGA_SWITCH_NUM 9

//-------------------------------------------------

    static jint initOutputDevices_native (
            JNIEnv *env,
            jobject thiz
    ){

        jint fd = open("/dev/huins_driver", O_RDWR);
        if (fd == -1) {
            ALOGE("Unable to open huins driver");
        }
        return fd;
    }

    static jint initInputDevices_native (
            JNIEnv *env,
            jobject thiz
            ){
        jint fd = open("/dev/fpga_push_switch", O_RDWR);
        if (fd == -1) {
            ALOGE("Unable to open fpga push switch");
        }
        return fd;
    }

    static jbooleanArray getSwitchStatus_native(
            JNIEnv *env,
            jobject thiz,
            jint fd
    ){
        unsigned char pushed[FPGA_SWITCH_NUM];
        memset(pushed, 0, sizeof(pushed));
        jbooleanArray finalPushed = env->NewBooleanArray(FPGA_SWITCH_NUM);
        jboolean temp[FPGA_SWITCH_NUM];
        read(fd, &pushed, sizeof(pushed));

        memset(temp, 0, sizeof(temp));
        for(int i = 0; i < FPGA_SWITCH_NUM; i++) {
            temp[i] = (jboolean) (pushed > 0);
        }
        env->SetBooleanArrayRegion(finalPushed, 0, FPGA_SWITCH_NUM, temp);
        return finalPushed;
    }

    static void writeDotMatrix_native(
            JNIEnv *env,
            jobject thiz,
            jint fd,
            jbooleanArray matrix
            ){

        int len = env -> GetArrayLength(matrix);
        if (len != MATRIX_ROW * MATRIX_COL) {
            ALOGE("matrix length should be %d", MATRIX_COL * MATRIX_ROW);
            return;
        }

        jboolean * mat;
        mat = env -> GetBooleanArrayElements(matrix, false);
        char data[len];

        for (int i=0; i < len; i++) {
            data[i] = (char) (mat[i] ? 1 : 0);
        }

        int result = ioctl(fd, IOCTL_SET_DOT_MATRIX, data);
        if (result < 0) {
            ALOGE("Unable to write Dot Matrix");
        }
        env -> ReleaseBooleanArrayElements(matrix, mat, NULL);
    }

    static void writeFND_native(
            JNIEnv *env,
            jobject thiz,
            jint fd,
            jint fnd
            ) {
        int result = ioctl(fd, IOCTL_SET_FND, fnd);
        if (result < 0) {
            ALOGE("Unable to write FND %d", &fnd);
        }
    }

    static void writeLCD_native(
            JNIEnv *env,
            jobject thiz,
            jint fd,
            jstring str
            ){
        const char* tempStr = env->GetStringUTFChars(str, NULL);
        char finalStr[LCD_MAX_LEN] = {0};

        int strLen = strlen(tempStr);
        int len = strLen < LCD_MAX_LEN ? strLen : LCD_MAX_LEN;
        int start = strLen > LCD_MAX_LEN ? strLen : 0;
        
        for (int i = 0; i < len; i++) {
            finalStr[i] = tempStr[start + i];
        }
        if (ioctl(fd, IOCTL_SET_LCD, finalStr) < 0) {
            ALOGE("Unable to write LCD %s", finalStr);
        }
        env->ReleaseStringUTFChars(str, tempStr);
    }

    static void writeLED_native(
            JNIEnv *env,
            jobject thiz,
            jint fd,
            jbooleanArray _led
            ) {
        int len = env->GetArrayLength(_led);
        jboolean* led = env->GetBooleanArrayElements(_led, NULL);
        unsigned int mask = 0;
        
        for (int i = 0; i < LED_NUM; i++) {
            mask <<= 1;
            mask |=  (bool)led[i] ? 1 : 0;
        }
        ioctl(fd, IOCTL_SET_LED, &mask);
        env->ReleaseBooleanArrayElements(_led, led, NULL);
    }

    static void writeBUZZER_native(
            JNIEnv *env,
            jobject thiz,
            jint fd,
            jint buzz
            ) {
        ioctl(fd, IOCTL_SET_BUZZER, &buzz);
    }

    static void writeMOTOR_native(
            JNIEnv *env,
            jobject thiz,
            jint fd,
            jint action,
            jint direction,
            jint speed
            ) {
        int op[3];
        op[0] = action;
        op[1] = direction;
        op[2] = speed;

        ioctl(fd, IOCTL_SET_MOTOR, op);
    }

    static void endInputDevices_native(
            JNIEnv *env,
            jobject thiz,
            jint fd
    ){
        close(fd);
    }

    static void endOutputDevices_native(
            JNIEnv *env,
            jobject thiz,
            jint fd
            ){
        close(fd);
    }

//------------------------------------------------------------------------

    static JNINativeMethod method_table[] = {
            { "initInputDevices_native", "()I", (void*)initInputDevices_native },
            { "initOutputDevices_native", "()I", (void*)initOutputDevices_native },
            { "getSwitchStatus_native", "(I)[Z", (void*)getSwitchStatus_native },
            { "writeDotMatrix_native", "(I[Z)V", (void*)writeDotMatrix_native },
            { "writeFND_native", "(II)V", (void*)writeFND_native },
            { "writeLCD_native", "(ILjava/lang/String;)V", (void*)writeLCD_native },
            { "writeLED_native", "(I[Z)V", (void*)writeLED_native },
            { "writeBUZZER_native", "(II)V", (void*)writeBUZZER_native },
            { "writeMOTOR_native", "(IIII)V", (void*)writeMOTOR_native },
            { "endInputDevices_native", "(I)V", (void*)endInputDevices_native },
            { "endOutputDevices_native", "(I)V", (void*)endOutputDevices_native }
    };

    int register_android_server_HuinsSystemController(JNIEnv *env){
        return jniRegisterNativeMethods(env, "com/android/server/HuinsSystemController", /* TODO java system service class path ex: com/android/server/LightsService */
                method_table, NELEM(method_table));
    }
};


