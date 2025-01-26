#pragma once

#include <grog_audio_plugin_client/allocator.hpp>

#include <string>


namespace Grog {

    class ControlPort {
    public:
        ControlPort() = delete;
        ControlPort(const std::string& name, float defaultValue, float minimumValue, float maximumValue);
        ~ControlPort();

        float GetValue() const;

        std::string GetName() const;
        float GetDefaultValue() const;
        float GetMinimumValue() const;
        float GetMaximumValue() const;

        void SetPtr(float* ptr);

    private:
        float* ptr = nullptr;
        std::string name = "";
        float defaultValue = 0.0f;
        float minimumValue = 0.0f;
        float maximumValue = 0.0f;
    };

}