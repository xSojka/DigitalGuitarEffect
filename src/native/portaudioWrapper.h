#pragma once

#include "portaudio.h"

#ifdef __APPLE__
#include "pa_mac_core.h"
#endif

#include <iostream>
#include <vector>
#include <functional>

#include "device.h"
#include "IAudioCallbackWrapper.h"

#define SAMPLE_FORMAT paFloat32
#define FRAMES_PER_BUFFER   (64)
using SAMPLE = float;

class PortaudioWrapper {
public:
    struct Sample2Channels{
        float left;
        float right;
    };
    PortaudioWrapper();
    PortaudioWrapper(PortaudioWrapper &&) = delete;
    PortaudioWrapper(const PortaudioWrapper &) = delete;
    PortaudioWrapper &operator=(PortaudioWrapper &&) = delete;
    PortaudioWrapper &operator=(const PortaudioWrapper &) = delete;
    ~PortaudioWrapper();

    int GetDeviceCount();
    Device GetDeviceInfo(int deviceId);
    std::vector<int> GetSampleRates(int inputDevice, int outputDevice);

    void StartStream(int inputDeviceId, int outputDeviceId, int sampleRate, void* obj);
    void StopStream();
private:
    static int audio_callback(
        const void *inputBuffer,
        void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData);

    PaStream* stream = nullptr;
};
