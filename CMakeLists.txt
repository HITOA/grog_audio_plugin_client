cmake_minimum_required(VERSION 3.16)
project(grog_audio_plugin_client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_library(grog_audio_plugin_client
    STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/grog_audio_plugin_client.cpp)
    
target_include_directories(grog_audio_plugin_client
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)