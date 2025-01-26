#pragma once

#include <grog_audio_plugin_client/allocator.hpp>

#include <cstdint>


namespace Grog {

    enum class AudioBufferType {
        None,
        Mono,
        Stereo
    };

    class AudioBuffer {
    public:
        AudioBuffer() = delete;
        AudioBuffer(AudioBufferType type);
        ~AudioBuffer();
        
        float* GetChannel(uint32_t idx);
        uint32_t GetChannelCount() const;
        AudioBufferType GetType() const;

        void SetChannelsPtr(float** ptr);

    private:
        float** channels = nullptr;
        uint32_t channelCount = 0;
        AudioBufferType type = AudioBufferType::None;
    };

}