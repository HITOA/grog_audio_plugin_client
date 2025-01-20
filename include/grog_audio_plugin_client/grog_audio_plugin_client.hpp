#pragma once

#include <vector>

namespace Grog {

    enum class PortType {
        AudioPort,
        MidiPort,
        ControlPort
    };

    enum class PortDirection {
        InputPort,
        OutputPort
    };

    struct PortInfo {
        const char* symbol;
        const char* name;
        PortType type;
        PortDirection direction;
    };

    class Port {
    public:
        Port(PortInfo& portInfo);
        ~Port();

        inline const PortInfo& GetInfo() const { return info; }

    private:
        PortInfo info;
    };

    class PortManager {
    public:
        Port* CreatePort(const char* name, const char* symbol, PortType type, PortDirection direction);
    
    private:
        std::vector<Port> ports{};
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
        virtual ~AudioPlugin() {}

        virtual void InitializePort(PortManager& manager) = 0;
        virtual void Begin() = 0;
        virtual void Process() = 0;
        virtual void End() = 0;

        virtual AudioPluginGUI* InstantiateGUI() { return nullptr; }
        virtual bool HasGUI() const { return false; }

        virtual const char* GetName() const = 0;
    };

    AudioPlugin* InstantiatePlugin();
}