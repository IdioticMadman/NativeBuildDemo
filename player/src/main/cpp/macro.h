//
// Created by 熊峰 on 2019-12-11.
//

#ifndef NATIVEBUILDDEMO_MACRO_H
#define NATIVEBUILDDEMO_MACRO_H

#include <jni.h>
#include <android/log.h>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "MY_FFMPEG", __VA_ARGS__)

//宏函数 删除对象
#define DELETE(obj) if(obj) { delete obj; obj = nullptr; }

//标记线程
//主线程
#define MAIN_THREAD 1
//子线程
#define THREAD_CHILD 2

//-------------------错误代码----------------

//打不开视频
#define FFMPEG_CAN_NOT_OPEN_URL 1
//找不到流媒体
#define FFMPEG_CAN_NOT_FIND_STREAM 2
//找不到解码器
#define FFMPEG_CAN_NOT_FIND_DECODER 3
//无法根据解码器创建上下文
#define FFMPEG_ALLOC_CODEC_CONTEXT_FAIL 4
//根据流信息 配置上下文参数失败
#define FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL 6
//打开解码器失败
#define FFMPEG_OPEN_DECODER_FAIL 7
//没有音视频
#define FFMPEG_NO_MEDIA 8


#endif //NATIVEBUILDDEMO_MACRO_H
