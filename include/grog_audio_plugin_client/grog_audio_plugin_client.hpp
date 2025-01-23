#pragma once

#include <cstdint>
#include <string>
#include <vector>

#define GROG_AUDIO_PLUGIN_PORT_MAX 256

#define GROG_AUDIO_PORT_TYPE_MONO 1
#define GROG_AUDIO_PORT_TYPE_STEREO 2


namespace Grog {

    enum class PortType {
        None,
        InputPort,
        OutputPort    
    };

    enum class PortDataType {
        None,
        AudioPort,
        ControlPort,
        MidiPort
    };

    class Port {
    public:
        Port() = delete;
        Port(const std::string& name, const std::string& symbol, PortType type, PortDataType dataType);
        ~Port();

        template<typename T>
        inline T* GetChannel(uint32_t channel) {
            return (T*)channels[channel];
        }

        template<typename T>
        inline T GetValue() {
            return ((T*)channels[0])[0];
        }

        void SetChannelPtrs(void** ptr, uint32_t channelCount);
        uint32_t GetChannelCount() const;

        inline void SetDefault(float value) { defaultValue = value; }
        inline float GetDefault() const { return defaultValue; }

        inline void SetMinimum(float value) { minimumValue = value; }
        inline float GetMinimum() const { return minimumValue; }

        inline void SetMaximum(float value) { maximumValue = value; }
        inline float GetMaximum() const { return maximumValue; }

        inline void SetComment(const std::string& str) { comment = str; }
        inline std::string GetComment() const { return comment; }

        inline bool HasDefaultMinMax() const { return !(defaultValue == minimumValue && minimumValue == maximumValue); }
        inline PortType GetType() const { return type; }
        inline PortDataType GetDataType() const { return dataType; }
        inline std::string GetName() const { return name; }
        inline std::string GetSymbol() const { return symbol; }

    private:
        void** channels = nullptr;
        uint32_t channelCount = 0;
        std::string name = "";
        std::string symbol = "";
        std::string comment = "";
        PortType type = PortType::None;
        PortDataType dataType = PortDataType::None;
        float defaultValue = 0.0f;
        float minimumValue = 0.0f;
        float maximumValue = 0.0f;
    };

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

    public:
        Port* GetPort(uint32_t idx);
        uint32_t GetPortCount();
        void** GetPtrs();

    protected:
        Port* CreatePort(const std::string& name, const std::string& symbol, PortType type, PortDataType dataType);
        Port* CreateAudioPort(const std::string& name, const std::string& symbol, PortType type, uint32_t channelCount);
        Port* CreateControlPort(const std::string& name, const std::string& symbol, float defaultValue, float minimumValue, float maximumValue);

    private:
        Port* ports[GROG_AUDIO_PLUGIN_PORT_MAX] = {};
        uint32_t portCount = 0;
        void* ptrs[GROG_AUDIO_PLUGIN_PORT_MAX];
        size_t ptrsOffset = 0;
    };

    AudioPlugin* InstantiatePlugin();
}