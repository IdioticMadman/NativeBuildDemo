#include <jni.h>
#include "test.h"

 extern "C"
 JNIEXPORT jstring JNICALL
 Java_com_example_nativebuilddemo_MainActivity_hello(JNIEnv *env, jobject thiz) {
     //调用第三方so库
     test();
     return env->NewStringUTF("Hello from native");
 }