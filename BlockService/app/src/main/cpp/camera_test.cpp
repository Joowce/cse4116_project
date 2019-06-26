//
// Created by root on 19. 6. 27.
//

#include <jni.h>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>

#include <vector>

#include <android/log.h>

#define LOG_TAG "FaceDetection/DetectionBasedTracker"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;

extern "C" JNIEXPORT jstring JNICALL
Java_com_dcclab_blockservice_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    VideoCapture capture;
    Mat Frame;
    int flag = capture.open(2);
    LOGD("flag: %d", flag);
    if(flag) {
        capture >> Frame;
        if (Frame.data[0]) {
            LOGD("Frame %c", Frame.data[0]);
        }
    } else {
        LOGD("no camera");
    }
    return env->NewStringUTF(hello.c_str());
}


