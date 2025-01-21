cmake_minimum_required(VERSION 3.16)
project(grog_audio_plugin_client_examples)

option(GROG_AUDIO_PLUGIN_CLIENT_BUILD_EXAMPLES "Build grog audio plugin client example." ON)

if (GROG_AUDIO_PLUGIN_CLIENT_BUILD_EXAMPLES)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/amp)
endif()