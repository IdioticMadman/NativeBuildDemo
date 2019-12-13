//
// Created by 熊峰 on 2019-12-11.
//

#include "VideoChannel.h"

VideoChannel::VideoChannel(int id, AVCodecContext *avCodecContext)
        : BaseChannel(id, avCodecContext) {
}

void *decoderTask(void *args) {
    auto videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->decode();
    return nullptr;
}

void *renderTask(void *args) {
    auto videoChannel = static_cast<VideoChannel *>(args);
    videoChannel->render();
    return nullptr;
}

void VideoChannel::play() {
    isPlaying = 1;
    //1. 解码
    pthread_create(&decodeId, nullptr, decoderTask, this);
    //2. 播放
    pthread_create(&renderId, nullptr, renderTask, this);
}

void VideoChannel::decode() {

}

void VideoChannel::render() {

}
