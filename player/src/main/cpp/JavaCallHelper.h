//
// Created by 熊峰 on 2019-12-11.
//

#ifndef NATIVEBUILDDEMO_JAVACALLHELPER_H
#define NATIVEBUILDDEMO_JAVACALLHELPER_H

#include <jni.h>
#include "macro.h"

class JavaCallHelper {
public:
    JavaCallHelper(JavaVM *javaVm, JNIEnv *env, jobject instance);

    ~JavaCallHelper();

    void onError(int thread, int code);

    void onPrePare(int thread);

private:
    JavaVM *javaVm;
    JNIEnv *jniEnv;
    jobject instance;
    jmethodID onErrorId;
    jmethodID onPrePareId;
};

#endif //NATIVEBUILDDEMO_JAVACALLHELPER_H
