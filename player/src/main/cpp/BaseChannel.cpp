//
// Created by 熊峰 on 2019-12-12.
//

#include "BaseChannel.h"

BaseChannel::BaseChannel(int id, AVCodecContext *avCodecContext) {
    this->id = id;
    this->avCodecContext = avCodecContext;
    this->packetQueue = new SafeQueue<AVPacket *>;
}


BaseChannel::~BaseChannel() {
    packetQueue->setReleaseCallback(BaseChannel::releaseAvPacket);
    packetQueue->clear();
    DELETE(packetQueue);
}

void BaseChannel::releaseAvPacket(AVPacket **packet) {
    if (packet) {
        av_packet_free(packet);
        *packet = nullptr;
    }
}

void BaseChannel::releaseAvFrame(AVFrame **frame) {
    if (frame) {
        av_frame_free(frame);
        *frame = nullptr;
    }

}
