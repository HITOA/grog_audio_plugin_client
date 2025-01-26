#include <grog_audio_plugin_client/midi_buffer.hpp>


Grog::MidiBuffer::MidiBuffer() {

}

Grog::MidiBuffer::~MidiBuffer() {

}

void Grog::MidiBuffer::SetPtrs(MidiEvent* first, MidiEvent* last) {
    this->first = first;
    this->last = last;
}

Grog::MidiEvent* Grog::MidiBuffer::begin() {
    return first;
}

Grog::MidiEvent* Grog::MidiBuffer::end() {
    return last;
}

const Grog::MidiEvent* Grog::MidiBuffer::cbegin() const {
    return first;
}

const Grog::MidiEvent* Grog::MidiBuffer::cend() const {
    return last;
}