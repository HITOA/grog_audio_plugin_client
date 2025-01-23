cmake_minimum_required(VERSION 3.16)
project(grog_audio_plugin_client_example_synth)

grog_add_plugin(grog_audio_plugin_client_example_synth
    FORMATS LV2 Standalone
    LV2URI "https://github.com/HITOA/grog_audio_plugin_client#synth")

target_sources(grog_audio_plugin_client_example_synth
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/synth.cpp)