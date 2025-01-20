#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>


Grog::Port::Port(PortInfo& portInfo) : info{ portInfo } {

}

Grog::Port::~Port() {

}

Grog::Port* Grog::PortManager::CreatePort(
    const char* name, const char* symbol, 
    PortType type, PortDirection direction) {

    PortInfo portInfo{};
    portInfo.name = name;
    portInfo.symbol = symbol;
    portInfo.type = type;
    portInfo.direction = direction;

    Port& port = ports.emplace_back(portInfo);
    return &port;
}