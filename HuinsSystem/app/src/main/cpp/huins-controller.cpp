//
// Created by 민지우 on 2019-06-27.
//

#define LOG_TAG "HuinsSystemService"
#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>

#include <stdio.h>

namespace android{
    int flag = 0;

    static void init_native (
            JNIEnv *env,
            jobject thiz
    ){

        /* TODO */
    }

    static int listen_native(
            JNIEnv *env,
            jobject thiz
    ){
        /* TODO */
        return (flag++) < 100 ? 0 : flag;
    }

    static void end_native(
            JNIEnv *env,
            jobject thiz
    ){
        /* TODO */
    }

    static JNINativeMethod method_table[] = {
            { "init_native", "()V", (void*)init_native },
            { "end_native", "()V", (void*)end_native },
            { "listen_native", "()I", (void*)listen_native}
    };

    int register_android_server_HuinsSystemService(JNIEnv *env){
        return jniRegisterNativeMethods(env, "", /* TODO java system service class path ex: com/android/server/LightsService */
                method_table, NELEM(method_table));
    }
};


