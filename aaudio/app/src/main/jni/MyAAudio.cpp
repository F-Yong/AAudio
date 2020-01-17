//
// Created by ZhouEryong on 2020-01-12.
//

#include "MyAAudio.h"
#include <android/log.h>

#define TAG "MyAAudio"
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)


char g_Buffer[1024 * 1024] = {0};
int g_dataSize = 0;
int g_readIndex = 0;

aaudio_data_callback_result_t myAAudioStreamDataCallback(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames) {

    memcpy(g_Buffer + g_dataSize, audioData, 2 * numFrames * sizeof(short));
    g_dataSize += 2 * numFrames * sizeof(short);
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

aaudio_data_callback_result_t myAAudioStreamDataCallbackPlay(
        AAudioStream *stream,
        void *userData,
        void *audioData,
        int32_t numFrames) {

    LOGI("myAAudioStreamDataCallbackPlay numFrames:%d, g_dataSize:%d", numFrames, g_dataSize);
    const int readSize = numFrames * 2 * sizeof(short);
    if (g_readIndex + readSize < g_dataSize) {
        memcpy(audioData, g_Buffer + g_readIndex, (size_t)readSize);
        g_readIndex += readSize;
        return AAUDIO_CALLBACK_RESULT_CONTINUE;
    } else
        return AAUDIO_CALLBACK_RESULT_STOP;
}

//aaudio_data_callback_result_t MyAAudio::onAudioReady(
//        AAudioStream *audioStream,
//        void *audioData,
//        int numFrames) {
//    float *floatData = (float *)audioData;
//    // Read stored data into the g_Buffer provided.
//    int32_t framesRead = AAudioStream_read(stream, g_Buffer, g_dataSize/2/sizeof(short), timeout);
//    if(framesRead <= 0)
//    {
//        LOGE("AAudioStream_read framesRead: %d",framesRead);
//    }
//    // LOGI("%s() framesRead = %d, numFrames = %d", __func__, framesRead, numFrames);
//    return framesRead > 0
//           ? AAUDIO_CALLBACK_RESULT_CONTINUE
//           : AAUDIO_CALLBACK_RESULT_STOP;
//}



MyAAudio::MyAAudio() {

}


int MyAAudio::Set(int AAUDIO_PERFORMANCE_MODE = AAUDIO_PERFORMANCE_MODE_LOW_LATENCY,
        int AAUDIO_SHARING_MODE = AAUDIO_SHARING_MODE_SHARED,
        int AAUDIO_FORMAT = AAUDIO_FORMAT_PCM_I16,
        int Rate = 48000,
        int ChannelCount = 2,
        int BufferCapacityInFrames = 4096){
    result = AAudio_createStreamBuilder(&builder);
    if (result != AAUDIO_OK) {
        LOGE("AAudio_createStreamBuilder error %d", result);
        return 0;
    }
    // int32_t DeviceId = AAudioStream_getDeviceId(stream);
    // AAudioStreamBuilder_setDeviceId(builder, 18);
    AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE);
    AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE);
    AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT);
    AAudioStreamBuilder_setSampleRate(builder, Rate);
    AAudioStreamBuilder_setChannelCount(builder, ChannelCount);
    AAudioStreamBuilder_setBufferCapacityInFrames(builder, BufferCapacityInFrames);

    return 1;
}

int MyAAudio::record(){
    if(IsRecord || IsPlay)
    {
        if(IsRecord)
            LOGI("Currently recording");
        else
            LOGI("Currently playing");

        return 0;
    }

    memset(g_Buffer, 0, sizeof(g_Buffer));
    g_dataSize = 0;
    g_readIndex = 0;

    AAudioStreamBuilder_setDataCallback(builder, myAAudioStreamDataCallback, this);
    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_INPUT);

    result = AAudioStreamBuilder_openStream(builder, &stream);
    if (result != AAUDIO_OK) {
        LOGE("AAudioStreamBuilder_openStream error %d", result);
        return 0;
    }
    // int32_t sizeInFrames = AAudioStream_getBufferSizeInFrames(stream);
    // LOGI("sizeInFrames=%d", sizeInFrames);
    result = AAudioStream_requestStart(stream); // 状态请求
    if (result != AAUDIO_OK) {
        LOGE("AAudioStream_requestStart error %d", result);
        return 0;
    }

    IsRecord = true;

    return 1;
}

int MyAAudio::stopRecord(char* DataBuffer,int DataSize)
{
    if(!IsRecord)
    {
        LOGI("No recording");
        return 0;
    }

    result = AAudioStream_requestStop(stream);
    AAudioStream_close(stream);
    closeStream();
    IsRecord = false;

    memcpy(DataBuffer, g_Buffer, sizeof(g_Buffer));
    DataSize = g_dataSize;

    memset(g_Buffer, 0, sizeof(g_Buffer));
    g_dataSize = 0;
    g_readIndex = 0;

    return 1;
}

int MyAAudio::play(char* DataBuffer,int DataSize) {
    if(IsRecord || IsPlay)
    {
        if(IsRecord)
            LOGI("Currently recording");
        else
            LOGI("Currently playing");

        return 0;
    }

    memcpy(g_Buffer, DataBuffer, sizeof(DataBuffer));
    g_dataSize = 0;
    g_readIndex = 0;

    AAudioStreamBuilder_setDataCallback(builder, myAAudioStreamDataCallbackPlay, this);
    AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT); // 设置音频流方向

    result = AAudioStreamBuilder_openStream(builder, &stream);
    if (result != AAUDIO_OK) {
        LOGE("AAudioStreamBuilder_openStream error %d", result);
        return 0;
    }
//    int32_t sizeInFrames = AAudioStream_getBufferSizeInFrames(stream);
//    LOGI("sizeInFrames=%d", sizeInFrames);
    result = AAudioStream_requestStart(stream); // 状态请求
    if (result != AAUDIO_OK) {
        LOGE("AAudioStream_requestStart error %d", result);
        return 0;
    }

    IsPlay = true;
    return 1;
}

int MyAAudio::stopPlay()
{
    if(!IsPlay)
    {
        LOGI("No playing");
        return 0;
    }

    result = AAudioStream_requestStop(stream);
    AAudioStream_close(stream);
    closeStream();

    memcpy(g_Buffer, 0, sizeof(g_Buffer));
    g_dataSize = 0;
    g_readIndex = 0;

    IsPlay = false;
    return 1;
}


void MyAAudio::closeStream()
{
    AAudioStreamBuilder_delete(builder);
}
