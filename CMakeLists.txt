cmake_minimum_required(VERSION 3.14...3.31)

project(mcpelauncherstrafesprintfix)

if(NOT CMAKE_ANDROID_ARCH_ABI STREQUAL "x86_64")
    message(FATAL_ERROR "Unsupported ABI")
endif()

set(CMAKE_CXX_STANDARD 23)

set(CPM_SOURCE_CACHE ${PROJECT_SOURCE_DIR}/cpm-cache CACHE PATH "")
include(cmake/CPM.cmake)

add_library(mcpelauncherstrafesprintfix SHARED src/main.cpp)

CPMAddPackage("gh:BasedInc/libhat#69149a1cee5a0228492826bd71af1a32d73ed87e")
target_link_libraries(mcpelauncherstrafesprintfix PRIVATE libhat::libhat log)

install(TARGETS mcpelauncherstrafesprintfix LIBRARY DESTINATION mods)
