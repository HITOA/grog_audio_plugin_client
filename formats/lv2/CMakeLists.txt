cmake_minimum_required(VERSION 3.16)
project(grog_audio_plugin_client_lv2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(GROG_AUDIO_PLUGIN_CLIENT_LV2_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "Grog audio plugin client lv2 source dir.")

if (PKG_CONFIG_FOUND)
    pkg_check_modules(LV2 lv2)
endif()

if (LV2_FOUND)
    set(GROG_AUDIO_PLUGIN_CLIENT_LV2_SDK_OK TRUE CACHE INTERNAL "Grog audio plugin client lv2 SDL is ok.")
endif()

function(_grog_add_plugin_lv2 target)
    if(NOT DEFINED GROG_AUDIO_PLUGIN_CLIENT_LV2_SDK_OK)
        message(WARNING "Not building lv2 plugin. The lv2 SDK could not be found.")
        return()
    endif()

    set(GROG_AUDIO_PLUGIN_CLIENT_LV2URI "${arg_LV2URI}")
    configure_file("${GROG_AUDIO_PLUGIN_CLIENT_LV2_DIR}/lv2_client_definitions.hpp.in"
        "${CMAKE_BINARY_DIR}/${target}/headers/lv2_client_definitions.hpp" @ONLY)

    add_library("${target}_lv2" 
        SHARED 
            ${GROG_AUDIO_PLUGIN_CLIENT_LV2_DIR}/lv2_client.cpp)
    target_link_libraries("${target}_lv2" PRIVATE "${target}" "${LV2_LIBRARIES}")
    target_include_directories("${target}_lv2" PRIVATE "${CMAKE_BINARY_DIR}/${target}/headers")
    set_target_properties("${target}_lv2"
        PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/${target}.lv2"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/${target}.lv2"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/${target}.lv2"
    )
    set_target_properties("${target}_lv2" PROPERTIES LINK_FLAGS_RELEASE -s)

    # Tool to generate plugin's metadata
    add_executable("${target}_lv2_metadata" 
        ${GROG_AUDIO_PLUGIN_CLIENT_LV2_DIR}/tools/grog_lv2_metadata_generator.cpp)
    target_link_libraries("${target}_lv2_metadata" PRIVATE "${target}")
    set_target_properties("${target}_lv2_metadata"
        PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/tools"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/tools"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}/tools"
    )

    # Call it after it is built
    add_custom_command(TARGET "${target}_lv2_metadata"
        POST_BUILD
            COMMAND "$<TARGET_FILE:${target}_lv2_metadata>" 
            "$<TARGET_FILE_DIR:${target}_lv2>" "$<TARGET_FILE_NAME:${target}_lv2>" 
            "${target}_lv2" "${arg_LV2URI}" VERBATIM)
endfunction()