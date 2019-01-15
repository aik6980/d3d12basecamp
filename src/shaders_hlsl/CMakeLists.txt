project(shaders_hlsl)

file(GLOB_RECURSE VS_FILES "*.vs.hlsl")
file(GLOB_RECURSE PS_FILES "*.ps.hlsl")

add_custom_target(${PROJECT_NAME} SOURCES ${VS_FILES} ${PS_FILES})

set_source_files_properties(${VS_FILES} PROPERTIES VS_TOOL_OVERRIDE "None")
set_source_files_properties(${PS_FILES} PROPERTIES VS_TOOL_OVERRIDE "None")

find_program(FXC fxc
	PATHS "$ENV{PROGRAMFILES\(X86\)}/Windows Kits/10/bin/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/x64")

file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME})
foreach(var ${VS_FILES})
	get_filename_component(file_we ${var} NAME_WE)
	add_custom_command(TARGET ${PROJECT_NAME}
		POST_BUILD 
		COMMAND ${FXC} /T "vs_5_0" /Ni /Od /Fo ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}/${file_we}.obj ${var}
		COMMENT "Effect Compiler ${var}"
		)
endforeach(var)
