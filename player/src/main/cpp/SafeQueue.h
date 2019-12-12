//
// Created by 熊峰 on 2019-12-10.
//

#ifndef NATIVEBUILDDEMO_SAFEQUEUE_H
#define NATIVEBUILDDEMO_SAFEQUEUE_H

#include <pthread.h>
#include <queue>
#include "macro.h"

template<typename T>
class SafeQueue {
    /**
     * 函数指针，释放时被回调
     */
    typedef void (*ReleaseCallback)(T *t);

    typedef void (*SyncHandler)(std::queue<T> *queue);

public:
    SafeQueue(){
        pthread_mutex_init(&pMutex, nullptr);
        pthread_cond_init(&pCond, nullptr);
    }

    ~SafeQueue(){
        pthread_mutex_destroy(&pMutex);
        pthread_cond_destroy(&pCond);
    }

    /**
     * 出队
     * @param t
     * @return
     */
    T dequeue(T &t){
        int ret = 0;
        pthread_mutex_lock(&pMutex);
        while (work && queue->empty()) {
            //如果没有数据，则等待新数据到达
            pthread_cond_wait(&pCond, &pMutex);
        }
        //队列不为空取出数据
        if (!queue->empty()) {
            t = queue->front();
            queue->pop();
            ret = 1;
        }
        pthread_mutex_unlock(&pMutex);
        return ret;
    }

    /**
     * 入队
     * @param t
     */
    void enqueue(T t){
        pthread_mutex_lock(&pMutex);
        if (work) {
            queue->push(t);
            pthread_cond_signal(&pCond);
        }
        pthread_mutex_unlock(&pMutex);
    }

    /**
     * 设置释放时，item 释放的回调
     * @param releaseCallback
     */
    void setReleaseCallback(ReleaseCallback releaseCallback){
        this->releaseCallback = releaseCallback;
    }

    void setSyncHandler(SyncHandler syncHandler){
        this->syncHandler = syncHandler;
    }

    /**
     * 提供一个线程安全的方法操作内部 queue
     */
    void sync(){
        pthread_mutex_lock(&pMutex);
        //主动丢包
        if (syncHandler) {
            syncHandler(queue);
        }
        pthread_mutex_unlock(&pMutex);
    }

    /**
        * 清除队列
        */
    void clear(){
        pthread_mutex_lock(&pMutex);
        uint32_t size = queue->size();
        for (int i = 0; i < size; ++i) {
            T t = queue->front();
            //释放 t
            if (!t && !releaseCallback) {
                this->releaseCallback(&t);
            }
            queue->pop();
        }
        pthread_mutex_unlock(&pMutex);
    }

    /**
     * 更新 safeQueue 的状态
     * @param status
     */
    void setWork(int status){
        pthread_mutex_lock(&pMutex);
        this->work = status;
        //通知当前状态发生变化
        pthread_cond_signal(&pCond);
        pthread_mutex_unlock(&pMutex);
    }

private:
    int work = 0;
    pthread_cond_t pCond{};
    pthread_mutex_t pMutex{};
    std::queue<T> *queue;
    ReleaseCallback releaseCallback = nullptr;
    SyncHandler syncHandler = nullptr;
};


#endif //NATIVEBUILDDEMO_SAFEQUEUE_H
