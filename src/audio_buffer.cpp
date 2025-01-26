#include <grog_audio_plugin_client/audio_buffer.hpp>


uint32_t GetChannelCountWithType(Grog::AudioBufferType type) {
    switch (type) {
        case Grog::AudioBufferType::Mono:
            return 1;
        case Grog::AudioBufferType::Stereo:
            return 2;
        default:
            return 1;
    }
}

Grog::AudioBuffer::AudioBuffer(AudioBufferType type) : type{ type } {
    channelCount = GetChannelCountWithType(type);
}

Grog::AudioBuffer::~AudioBuffer() {
}

float* Grog::AudioBuffer::GetChannel(uint32_t idx) {
    return channels[idx];
}

uint32_t Grog::AudioBuffer::GetChannelCount() const {
    return channelCount;
}

Grog::AudioBufferType Grog::AudioBuffer::GetType() const {
    return type;
}

void Grog::AudioBuffer::SetChannelsPtr(float** ptr) {
    channels = ptr;
}