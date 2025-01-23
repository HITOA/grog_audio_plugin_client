#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>
#include <lv2_client_definitions.hpp>
#include <lv2.h>

static LV2_Handle instantiate (const struct LV2_Descriptor *descriptor, 
    double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    Grog::AudioPlugin* pluginInstance = Grog::InstantiatePlugin();
    return pluginInstance;
}

static void connect_port (LV2_Handle instance, uint32_t port, void *data_location)
{
    Grog::AudioPlugin* pluginInstance = (Grog::AudioPlugin*)instance;
    if (!pluginInstance)
        return;
    pluginInstance->GetPtrs()[port] = data_location;
}

static void activate (LV2_Handle instance)
{
    Grog::AudioPlugin* pluginInstance = (Grog::AudioPlugin*)instance;
    if (!pluginInstance)
        return;
    pluginInstance->Begin();
}

static void run (LV2_Handle instance, uint32_t sample_count)
{
    Grog::AudioPlugin* pluginInstance = (Grog::AudioPlugin*)instance;
    if (!pluginInstance)
        return;
    pluginInstance->Process(sample_count);
}

static void deactivate (LV2_Handle instance)
{
    Grog::AudioPlugin* pluginInstance = (Grog::AudioPlugin*)instance;
    if (!pluginInstance)
        return;
    pluginInstance->End();
}

static void cleanup (LV2_Handle instance)
{
    Grog::AudioPlugin* pluginInstance = (Grog::AudioPlugin*)instance;
    if (!pluginInstance)
        return;
    delete pluginInstance;
}

static const void* extension_data (const char *uri)
{
    return nullptr;
}

const LV2_Descriptor descriptor = {
    GROG_AUDIO_PLUGIN_CLIENT_LV2URI,
    instantiate,
    connect_port,
    activate,
    run,
    deactivate,
    cleanup,
    extension_data
};

LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor (uint32_t index)
{
    if (!index)
        return &descriptor;
    return nullptr;
}