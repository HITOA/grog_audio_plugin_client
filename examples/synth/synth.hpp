#pragma once

#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>

enum class KeyStatus {
    KeyOff,
    KeyPressed,
    KeyReleased
};

struct ADSR {
    float attack = 0.1f;
    float decay = 0.1f;
    float sustain = 0.5f;
    float release = 0.1f;
};

class Key {
public:
    Key() = default;
    
    void SetSampleRate(double rate);
    void Press(const uint8_t note, const uint8_t vel, const ADSR adsr);
    void Release(const uint8_t note, const uint8_t vel);
    void Off();
    float Adsr();
    float Get();
    void Proceed();

private:
    KeyStatus status = KeyStatus::KeyOff;
    uint8_t note = 0;
    uint8_t velocity = 0;
    ADSR adsr = {};
    double sampleRate = 44100.0;
    double position = 0.0;
    float startLevel = 0.1f;
    float frequency = 0.0f;
    double time = 0.0;
};

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
    void Play(uint32_t begin, uint32_t end);

private:
    Grog::AudioBuffer audioOut = { Grog::AudioBufferType::Stereo };

    Grog::MidiBuffer midiIn = {};

    Grog::ControlPort level = { "Level", 0.1f, 0.0f, 1.0f };
    Grog::ControlPort attack = { "Attack", 0.1f, 0.001f, 5.0f };
    Grog::ControlPort decay = { "Decay", 0.1f, 0.001f, 5.0f };
    Grog::ControlPort sustain = { "Sustain", 0.5f, 0.0f, 1.0f };
    Grog::ControlPort release = { "Release", 0.1f, 0.001f, 5.0f };

    double position = 0.0;

    Key key{};
};