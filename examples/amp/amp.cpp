#include "amp.hpp"

#define AMP_AUDIO_PORT_TYPE GROG_AUDIO_PORT_TYPE_STEREO

AmpPlugin::AmpPlugin() {
    audioInPort = CreateAudioPort("Audio In", "audio_in", Grog::PortType::InputPort, AMP_AUDIO_PORT_TYPE);
    audioOutPort = CreateAudioPort("Audio Out", "audio_out", Grog::PortType::OutputPort, AMP_AUDIO_PORT_TYPE);
    amplitudePort = CreateControlPort("Amplitude", "amp", 1.0f, 0.0f, 2.0f);
    amplitudePort->SetComment("Amplitude of the output signal");
}

void AmpPlugin::Begin() {

}

void AmpPlugin::Process(uint32_t sampleCount) {
    float amplitude = amplitudePort->GetValue<float>();

    for (uint32_t i = 0; i < AMP_AUDIO_PORT_TYPE; ++i) {
        float* audioIn = audioInPort->GetChannel<float>(i);
        float* audioOut = audioOutPort->GetChannel<float>(i);
        for (uint32_t j = 0; j < sampleCount; ++j) {
            audioOut[j] = audioIn[j] * amplitude;
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