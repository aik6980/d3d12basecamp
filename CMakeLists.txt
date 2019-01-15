cmake_minimum_required(VERSION 3.12)
project(basecamp)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(src/common)
add_subdirectory(src/device_d3d12)
add_subdirectory(src/shaders_hlsl)