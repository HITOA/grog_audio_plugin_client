#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>

#include <cstring>

Grog::AudioPlugin::~AudioPlugin() {
    if (controlPorts) {
        allocator.Deallocate(controlPorts);
        controlPorts = nullptr;
    }
}

Grog::Allocator* Grog::AudioPlugin::GetAllocator() {
    return &allocator;
}

void Grog::AudioPlugin::SetSampleRate(double sampleRate) {
    this->sampleRate = sampleRate;
}

double Grog::AudioPlugin::GetSampleRate() {
    return sampleRate;
}

void Grog::AudioPlugin::SetDataPtr(void* ptr) {
    data = ptr;
}

void* Grog::AudioPlugin::GetDataPtr() {
    return data;
}

Grog::AudioBuffer* Grog::AudioPlugin::GetAudioInputBuffer() {
    return inputAudioBuffer;
}

Grog::AudioBuffer* Grog::AudioPlugin::GetAudioOutputBuffer() {
    return outputAudioBuffer;
}

Grog::MidiBuffer* Grog::AudioPlugin::GetMidiInputBuffer() {
    return inputMidiBuffer;
}

Grog::ControlPort* Grog::AudioPlugin::GetControlPort(uint32_t idx) {
    if (controlPortCount <= idx)
        return nullptr;
    return controlPorts[idx];
}

uint32_t Grog::AudioPlugin::GetControlPortCount() const {
    return controlPortCount;
}

void Grog::AudioPlugin::SetAudioInputBuffer(AudioBuffer* buffer) {
    inputAudioBuffer = buffer;
}

void Grog::AudioPlugin::SetAudioOutputBuffer(AudioBuffer* buffer) {
    outputAudioBuffer = buffer;
}

void Grog::AudioPlugin::SetMidiInputBuffer(MidiBuffer* buffer) {
    inputMidiBuffer = buffer;
}

void Grog::AudioPlugin::AddControlPort(ControlPort* port) {
    ControlPort** newControlPorts = (ControlPort**)allocator.Allocate(sizeof(ControlPort*) * (controlPortCount + 1));
    memcpy(newControlPorts, controlPorts, sizeof(ControlPort*) * controlPortCount);
    newControlPorts[controlPortCount] = port;
    ++controlPortCount;
    allocator.Deallocate(controlPorts);
    controlPorts = newControlPorts;
}