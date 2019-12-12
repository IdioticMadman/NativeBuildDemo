#include <jni.h>
#include <string>
#include "MyFFmpeg.h"
#include "macro.h"

extern "C" {
#include "libavcodec/avcodec.h"
}
MyFFmpeg *myFFmpeg = nullptr;

static JavaVM *_vm;

int JNI_OnLoad(JavaVM *vm, void *reverse) {
    _vm = vm;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_player_MyPlayer_nativePrepare(JNIEnv *env, jobject thiz, jstring data_source) {
    const char *dataSource = env->GetStringUTFChars(data_source, JNI_FALSE);
    auto callHelper = new JavaCallHelper(_vm, env, thiz);
    myFFmpeg = new MyFFmpeg(callHelper, dataSource);

    myFFmpeg->prepare();

    env->ReleaseStringUTFChars(data_source, dataSource);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_player_MyPlayer_nativeStart(JNIEnv *env, jobject thiz) {
    myFFmpeg->start();
}

