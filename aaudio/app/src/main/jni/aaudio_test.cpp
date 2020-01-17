//
// Created by ZhouEryong on 2020-01-08.
//

#include "com_example_aaudio_MainActivity.h"
#include "MyAAudio.h"

static MyAAudio m_aaudio;

// record
int JNICALL Java_com_example_aaudio_MainActivity_MyRecord
        (JNIEnv *, jobject,jint device_ID){

    int result = m_aaudio.record();
    if(result)
    {
        // 正常录音
        return 101;
    }
    else{
        // 错误开启录音
        return 100;
    }
}

// stop  java_com_example_aaudio_MainActivity
int JNICALL Java_com_example_aaudio_MainActivity_MyStopRecord
        (JNIEnv *, jobject) {

    // 接受录音数据
    char DataBuffer[1024 * 1024] = {0};
    int DataSize = 0;

    int result = m_aaudio.stopRecord(DataBuffer,DataSize);
    if(result)
    {
        // 正常结束录音

        return 201;
    }
    else{
        // 错误结束录音
        return 200;
    }
}

// play
int JNICALL Java_com_example_aaudio_MainActivity_MyPlay
        (JNIEnv *, jobject) {
    // 播放音频数据：下面变量应该是音频数据的
    char* DataBuffer = {0};
    int DataSize = 0;

    int result = m_aaudio.play(DataBuffer,DataSize);
    if(result)
    {
        // 正常播放录音

        return 301;
    }
    else
    {
        // 异常播放录音
        return 300;
    }
}

int JNICALL Java_com_example_aaudio_MainActivity_MyStopPlay
        (JNIEnv *, jobject) {

    int result = m_aaudio.stopPlay();
    if(result)
    {
        // 正常播放录音

        return 401;
    }
    else
    {
        // 异常播放录音
        return 400;
    }
}
