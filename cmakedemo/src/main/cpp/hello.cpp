#include <jni.h>
#include "test.h"
#include <android/log.h>
#include <pthread.h>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_cmakedemo_MainActivity_hello(JNIEnv *env, jobject thiz) {
    //调用第三方so库
    int result = test();
    pthread_t pid;
    pthread_create(&pid, 0, 0, 0);
    __android_log_print(ANDROID_LOG_ERROR, "JNI", "result: %d", result);
    return env->NewStringUTF("Hello from native");
}
