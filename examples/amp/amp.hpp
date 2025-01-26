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
    Grog::AudioBuffer audioIn = { Grog::AudioBufferType::Stereo };
    Grog::AudioBuffer audioOut = { Grog::AudioBufferType::Stereo };
    Grog::ControlPort amplitude = { "Amplitude", 1.0f, 0.0f, 2.0f };
};