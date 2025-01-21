#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>
#include <lv2.h>

struct LV2Plugin {
    Grog::AudioPlugin* pluginInstance = nullptr;
};

static LV2_Handle instantiate (const struct LV2_Descriptor *descriptor, 
    double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    LV2Plugin* instance = new LV2Plugin{};
    instance->pluginInstance = Grog::InstantiatePlugin();
    return instance;
}

static void connect_port (LV2_Handle instance, uint32_t port, void *data_location)
{
    
}

static void activate (LV2_Handle instance)
{
    
}

static void run (LV2_Handle instance, uint32_t sample_count)
{
    
}

static void deactivate (LV2_Handle instance)
{
    
}

static void cleanup (LV2_Handle instance)
{
    LV2Plugin* i = (LV2Plugin*)instance;
    delete i->pluginInstance;
    delete i;
}

static const void* extension_data (const char *uri)
{
    return nullptr;
}

LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor (uint32_t index)
{
    return nullptr;
}