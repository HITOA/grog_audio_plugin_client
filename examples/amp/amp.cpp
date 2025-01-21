#include "amp.hpp"

void AmpPlugin::InitializePort(Grog::PortManager& manager) {
    inputAudioPort = manager.CreatePort(
        "Audio Input", "audio_in", Grog::PortType::AudioPort, Grog::PortDirection::InputPort);
    outputAudioPort = manager.CreatePort(
        "Audio Output", "audio_out", Grog::PortType::AudioPort, Grog::PortDirection::OutputPort);
}

void AmpPlugin::Begin() {

}

void AmpPlugin::Process() {
    
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
    return "Amp Example";
}

Grog::AudioPluginType AmpPlugin::GetType() const {
    return Grog::AudioPluginType::GeneratorPlugin;
}

Grog::AudioPlugin* Grog::InstantiatePlugin() {
    return new AmpPlugin{};
}