#include "portaudioWrapper.h"

PortaudioWrapper::PortaudioWrapper()
{
    std::cout << "PortaudioWrapper Constructor.\n";
    auto err = Pa_Initialize();
    if(err != paNoError)
    {
        std::cerr << "ERROR: PortaudioWrapper Constructor: Pa_Initialize returned 0x" << err << std::endl;
    }
}

PortaudioWrapper::~PortaudioWrapper()
{
    std::cout << "PortaudioWrapper Destructor.\n";
    auto err = Pa_Terminate();
    if(err != paNoError)
    {
        std::cerr << "ERROR: PortaudioWrapper Destructor: Pa_Terminate returned 0x" << err << std::endl;
    }
}

int PortaudioWrapper::GetDeviceCount()
{
    return static_cast<int>(Pa_GetDeviceCount());
}

Device PortaudioWrapper::GetDeviceInfo(int deviceId)
{
    Device result;
    auto deviceInfo = Pa_GetDeviceInfo(deviceId);

    result.name = deviceInfo->name;

    result.inChannels = deviceInfo->maxInputChannels;
    result.outChannels = deviceInfo->maxOutputChannels;

    result.inLowLatency = deviceInfo->defaultLowInputLatency;
    result.inHighLatency = deviceInfo->defaultHighInputLatency;

    result.outLowLatency = deviceInfo->defaultLowOutputLatency;
    result.outHighLatency = deviceInfo->defaultHighOutputLatency;

    return result;
}

std::vector<int> PortaudioWrapper::GetSampleRates(int inputDeviceId, int outputDeviceId)
{
    PaStreamParameters inputParameters{}, outputParameters{};

    inputParameters.device = inputDeviceId;
    inputParameters.channelCount = Pa_GetDeviceInfo(inputDeviceId)->maxInputChannels;
    inputParameters.sampleFormat = SAMPLE_FORMAT;

    outputParameters.device = outputDeviceId;
    outputParameters.channelCount = Pa_GetDeviceInfo(outputDeviceId)->maxOutputChannels;
    outputParameters.sampleFormat = SAMPLE_FORMAT;

    static std::vector<int> standardSampleRates = {
        8000, 9600, 11025, 12000, 16000, 22050, 24000, 32000,
        44100, 48000, 88200, 96000, 192000
    };

    std::vector<int> result;
    for(const auto& sampleRate : standardSampleRates)
    {
        auto err = Pa_IsFormatSupported(&inputParameters, &outputParameters, sampleRate);
        if(err == paFormatIsSupported)
        {
            result.push_back(sampleRate);
        }
    }

    return result;
}

void PortaudioWrapper::StartStream(int inputDeviceId, int outputDeviceId, int sampleRate, void* obj)
{
    PaStreamParameters inputParameters{}, outputParameters{};

    inputParameters.device = inputDeviceId;
    inputParameters.channelCount = 1;
    inputParameters.sampleFormat = SAMPLE_FORMAT;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;

    outputParameters.device = outputDeviceId;
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = SAMPLE_FORMAT;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;

#ifdef __APPLE__
    PaMacCoreStreamInfo macInfo;
    PaMacCore_SetupStreamInfo(&macInfo, paMacCorePro);
    inputParameters.hostApiSpecificStreamInfo = &macInfo;
    outputParameters.hostApiSpecificStreamInfo = &macInfo;
#endif

    Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,
              sampleRate,
              FRAMES_PER_BUFFER,
              0,
              audio_callback,
              obj);
    Pa_StartStream(stream);
    
    const PaStreamInfo* info = Pa_GetStreamInfo(stream);
    printf("Input latency: %fms\n", info->inputLatency*1000);
    printf("Output latency: %fms\n", info->outputLatency*1000);
}

void PortaudioWrapper::StopStream()
{
    Pa_CloseStream(stream);
}

int PortaudioWrapper::audio_callback(
        const void *inputBuffer,
        void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData
    )
{
    SAMPLE *out = (SAMPLE*)outputBuffer;
    const SAMPLE *in = (const SAMPLE*)inputBuffer;
    unsigned int i;
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if(inputBuffer == NULL)
    {
        for(i=0; i<framesPerBuffer; i++)
        {
            *out++ = 0;
            *out++ = 0;
        }
    }
    else
    {
        for(i=0; i<framesPerBuffer; i++)
        {
            StereoSample sample;

            sample.left = *in++;
            sample.right = sample.left;

            auto result = static_cast<IAudioCallbackWrapper*>(userData)->AudioEffectHandler(sample);

            *out++ = result.left;
            *out++ = result.right;
        }
    }

    return paContinue;
}

