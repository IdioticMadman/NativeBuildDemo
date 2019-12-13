//
// Created by 熊峰 on 2019-12-11.
//


#include "VideoChannel.h"

VideoChannel::VideoChannel(int id, AVCodecContext *avCodecContext)
        : BaseChannel(id, avCodecContext) {
    this->frameQueue = new SafeQueue<AVFrame *>;
}


VideoChannel::~VideoChannel() {
    this->frameQueue->setReleaseCallback(BaseChannel::releaseAvFrame);
    this->frameQueue->clear();
    DELETE(frameQueue);
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
    frameQueue->setWork(1);
    LOGE("开始播放");
    //1. 解码
    pthread_create(&decodeId, nullptr, decoderTask, this);
    //2. 播放
    pthread_create(&renderId, nullptr, renderTask, this);
}

void VideoChannel::decode() {
    LOGE("开始解码");
    AVPacket *avPacket = nullptr;
    int ret = 0;
    while (isPlaying) {
        ret = packetQueue->dequeue(avPacket);
        if (!isPlaying) {
            //不在播放，跳出
            break;
        }
        if (!ret) {
            //取出 packet 失败，尝试再次获取
            continue;
        }
        //fixme 这个地方可能会出现喂饱了的情况没处理
        ret = avcodec_send_packet(avCodecContext, avPacket);
        //送给解码器，释放packet
        releaseAvPacket(&avPacket);
        if (ret != 0) {
            //解码失败，退出解码
            break;
        }
        AVFrame *avFrame = av_frame_alloc();
        ret = avcodec_receive_frame(avCodecContext, avFrame);\
        if (ret == AVERROR(EAGAIN)) {
            //暂时没有 avFrame 输出，需要继续输入 packet
            continue;
        } else if (ret != 0) {
            //读取 frame 失败
            break;
        }
        //再开一个线程来渲染
        frameQueue->enqueue(avFrame);
    }
    releaseAvPacket(&avPacket);
}

void VideoChannel::render() {
    LOGE("开始渲染");
    //获取缩放上下文
    swsContext = sws_getContext(avCodecContext->width, avCodecContext->height,
                                avCodecContext->pix_fmt,
                                avCodecContext->width, avCodecContext->height, AV_PIX_FMT_RGBA,
                                SWS_BILINEAR, nullptr, nullptr, nullptr);

    AVFrame *avFrame = nullptr;
    uint8_t *dstData[4];
    int dstLineSize[4];
    int ret = 0;
    av_image_alloc(dstData, dstLineSize, avCodecContext->width, avCodecContext->height,
                   AV_PIX_FMT_RGBA, 1);
    while (isPlaying) {
        ret = frameQueue->dequeue(avFrame);
        if (!isPlaying) {
            break;
        }
        if (!ret) {
            continue;
        }
        sws_scale(swsContext, avFrame->data, avFrame->linesize, 0,
                  avCodecContext->height, dstData, dstLineSize);

        if (renderFrameCallback) {
            renderFrameCallback(dstData[0], dstLineSize[0], avCodecContext->width,
                                avCodecContext->height);
        } else {
            LOGE("no renderFrameCallback, please set it");
        }
        releaseAvFrame(&avFrame);
    }
    av_freep(&dstData[0]);
    releaseAvFrame(&avFrame);
}

void VideoChannel::setReaderFrameCallback(VideoChannel::RenderFrameCallback callback) {
    this->renderFrameCallback = callback;
}

