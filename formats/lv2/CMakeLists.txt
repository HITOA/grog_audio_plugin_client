cmake_minimum_required(VERSION 3.16)
project(grog_audio_plugin_client_lv2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(GROG_AUDIO_PLUGIN_CLIENT_LV2_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Grog audio plugin client lv2 source dir.")

function(_grog_add_plugin_lv2 target)
    add_library("${target}_lv2" 
        SHARED 
            ${GROG_AUDIO_PLUGIN_CLIENT_LV2_DIR}/grog_audio_plugin_client_lv2.cpp)
    target_link_libraries("${target}_lv2" PRIVATE "${target}")
    set_target_properties("${target}_lv2"
        PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/lv2"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/lv2"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/lv2"
    )

    # Tool to generate plugin's metadata
    add_executable("${target}_lv2_metadata_generator" 
        ${GROG_AUDIO_PLUGIN_CLIENT_LV2_DIR}/tools/grog_lv2_metadata_generator.cpp)
    target_link_libraries("${target}_lv2_metadata_generator" PRIVATE "${target}")
    set_target_properties("${target}_lv2_metadata_generator"
        PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/tools"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/tools"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/tools"
    )

    # Call it after it is built
    add_custom_command(TARGET "${target}_lv2_metadata_generator"
        POST_BUILD
            COMMAND "$<TARGET_FILE:${target}_lv2_metadata_generator>" 
            "$<TARGET_FILE_DIR:${target}_lv2>" "$<TARGET_FILE_NAME:${target}_lv2>" 
            "${target}_lv2" "${arg_LV2URI}" VERBATIM)
endfunction()