#pragma once

#include <grog_audio_plugin_client/allocator.hpp>
#include <grog_audio_plugin_client/audio_buffer.hpp>
#include <grog_audio_plugin_client/control_port.hpp>
#include <grog_audio_plugin_client/midi_buffer.hpp>

#include <cstdint>
#include <string>
#include <vector>


namespace Grog {

    class AudioPluginGUI {
    public:
        virtual ~AudioPluginGUI() {}

        virtual void Begin() = 0;
        virtual void Render() = 0;
        virtual void End() = 0;
    };

    class AudioPlugin {
    public:
        virtual ~AudioPlugin();

        virtual void Begin() = 0;
        virtual void Process(uint32_t sampleCount) = 0;
        virtual void End() = 0;

        virtual AudioPluginGUI* InstantiateGUI() { return nullptr; }
        virtual bool HasGUI() const { return false; }

        virtual const char* GetName() const = 0;

        virtual size_t GetMemoryRequirement() const { return 1e6; }
        
    public:
        Allocator* GetAllocator();

        void SetSampleRate(double sampleRate);
        double GetSampleRate();

        void SetDataPtr(void* ptr);
        void* GetDataPtr();

        AudioBuffer* GetAudioInputBuffer();
        AudioBuffer* GetAudioOutputBuffer();

        MidiBuffer* GetMidiInputBuffer();

        ControlPort* GetControlPort(uint32_t idx);
        uint32_t GetControlPortCount() const;

    protected:
        void SetAudioInputBuffer(AudioBuffer* buffer);
        void SetAudioOutputBuffer(AudioBuffer* buffer);

        void SetMidiInputBuffer(MidiBuffer* buffer);

        void AddControlPort(ControlPort* port);
        
    private:
        Allocator allocator = { GetMemoryRequirement() };

        double sampleRate = 0.0;
        void* data = nullptr;

        AudioBuffer* inputAudioBuffer = nullptr;
        AudioBuffer* outputAudioBuffer = nullptr;
        MidiBuffer* inputMidiBuffer = nullptr;
        ControlPort** controlPorts = nullptr;
        uint32_t controlPortCount = 0;
    };

    AudioPlugin* InstantiatePlugin();
    void DestroyPlugin(AudioPlugin* pluginInstance);
}