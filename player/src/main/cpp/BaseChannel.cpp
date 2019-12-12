//
// Created by 熊峰 on 2019-12-12.
//

#include "BaseChannel.h"

BaseChannel::BaseChannel(int id) {
    this->id = id;
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
