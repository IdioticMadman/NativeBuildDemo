//
// Created by 熊峰 on 2019-12-12.
//

#ifndef NATIVEBUILDDEMO_BASECHANNEL_H
#define NATIVEBUILDDEMO_BASECHANNEL_H

#include <jni.h>

#include "SafeQueue.h"
#include "macro.h"

extern "C" {
#include <libavcodec/avcodec.h>
};


class BaseChannel {
public:
    BaseChannel(int id, AVCodecContext *avCodecContext);

    virtual ~BaseChannel();

    //释放 avPacket 的函数指针
    static void releaseAvPacket(AVPacket **packet);

    //释放 avFrame 的函数指针
    static void releaseAvFrame(AVFrame **frame);

    /**
     * stream id 代表是哪个流 和 avFormatContext 的 stream 的 index匹配
     */
    int id;

    virtual void play() = 0;

    SafeQueue<AVPacket *> *packetQueue = nullptr;
    //解码器的上下文
    AVCodecContext *avCodecContext;
};

#endif //NATIVEBUILDDEMO_BASECHANNEL_H
