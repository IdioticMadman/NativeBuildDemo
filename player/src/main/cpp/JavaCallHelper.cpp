//
// Created by 熊峰 on 2019-12-11.
//

#include "JavaCallHelper.h"

JavaCallHelper::JavaCallHelper(JavaVM *javaVm, JNIEnv *env, jobject instance) {
    this->javaVm = javaVm;
    //主线程的env
    this->jniEnv = env;
    //涉及到jobject需要跨方法，跨线程的时候，需要改成全局引用
    this->instance = env->NewGlobalRef(instance);
    //获取
    jclass clz = env->GetObjectClass(instance);
    this->onErrorId = env->GetMethodID(clz, "onError", "(I)V");
    this->onPrePareId = env->GetMethodID(clz, "onPrePare", "()V");
}

JavaCallHelper::~JavaCallHelper() {
    this->jniEnv->DeleteGlobalRef(this->instance);
}

void JavaCallHelper::onError(int thread, int code) {
    if (thread == MAIN_THREAD) {
        this->jniEnv->CallVoidMethod(this->instance, this->onErrorId, code);
    } else {
        //子线程需要attachThread才能使用 env
        JNIEnv *env;
        javaVm->AttachCurrentThread(&env, nullptr);
        env->CallVoidMethod(instance, onErrorId, code);
        javaVm->DetachCurrentThread();
    }
}

void JavaCallHelper::onPrePare(int thread) {

    if (thread == MAIN_THREAD) {
        this->jniEnv->CallVoidMethod(this->instance, this->onPrePareId);
    } else {
        //子线程需要attachThread才能使用 env
        JNIEnv *env;
        javaVm->AttachCurrentThread(&env, nullptr);
        env->CallVoidMethod(instance, onPrePareId);
        javaVm->DetachCurrentThread();
    }
}
