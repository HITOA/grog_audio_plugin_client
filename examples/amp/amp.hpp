#pragma once

#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>

class AmpPlugin : public Grog::AudioPlugin {
public:
    AmpPlugin();

    void Begin() override;
    void Process(uint32_t sampleCount) override;
    void End() override;

    Grog::AudioPluginGUI* InstantiateGUI() override;
    bool HasGUI() const override;

    const char* GetName() const override;

private:
    Grog::Port* audioInPort = nullptr;
    Grog::Port* audioOutPort = nullptr;
    Grog::Port* amplitudePort = nullptr;
};