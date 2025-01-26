#include "synth.hpp"

#include <cmath>

#define SYNTH_AUDIO_PORT_TYPE GROG_AUDIO_PORT_TYPE_STEREO

SynthPlugin::SynthPlugin() {
    SetAudioOutputBuffer(&audioOut);
    SetMidiInputBuffer(&midiIn);
    AddControlPort(&level);
    AddControlPort(&attack);
    AddControlPort(&decay);
    AddControlPort(&sustain);
    AddControlPort(&release);
}

void SynthPlugin::Begin() {

}

void SynthPlugin::Process(uint32_t sampleCount) {

    for (const Grog::MidiEvent& event : midiIn) {
        switch (event.message.GetMessageType()) {
            case Grog::MidiMessageType::NoteOn:
                break;
            case Grog::MidiMessageType::NoteOff:
                break;
            default:
                break;
        }
    }

    float freq = 440.0f;
    float levelValue = level.GetValue();

    for (uint32_t i = 0; i < 2; ++i) {
        float* audioOutPtr = audioOut.GetChannel(i);
        double p = position;
        for (uint32_t j = 0; j < sampleCount; ++j) {
            audioOutPtr[j] = sin(2.0 * M_PI * p) * levelValue;
            p += freq / GetSampleRate();
        }
    }

    position += freq / GetSampleRate() * sampleCount;
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

void Grog::DestroyPlugin(AudioPlugin* pluginInstance) {
    delete (SynthPlugin*)pluginInstance;
}