//
// Created by 熊峰 on 2019-12-24.
//
#include <jni.h>
#include <android/log.h>
#include "lame.h"

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_lamedemo_MainActivity_hello(JNIEnv *env, jobject thiz) {
    __android_log_print(ANDROID_LOG_ERROR, "Encoder", "hello");
    const char *string = get_lame_url();
    return env->NewStringUTF(string);
}
