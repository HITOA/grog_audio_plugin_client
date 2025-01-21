#pragma once

#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>

class AmpPlugin : public Grog::AudioPlugin {
public:
    void InitializePort(Grog::PortManager& manager) override;
    void Begin() override;
    void Process() override;
    void End() override;

    Grog::AudioPluginGUI* InstantiateGUI() override;
    bool HasGUI() const override;

    const char* GetName() const override;
    Grog::AudioPluginType GetType() const override;

private:
    Grog::Port* inputAudioPort = nullptr;
    Grog::Port* outputAudioPort = nullptr;
};