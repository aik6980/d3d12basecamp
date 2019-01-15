project(common)

file(GLOB_RECURSE PROJECT_FILES "*.h" "*.cpp")
add_library(${PROJECT_NAME} STATIC ${PROJECT_FILES})

include_directories(${CMAKE_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR})

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

if (MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yustdafx.h")
	set_source_files_properties(stdafx.cpp PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
endif(MSVC)