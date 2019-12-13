//
// Created by 熊峰 on 2019-12-11.
//

#ifndef NATIVEBUILDDEMO_AUDIOCHANNEL_H
#define NATIVEBUILDDEMO_AUDIOCHANNEL_H

#include "BaseChannel.h"

class AudioChannel : public BaseChannel {
public:
    AudioChannel(int id, AVCodecContext *avCodecContext);

    void play();
};

#endif //NATIVEBUILDDEMO_AUDIOCHANNEL_H
