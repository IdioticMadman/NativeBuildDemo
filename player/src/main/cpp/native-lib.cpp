#include <jni.h>
#include <string>
#include "MyFFmpeg.h"
#include <android/native_window_jni.h>
#include "macro.h"

MyFFmpeg *myFFmpeg = nullptr;
ANativeWindow *window = nullptr;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static JavaVM *_vm;

int JNI_OnLoad(JavaVM *vm, void *reverse) {
    _vm = vm;
    return JNI_VERSION_1_6;
}

void render(uint8_t *data, int lineSize, int w, int h) {
    pthread_mutex_lock(&mutex);
    if (!window) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    ANativeWindow_setBuffersGeometry(window, w, h, WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer windowBuffer;
    if (ANativeWindow_lock(window, &windowBuffer, nullptr)) {
        ANativeWindow_release(window);
        window = nullptr;
        pthread_mutex_unlock(&mutex);
        return;
    }

    auto dstData = static_cast<uint8_t *>(windowBuffer.bits);
    int dstLineSize = windowBuffer.stride * 4;
    for (int i = 0; i < windowBuffer.height; ++i) {
        memcpy(dstData + i * dstLineSize, data + i * lineSize, static_cast<size_t>(dstLineSize));
    }
    ANativeWindow_unlockAndPost(window);
    pthread_mutex_unlock(&mutex);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_player_MyPlayer_nativePrepare(JNIEnv *env, jobject thiz, jstring data_source) {
    const char *dataSource = env->GetStringUTFChars(data_source, JNI_FALSE);
    auto callHelper = new JavaCallHelper(_vm, env, thiz);
    myFFmpeg = new MyFFmpeg(callHelper, dataSource);
    myFFmpeg->setRenderFrameCallback(render);
    myFFmpeg->prepare();
    env->ReleaseStringUTFChars(data_source, dataSource);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_player_MyPlayer_nativeStart(JNIEnv *env, jobject thiz) {
    myFFmpeg->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_player_MyPlayer_nativeSetSurface(JNIEnv *env, jobject thiz, jobject surface) {
    pthread_mutex_lock(&mutex);
    if (window) {
        //之前已经设置过windows
        ANativeWindow_release(window);
        window = nullptr;
    }
    window = ANativeWindow_fromSurface(env, surface);
    pthread_mutex_unlock(&mutex);
}