//
// Created by 민지우 on 2019-06-27.
//
#include <jni.h>

int flag = 0;

extern "C" JNIEXPORT void JNICALL
Java_com_dcclab_huinssystemservice_HuinsSystemController_init (
    JNIEnv *env,
    jobject thiz
        ){

    /* TODO */
}

extern "C" JNIEXPORT jint JNICALL
Java_com_dcclab_huinssystemservice_HuinsSystemController_listen(
    JNIEnv *env,
    jobject thiz
        ){
    /* TODO */
    return (flag++) < 100 ? 0 : flag;
}

extern "C" JNIEXPORT void JNICALL
Java_com_dcclab_huinssystemservice_HuinsSystemController_end(
    JNIEnv *env,
    jobject thiz
        ){
    /* TODO */
}
