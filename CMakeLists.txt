cmake_minimum_required(VERSION 3.12)
project(basecamp)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#common library
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/common)
file(GLOB_RECURSE PROJECT_FILES "${PROJECT_SOURCE_DIR}/*.h" "${PROJECT_SOURCE_DIR}/*.cpp")
add_library(common STATIC ${PROJECT_FILES})

include_directories(${CMAKE_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR})

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

if (MSVC)
	set_target_properties(common PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
	set_source_files_properties(${PROJECT_SOURCE_DIR}/stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif(MSVC)
#

#d3d12
set(PROJECT_NAME device_d3d12)
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/device_d3d12)
file(GLOB_RECURSE PROJECT_FILES "${PROJECT_SOURCE_DIR}/*.h" "${PROJECT_SOURCE_DIR}/*.cpp")
add_executable(${PROJECT_NAME} WIN32 ${PROJECT_FILES})
target_link_libraries(${PROJECT_NAME} common d3d12 dxgi d3dcompiler)

include_directories(${CMAKE_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR})

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

if (MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
	set_source_files_properties(${PROJECT_SOURCE_DIR}/stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif(MSVC)
#

#add_subdirectory(src/common)
#add_subdirectory(src/device_d3d12)
#add_subdirectory(src/shaders_hlsl)