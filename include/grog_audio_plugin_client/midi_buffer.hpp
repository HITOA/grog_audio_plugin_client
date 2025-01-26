#pragma once

#include <grog_audio_plugin_client/allocator.hpp>
#include <grog_audio_plugin_client/midi.hpp>

#include <cstdint>


namespace Grog {

    struct MidiEvent {
        uint32_t frame;
        MidiMessage message;
    };
    
    class MidiBuffer {
    public:
        MidiBuffer();
        ~MidiBuffer();

        MidiEvent* begin();
        MidiEvent* end();
        const MidiEvent* cbegin() const;
        const MidiEvent* cend() const;

        void SetPtrs(MidiEvent* begin, MidiEvent* end);

    private:
        MidiEvent* first = nullptr;
        MidiEvent* last = nullptr;
    };

}