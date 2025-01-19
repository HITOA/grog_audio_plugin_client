cmake_minimum_required(VERSION 3.16)
project(grog_audio_plugin_client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(cmake/target_lv2.cmake)
include(cmake/target_standalone.cmake)

function(grog_add_plugin target)
    set(one_value_args
        VERSION)
    set(multi_value_args
        FORMATS)
    cmake_parse_arguments(PARSE_ARGV 1 arg  "" "${one_value_args}" "${multi_value_args}")
    add_library("${target}" STATIC)
endfunction()