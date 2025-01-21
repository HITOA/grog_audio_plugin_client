cmake_minimum_required(VERSION 3.16)
project(grog_audio_plugin_client_example_amp)

grog_add_plugin(grog_audio_plugin_client_example_amp
    FORMATS LV2 Standalone
    LV2URI "https://github.com/HITOA/grog_audio_plugin_client#amp")

target_sources(grog_audio_plugin_client_example_amp
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/amp.cpp)