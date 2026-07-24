cmake_minimum_required(VERSION 3.23...3.31)

project(mcpelauncherstrafesprintfix)

if(NOT CMAKE_ANDROID_ARCH_ABI STREQUAL "x86_64")
    message(FATAL_ERROR "Unsupported ABI")
endif()

set(CMAKE_CXX_STANDARD 23)

set(CPM_SOURCE_CACHE ${PROJECT_SOURCE_DIR}/cpm-cache CACHE PATH "")
include(vendor/CPM.cmake)

add_library(mcpelauncherstrafesprintfix SHARED src/main.cpp)

target_compile_options(mcpelauncherstrafesprintfix PRIVATE -Wall -Wextra -Wconversion)

CPMAddPackage("gh:BasedInc/libhat@0.11.1")
target_link_libraries(mcpelauncherstrafesprintfix PRIVATE libhat::libhat log)

install(TARGETS mcpelauncherstrafesprintfix LIBRARY DESTINATION mods)
