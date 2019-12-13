//
// Created by 熊峰 on 2019-12-11.
//

#ifndef NATIVEBUILDDEMO_MYFFMPEG_H
#define NATIVEBUILDDEMO_MYFFMPEG_H

#include <pthread.h>
#include <cstring>
#include "JavaCallHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"
#include "BaseChannel.h"

extern "C" {
#include "libavformat/avformat.h"
}

class MyFFmpeg {
public:
    MyFFmpeg(JavaCallHelper *callHelper, const char *dataSource);

    ~MyFFmpeg();

    void prepare();

    void _prepare();

    void start();

    void _start();

    void setRenderFrameCallback(VideoChannel::RenderFrameCallback renderFrameCallback);

private:
    JavaCallHelper *callHelper;;
    char *dataSource;
    pthread_t preparePid = 0;
    pthread_t playPid = 0;
    AVFormatContext *avFormatContext = nullptr;
    AudioChannel *audioChannel = nullptr;
    VideoChannel *videoChannel = nullptr;
    int isPlaying = 0;
    //视频数据回调
    VideoChannel::RenderFrameCallback renderFrameCallback = nullptr;
};

#endif //NATIVEBUILDDEMO_MYFFMPEG_H
