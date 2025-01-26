#pragma once

#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>

class SynthPlugin : public Grog::AudioPlugin {
public:
    SynthPlugin();

    void Begin() override;
    void Process(uint32_t sampleCount) override;
    void End() override;

    Grog::AudioPluginGUI* InstantiateGUI() override;
    bool HasGUI() const override;

    const char* GetName() const override;

private:
    Grog::AudioBuffer audioOut = { Grog::AudioBufferType::Stereo };

    Grog::MidiBuffer midiIn = {};

    Grog::ControlPort level = { "Level", 0.1f, 0.0f, 1.0f };
    Grog::ControlPort attack = { "Attack", 0.1f, 0.001f, 5.0f };
    Grog::ControlPort decay = { "Decay", 0.1f, 0.001f, 5.0f };
    Grog::ControlPort sustain = { "Sustain", 0.5f, 0.0f, 1.0f };
    Grog::ControlPort release = { "Release", 0.1f, 0.001f, 5.0f };

    double position = 0.0;
};