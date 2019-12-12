//
// Created by 熊峰 on 2019-12-11.
//

#ifndef NATIVEBUILDDEMO_VIDEOCHANNEL_H
#define NATIVEBUILDDEMO_VIDEOCHANNEL_H

#include "BaseChannel.h"

class VideoChannel : public BaseChannel {
public:
    VideoChannel(int id);

    /**
     * 解码 + 播放
     */
    void play();
};

#endif //NATIVEBUILDDEMO_VIDEOCHANNEL_H
