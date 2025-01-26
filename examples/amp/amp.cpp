#include "amp.hpp"

AmpPlugin::AmpPlugin() {
    SetAudioInputBuffer(&audioIn);
    SetAudioOutputBuffer(&audioOut);
    AddControlPort(&amplitude);
}

void AmpPlugin::Begin() {

}

void AmpPlugin::Process(uint32_t sampleCount) {
    float amplitudeValue = amplitude.GetValue();
    for (uint32_t i = 0; i < 2; ++i) {
        float* audioInPtr = audioIn.GetChannel(i);
        float* audioOutPtr = audioOut.GetChannel(i);
        for (uint32_t j = 0; j < sampleCount; ++j) {
            audioOutPtr[j] = audioInPtr[j] * amplitudeValue;
        }
    }
}

void AmpPlugin::End() {

}

Grog::AudioPluginGUI* AmpPlugin::InstantiateGUI() {
    return nullptr;
}

bool AmpPlugin::HasGUI() const {
    return false;
}

const char* AmpPlugin::GetName() const {
    return "Grog Amp";
}

Grog::AudioPlugin* Grog::InstantiatePlugin() {
    return new AmpPlugin{};
}