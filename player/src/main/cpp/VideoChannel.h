//
// Created by 熊峰 on 2019-12-11.
//

#ifndef NATIVEBUILDDEMO_VIDEOCHANNEL_H
#define NATIVEBUILDDEMO_VIDEOCHANNEL_H

#include <pthread.h>
#include "BaseChannel.h"

class VideoChannel : public BaseChannel {
public:
    VideoChannel(int id, AVCodecContext *avCodecContext);

    /**
     * 解码 + 播放
     */
    void play();

    void decode();

    void render();

private:
    pthread_t decodeId = 0;
    pthread_t renderId = 0;
    int isPlaying = 0;
};

#endif //NATIVEBUILDDEMO_VIDEOCHANNEL_H
