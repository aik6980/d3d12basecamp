project(device_d3d12)

file(GLOB_RECURSE PROJECT_FILES "*.h" "*.cpp")
add_executable(${PROJECT_NAME} WIN32 ${PROJECT_FILES})
target_link_libraries(${PROJECT_NAME} common d3d12 dxgi d3dcompiler)

include_directories(${CMAKE_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR})

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

if (MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
	set_source_files_properties(stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif(MSVC)
