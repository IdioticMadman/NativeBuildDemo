//
// Created by 熊峰 on 2019-12-11.
//

#include "MyFFmpeg.h"

//pthread 异步线程准备媒体信息获取
void *taskPrepare(void *args) {
    auto myFFmpeg = static_cast<MyFFmpeg *>(args);
    myFFmpeg->_prepare();
    return nullptr;
}

void *taskPlay(void *args) {
    auto myFFmpeg = static_cast<MyFFmpeg *>(args);
    myFFmpeg->_start();
    return nullptr;
}

MyFFmpeg::MyFFmpeg(JavaCallHelper *helper, const char *dataSource) {
    this->callHelper = helper;
    this->dataSource = new char[strlen(dataSource) + 1];
    strcpy(this->dataSource, dataSource);
}

MyFFmpeg::~MyFFmpeg() {
    DELETE(dataSource);
    DELETE(callHelper);
}

void MyFFmpeg::prepare() {
    pthread_create(&preparePid, nullptr, taskPrepare, this);
}

void MyFFmpeg::_prepare() {
    //初始化网络，让ffmepg能够使用网络
    avformat_network_init();
    int ret = 0;
    //1. 打开媒体文件地址，获取当前视频的信息
    ret = avformat_open_input(&avFormatContext, dataSource, nullptr, nullptr);
    if (ret < 0) {
        //打开媒体失败
        LOGE("打开媒体失败: %s", av_err2str(ret));
        callHelper->onError(THREAD_CHILD, FFMPEG_CAN_NOT_OPEN_URL);
        return;
    }
    //2. 查找媒体中的音视频流
    ret = avformat_find_stream_info(avFormatContext, nullptr);
    if (ret < 0) {
        LOGE("查找流信息失败: %s", av_err2str(ret));
        callHelper->onError(THREAD_CHILD, FFMPEG_CAN_NOT_FIND_STREAM);
        return;
    }
    //几个流
    unsigned int streams = avFormatContext->nb_streams;
    //获取流
    for (int i = 0; i < streams; ++i) {
        AVStream *stream = avFormatContext->streams[i];
        //包含解码这段流的各种参数信息
        AVCodecParameters *codecPar = stream->codecpar;

        //无论是视频还是音频都需要的操作
        //3. 获得解码器
        AVCodec *decoder = avcodec_find_decoder(codecPar->codec_id);
        if (decoder == nullptr) {
            //找不到解码器
            LOGE("找不到解码器");
            callHelper->onError(THREAD_CHILD, FFMPEG_CAN_NOT_FIND_DECODER);
            return;
        }
        //4. 获取解码器上下文
        AVCodecContext *codecCtx = avcodec_alloc_context3(decoder);
        if (codecCtx == nullptr) {
            LOGE("初始化解码器上下文失败");
            callHelper->onError(THREAD_CHILD, FFMPEG_ALLOC_CODEC_CONTEXT_FAIL);
            return;
        }
        //5. 设置解码器上下文的参数
        ret = avcodec_parameters_to_context(codecCtx, codecPar);
        if (ret < 0) {
            LOGE("给解码器上下文设置参数失败: %s", av_err2str(ret));
            callHelper->onError(THREAD_CHILD, FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL);
            return;
        }
        //6. 打开编码器
        ret = avcodec_open2(codecCtx, decoder, nullptr);
        if (ret < 0) {
            LOGE("打开解码器失败: %s", av_err2str(ret));
            callHelper->onError(THREAD_CHILD, FFMPEG_OPEN_DECODER_FAIL);
            return;
        }

        AVMediaType mediaType = codecPar->codec_type;

        if (mediaType == AVMEDIA_TYPE_AUDIO) {
            audioChannel = new AudioChannel(i, codecCtx);
        } else if (mediaType == AVMEDIA_TYPE_VIDEO) {
            videoChannel = new VideoChannel(i, codecCtx);
        }
    }
    if (!audioChannel && !videoChannel) {
        LOGE("没有视频流");
        callHelper->onError(THREAD_CHILD, FFMPEG_NO_MEDIA);
        return;
    }
    //准备就绪，可以开始播放
    LOGE("准备就绪啦~~~");
    callHelper->onPrePare(THREAD_CHILD);
}

void MyFFmpeg::start() {
    isPlaying = 1;
    if (videoChannel) {
        videoChannel->packetQueue->setWork(1);
        //开启播放，decode + render
        videoChannel->play();
    }
    //开启读取 packet 线程
    pthread_create(&playPid, nullptr, taskPlay, this);
}

void MyFFmpeg::_start() {
    int ret;
    while (isPlaying) {
        AVPacket *packet = av_packet_alloc();
        ret = av_read_frame(avFormatContext, packet);
        if (ret == 0) {
            if (audioChannel && packet->stream_index == audioChannel->id) {
                audioChannel->packetQueue->enqueue(packet);
            } else if (videoChannel && packet->stream_index == videoChannel->id) {
                videoChannel->packetQueue->enqueue(packet);
            }
        } else if (AVERROR_EOF == ret) {
            LOGE("到文件末尾");
        } else {

        }
    }

}


