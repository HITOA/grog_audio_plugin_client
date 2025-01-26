#include "synth.hpp"

#include <cmath>

#define SYNTH_AUDIO_PORT_TYPE GROG_AUDIO_PORT_TYPE_STEREO


void Key::SetSampleRate(double rate) {
    sampleRate = rate;
}

void Key::Press(const uint8_t note, const uint8_t vel, const ADSR adsr) {
    startLevel = Adsr();
    this->note = note;
    this->velocity = vel;
    this->adsr = adsr;
    status = KeyStatus::KeyPressed;
    frequency = (float)(pow(2.0, ((double)note - 69.0) / 12.0) * 440.0);
    time = 0.0;
}

void Key::Release(const uint8_t note, const uint8_t vel) {
    if (status != KeyStatus::KeyPressed || this->note != note)
        return;
    startLevel = Adsr();
    status = KeyStatus::KeyReleased;
    time = 0.0;
}

void Key::Off() {
    position = 0.0;
    status = KeyStatus::KeyOff;
}

float Key::Adsr() {
    switch (status) {
        case KeyStatus::KeyPressed:
            if (time < adsr.attack)
                return startLevel + (1.0f - startLevel) * time / adsr.attack;
            if (time < adsr.attack + adsr.decay)
                return 1.0f + (adsr.sustain - 1.0f) * (time - adsr.attack) / adsr.decay;
            return adsr.sustain;
        case KeyStatus::KeyReleased:
            return startLevel - startLevel * time / adsr.release;
        default:
            return 0.0f;
    }
}

inline float Key::Get() {
    float v = sin(2.0 * M_PI * position);
    return Adsr() * v * ((float)velocity / 127.0f);
}

void Key::Proceed() {
    time += 1.0 / sampleRate;
    position += frequency / sampleRate;
    if (status == KeyStatus::KeyReleased && time >= adsr.release)
        Off();
}

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
    key.SetSampleRate(GetSampleRate());
}

void SynthPlugin::Process(uint32_t sampleCount) {

    uint32_t lastFrame = 0;

    for (const Grog::MidiEvent& event : midiIn) {
        Play(lastFrame, event.frame);
        lastFrame = event.frame;
        switch (event.message.GetMessageType()) {
            case Grog::MidiMessageType::NoteOn:
                key.Press(event.message.data1, event.message.data2, ADSR{
                    attack.GetValue(),
                    decay.GetValue(),
                    sustain.GetValue(),
                    release.GetValue()
                });
                break;
            case Grog::MidiMessageType::NoteOff:
                key.Release(event.message.data1, event.message.data2);
                break;
            case Grog::MidiMessageType::Controller:
                if (event.message.GetController() == Grog::MidiController::AllNotesOff) key.Off();
                else if (event.message.GetController() == Grog::MidiController::AllSoundsOff) key.Off();
                break;
            default:
                break;
        }
    }

    Play(lastFrame, sampleCount);
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

void SynthPlugin::Play(uint32_t begin, uint32_t end) {
    float levelValue = level.GetValue();
    float* audioOutLeftPtr = audioOut.GetChannel(0);
    float* audioOutRightPtr = audioOut.GetChannel(1);
    for (uint32_t j = begin; j < end; ++j) {
        float v = key.Get();
        audioOutLeftPtr[j] = v * levelValue;
        audioOutRightPtr[j] = v * levelValue;
        key.Proceed();
    }
}

Grog::AudioPlugin* Grog::InstantiatePlugin() {
    return new SynthPlugin{};
}

void Grog::DestroyPlugin(AudioPlugin* pluginInstance) {
    delete (SynthPlugin*)pluginInstance;
}