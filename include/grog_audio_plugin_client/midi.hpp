#pragma once

#include <cstdint>

namespace Grog {

    enum class MidiMessageType {
        Invalid = 0x00,
        NoteOff = 0x80,
        NoteOn = 0x90,
        NotePressure = 0xA0,
        Controller = 0xB0,
        ProgramChange = 0xC0,
        ChannelPressure = 0xD0,
        SystemExclusive = 0xF0,
        MTCQuarterFrame = 0xF1,
        SongPosition = 0xF2,
        SongSelect = 0xF3,
        TuneRequest = 0xF6,
        Clock = 0xF8,
        Start = 0xFA,
        Continue = 0xFB,
        Stop = 0xFC,
        ActiveSensing = 0xFE,
        Reset = 0xFF
    };

    enum class MidiController {
        MSBBankSelection = 0x00,
        MSBModulation = 0x01,
        MSBBreath = 0x02,
        MSBFoot = 0x04,
        MSBPortamentoTime = 0x05,
        MSBDataEntry = 0x06,
        MSBMainVolume = 0x07,
        MSBBalance = 0x08,
        MSBPanpot = 0x0A,
        MSBExpression = 0x0B,
        MSBEffect1 = 0x0C,
        MSBEffect2 = 0x0D,
        MSBGeneralPurpose1 = 0x10,
        MSBGeneralPurpose2 = 0x11,
        MSBGeneralPurpose3 = 0x12,
        MSBGeneralPurpose4 = 0x13,
        LSBBankSelection = 0x20,
        LSBModulation = 0x21,
        LSBBreath = 0x22,
        LSBFoot = 0x24,
        LSBPortamentoTime = 0x25,
        LSBDataEntry = 0x26,
        LSBMainVolume = 0x27,
        LSBBalance = 0x28,
        LSBPanpot = 0x2A,
        LSBExpression = 0x2B,
        LSBEffect1 = 0x2C,
        LSBEffect2 = 0x2D,
        LSBGeneralPurpose1 = 0x30,
        LSBGeneralPurpose2 = 0x31,
        LSBGeneralPurpose3 = 0x32,
        LSBGeneralPurpose4 = 0x33,
        SustainPedal = 0x40,
        Portamento = 0x41,
        Sostenuto = 0x42,
        SoftPedal = 0x43,
        LegatoFootSwitch = 0x44,
        Hold2 = 0x45,
        SC1SoundVariation = 0x46,
        SC2Timbre = 0x47,
        SC3ReleaseTime = 0x48,
        SC4AttachTime = 0x49,
        SC5Brightness = 0x4A,
        SC6 = 0x4B,
        SC7 = 0x4C,
        SC8 = 0x4D,
        SC9 = 0x4E,
        SC10 = 0x4F,
        GeneralPrupose5 = 0x50,
        GeneralPurpose6 = 0x51,
        GeneralPurpose7 = 0x52,
        GeneralPurpose8 = 0x53,
        PortamentoControl = 0x54,
        E1ReverbDepth = 0x5B,
        E2TremoloDepth = 0x5C,
        E3ChorusDepth = 0x5D,
        E4DetuneDepth = 0x5E,
        E5PhaserDepth = 0x5F,
        DataIncrement = 0x60,
        DataDecrement = 0x61,
        LSBNonRegisteredParameterNumber = 0x62,
        MSBNonRegisteredParameterNumber = 0x63,
        LSBRegisteredParameterNumber = 0x64,
        MSBRegisteredParameterNumber = 0x65,
        AllSoundsOff = 0x78,
        ResetControllers = 0x79,
        LocalControlSwitch = 0x7A,
        AllNotesOff = 0x7B,
        OmniOff = 0x7C,
        OmniOn = 0x7D,
        Mono1 = 0x7E,
        Mono2 = 0x7F
    };

    struct MidiMessage {
        union {
            uint8_t bytes[3];
            struct {
                uint8_t status;
                uint8_t data1;
                uint8_t data2;
            };
        };

        inline MidiMessageType GetMessageType() {
            if (status >= 0x80 && status < 0xF0) {
                return (MidiMessageType)(status & 0xF0U);
            } else {
                return (MidiMessageType)status;
            }
        }
    };
}