#include "synth.hpp"

#define SYNTH_AUDIO_PORT_TYPE GROG_AUDIO_PORT_TYPE_STEREO

SynthPlugin::SynthPlugin() {
    audioOutPort = CreateAudioPort("Audio Out", "audio_out", Grog::PortType::OutputPort, SYNTH_AUDIO_PORT_TYPE);
}

void SynthPlugin::Begin() {

}

void SynthPlugin::Process(uint32_t sampleCount) {
    
}

void SynthPlugin::End() {

}

Grog::AudioPluginGUI* SynthPlugin::InstantiateGUI() {
    return nullptr;
}

bool SynthPlugin::HasGUI() const {
    return false;
}

const char* SynthPlugin::GetName() const {
    return "Grog Synth";
}

Grog::AudioPlugin* Grog::InstantiatePlugin() {
    return new SynthPlugin{};
}