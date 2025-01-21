cmake_minimum_required(VERSION 3.16)
project(grog_audio_plugin_client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(GROG_AUDIO_PLUGIN_CLIENT_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Grog audio plugin client source dir.")

find_package(PkgConfig)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/formats/lv2)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/formats/standalone)

add_library(grog_audio_plugin_client STATIC ${GROG_AUDIO_PLUGIN_CLIENT_DIR}/src/grog_audio_plugin_client.cpp)
target_include_directories(grog_audio_plugin_client PUBLIC ${GROG_AUDIO_PLUGIN_CLIENT_DIR}/include)

function(grog_add_plugin target)
    set(one_value_args
        VERSION
        LV2URI)
    set(multi_value_args
        FORMATS)
    cmake_parse_arguments(PARSE_ARGV 1 arg  "" "${one_value_args}" "${multi_value_args}")
    add_library("${target}" STATIC)
    target_link_libraries("${target}" PUBLIC grog_audio_plugin_client)
    set_target_properties("${target}"
        PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/lib"
    )

    if ("LV2" IN_LIST arg_FORMATS)
        _grog_add_plugin_lv2("${target}")
    endif()
endfunction()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)