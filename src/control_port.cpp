#include <grog_audio_plugin_client/control_port.hpp>


Grog::ControlPort::ControlPort(const std::string& name, float defaultValue, float minimumValue, float maximumValue) :
    name{ name }, defaultValue{ defaultValue }, minimumValue{ minimumValue }, maximumValue{ maximumValue } {

}

Grog::ControlPort::~ControlPort() {

}

float Grog::ControlPort::GetValue() const {
    if (ptr)
        return *ptr;
    return defaultValue;
}

std::string Grog::ControlPort::GetName() const {
    return name;
}

float Grog::ControlPort::GetDefaultValue() const {
    return defaultValue;
}

float Grog::ControlPort::GetMinimumValue() const {
    return minimumValue;
}

float Grog::ControlPort::GetMaximumValue() const {
    return maximumValue;
}

void Grog::ControlPort::SetPtr(float* ptr) {
    this->ptr = ptr;
}