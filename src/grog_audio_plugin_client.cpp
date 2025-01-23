#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>


Grog::Port::Port(const std::string& name, const std::string& symbol, PortType type, PortDataType dataType) :
    name{ name }, symbol{ symbol }, type{ type }, dataType{ dataType } {}

Grog::Port::~Port() {
}

void Grog::Port::SetChannelPtrs(void** ptr, uint32_t channelCount) {
    channels = ptr;
    this->channelCount = channelCount;
}

uint32_t Grog::Port::GetChannelCount() const {
    return channelCount;
}

Grog::AudioPlugin::~AudioPlugin() {
    for (uint32_t i = 0; i < portCount; ++i)
        delete ports[i];
}

Grog::Port* Grog::AudioPlugin::GetPort(uint32_t idx) {
    return ports[idx];
}

uint32_t Grog::AudioPlugin::GetPortCount() {
    return portCount;
}

void** Grog::AudioPlugin::GetPtrs() {
    return ptrs;
}

Grog::Port* Grog::AudioPlugin::CreatePort(const std::string& name, const std::string& symbol, 
    PortType type, PortDataType dataType) {
    if (portCount + 1 >= GROG_AUDIO_PLUGIN_PORT_MAX || ptrsOffset + 1 >= GROG_AUDIO_PLUGIN_PORT_MAX)
        return nullptr;

    Port* port = new Port{ name, symbol, type, dataType };
    ports[portCount] = port;
    ++portCount;
    port->SetChannelPtrs(&ptrs[ptrsOffset], 1);
    ptrsOffset += 1;
    return port;
}

Grog::Port* Grog::AudioPlugin::CreateAudioPort(const std::string& name, const std::string& symbol, 
    PortType type, uint32_t channelCount) {
    if (portCount + 1 >= GROG_AUDIO_PLUGIN_PORT_MAX || ptrsOffset + channelCount >= GROG_AUDIO_PLUGIN_PORT_MAX)
        return nullptr;
    Port* port = new Port{ name, symbol, type, PortDataType::AudioPort };
    ports[portCount] = port;
    ++portCount;
    port->SetChannelPtrs(&ptrs[ptrsOffset], channelCount);
    ptrsOffset += channelCount;
    return port;
}

Grog::Port* Grog::AudioPlugin::CreateControlPort(const std::string& name, const std::string& symbol, 
    float defaultValue, float minimumValue, float maximumValue) {
    Port* port = CreatePort(name, symbol, PortType::InputPort, PortDataType::ControlPort);
    if (port == nullptr)
        return nullptr;
    port->SetDefault(defaultValue);
    port->SetMinimum(minimumValue);
    port->SetMaximum(maximumValue);
    return port;
}
