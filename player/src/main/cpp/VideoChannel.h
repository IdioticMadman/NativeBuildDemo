//
// Created by 熊峰 on 2019-12-11.
//

#ifndef NATIVEBUILDDEMO_VIDEOCHANNEL_H
#define NATIVEBUILDDEMO_VIDEOCHANNEL_H

#include <pthread.h>
#include "BaseChannel.h"

extern "C" {
#include "libswscale/swscale.h"
#include <libavutil/imgutils.h>
};

class VideoChannel : public BaseChannel {
public:

    typedef void (*RenderFrameCallback)(uint8_t *, int, int, int);

    VideoChannel(int id, AVCodecContext *avCodecContext);

    ~VideoChannel();

    /**
     * 解码 + 播放
     */
    void play();

    /**
     * 解码
     */
    void decode();

    /**
     * 渲染，即播放
     */
    void render();

    void setReaderFrameCallback(RenderFrameCallback callback);

private:
    SafeQueue<AVFrame *> *frameQueue;
    pthread_t decodeId = 0;
    pthread_t renderId = 0;
    int isPlaying = 0;
    SwsContext *swsContext = nullptr;
    RenderFrameCallback renderFrameCallback = nullptr;
};

#endif //NATIVEBUILDDEMO_VIDEOCHANNEL_H
