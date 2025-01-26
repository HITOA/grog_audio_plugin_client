#include <grog_audio_plugin_client/grog_audio_plugin_client.hpp>

#include <lv2_client_definitions.hpp>
#include <lv2.h>
#include <lv2/core/lv2_util.h>
#include <lv2/atom/atom.h>
#include <lv2/atom/util.h>
#include <lv2/urid/urid.h>
#include <lv2/midi/midi.h>

#include <vector>

#define GROG_MIDI_EVENT_COUNT_MAX 128


struct Urids {
    LV2_URID midiMidiEvent;
};

struct LV2MidiData {
    const LV2_Atom_Sequence* midiInputPtr = nullptr;
    std::vector<Grog::MidiEvent> midiEvents = {};
};

struct LV2PluginInstanceData {
    LV2_URID_Map* map = nullptr;
    Urids urids = {};
    std::vector<float*> audioPtrs = {};
    LV2MidiData midiInput = {};
};

static void cleanup (LV2_Handle instance)
{
    Grog::AudioPlugin* pluginInstance = (Grog::AudioPlugin*)instance;
    if (!pluginInstance)
        return;
    LV2PluginInstanceData* pluginInstanceData = (LV2PluginInstanceData*)pluginInstance->GetDataPtr();
    pluginInstance->GetAllocator()->Destroy(pluginInstanceData);
    Grog::DestroyPlugin(pluginInstance);
}

static void allocate_audio_ptrs(Grog::AudioPlugin* pluginInstance, LV2PluginInstanceData* pluginInstanceData) {
    uint32_t ptrsCount = 0;

    Grog::AudioBuffer* audioInputBuffer = pluginInstance->GetAudioInputBuffer();
    Grog::AudioBuffer* audioOutputBuffer = pluginInstance->GetAudioOutputBuffer();

    if (audioInputBuffer)
        ptrsCount += audioInputBuffer->GetChannelCount();
    if (audioOutputBuffer)
        ptrsCount += audioOutputBuffer->GetChannelCount();

    pluginInstanceData->audioPtrs.resize(ptrsCount);

    uint32_t ptrsOffset = 0;
    if (audioInputBuffer) {
        audioInputBuffer->SetChannelsPtr(pluginInstanceData->audioPtrs.data());
        ptrsOffset += audioInputBuffer->GetChannelCount();
    }
    if (audioOutputBuffer) {
        audioOutputBuffer->SetChannelsPtr(pluginInstanceData->audioPtrs.data() + ptrsOffset);
        ptrsOffset += audioOutputBuffer->GetChannelCount();
    }
}

static LV2_Handle instantiate (const struct LV2_Descriptor *descriptor, 
    double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    Grog::AudioPlugin* pluginInstance = Grog::InstantiatePlugin();
    
    LV2PluginInstanceData* pluginInstanceData = pluginInstance->GetAllocator()->Create<LV2PluginInstanceData>();

    allocate_audio_ptrs(pluginInstance, pluginInstanceData);

    if (pluginInstance->GetMidiInputBuffer())
        pluginInstanceData->midiInput.midiEvents.reserve(GROG_MIDI_EVENT_COUNT_MAX);
    
    const char* missing = lv2_features_query(features,
        LV2_URID__map, &pluginInstanceData->map, true,
        NULL);

    pluginInstance->SetDataPtr(pluginInstanceData);
    pluginInstance->SetSampleRate(sample_rate);

    if (missing) {
        cleanup((LV2_Handle)pluginInstance);
        return nullptr;
    }

    pluginInstanceData->urids.midiMidiEvent = pluginInstanceData->map->map(pluginInstanceData->map->handle, LV2_MIDI__MidiEvent);

    return pluginInstance;
}

static void connect_port (LV2_Handle instance, uint32_t port, void* data_location)
{
    Grog::AudioPlugin* pluginInstance = (Grog::AudioPlugin*)instance;
    if (!pluginInstance)
        return;
    LV2PluginInstanceData* pluginInstanceData = (LV2PluginInstanceData*)pluginInstance->GetDataPtr();

    uint32_t audioPortCount = pluginInstanceData->audioPtrs.size();
    uint32_t controlPortCount = pluginInstance->GetControlPortCount();

    if (port < audioPortCount) {
        //Bind Audio Port
        pluginInstanceData->audioPtrs[port] = (float*)data_location;
    }
    else if (port < audioPortCount + controlPortCount) {
        //Bind Control Port
        pluginInstance->GetControlPort(port - pluginInstanceData->audioPtrs.size())->SetPtr((float*)data_location);
    }
    else if (port == audioPortCount + controlPortCount && pluginInstance->GetMidiInputBuffer()) {
        //Bind Midi Port
        pluginInstanceData->midiInput.midiInputPtr = (LV2_Atom_Sequence*)data_location;
    }
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
    LV2PluginInstanceData* pluginInstanceData = (LV2PluginInstanceData*)pluginInstance->GetDataPtr();
    
    if (pluginInstanceData->midiInput.midiInputPtr) {
        uint32_t count = 0;
        LV2_ATOM_SEQUENCE_FOREACH(pluginInstanceData->midiInput.midiInputPtr, ev) {
            const uint32_t frame = ev->time.frames;
            if (ev->body.type == pluginInstanceData->urids.midiMidiEvent) {
                const uint8_t* const msg = (const uint8_t*)(ev + 1);
                Grog::MidiEvent event = {};
                event.frame = frame;
                memcpy(event.message.bytes, msg, sizeof(uint8_t) * 3);
                pluginInstanceData->midiInput.midiEvents[count++] = event;
            }
        }
        pluginInstance->GetMidiInputBuffer()->SetPtrs(
            &pluginInstanceData->midiInput.midiEvents[0],
            &pluginInstanceData->midiInput.midiEvents[count]
        );
    }

    pluginInstance->Process(sample_count);
}

static void deactivate (LV2_Handle instance)
{
    Grog::AudioPlugin* pluginInstance = (Grog::AudioPlugin*)instance;
    if (!pluginInstance)
        return;
    pluginInstance->End();
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