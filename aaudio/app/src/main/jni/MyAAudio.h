//
// Created by ZhouEryong on 2020-01-12.
//

#ifndef MyAAUDIO_AAUDIO_H
#define MyAAUDIO_AAUDIO_H


#include <aaudio/AAudio.h>
#include <iostream>
#include <list>
using namespace std;

class MyAAudio {

public:
    MyAAudio();

    int Set(int AAUDIO_PERFORMANCE_MODE,
            int AAUDIO_SHARING_MODE,
            int AAUDIO_FORMAT,
            int Rate,
            int ChannelCount,
            int BufferCapacityInFrames);
    int record();
    int stopRecord(char* DataBuffer,int DataSize);
    int play(char* DataBuffer,int DataSize);
    int stopPlay();

private:
    void closeStream();

    bool IsRecord = false;
    bool IsPlay = false;
    int timeout = 100;
    AAudioStream *stream;
    AAudioStreamBuilder *builder = NULL;
    aaudio_result_t result;

//    AAudioStream* mPlaybackStream = NULL;
//    AAudioStream* mRecordingStream = NULL;
};
#endif //MyAAUDIO_AAUDIO_H
