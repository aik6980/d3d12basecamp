location "build"

workspace "basecamp"
	configurations { "debug", "profile", "release" }
	platforms { "win64" }
	
	--systemversion ("10.0.10240.0:10.0.15063.0")
	systemversion "10.0.15063.0"
	startproject "device_vulkan"
	
	filter "platforms:win64"
		system "Windows"
		architecture "x64"
		
	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "On"
	
	filter "configurations:profile"
		defines { "NDEBUG" }
		optimize "On"
	
	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "On"

rule "HLSLCompile"
	display "HLSL Compiler"
	fileextension ".hlsl"
	
	propertydefinition {
		name = "profile",
		kind = "string",
		value = ""
	}
	
	propertydefinition {
		name = "asm_file_output",
		kind = "string",
		value = ""
	}
	
	buildmessage 'Compiling %(Filename) with HLSLCompile'
	buildcommands '"$(WindowsSdkDir)bin/$(TargetPlatformVersion)/x64/fxc.exe" /T [profile] /Ni [asm_file_output] /Fo "$(TargetPath)/%(Filename).obj" "%(FullPath)"'
	buildoutputs '$(TargetPath)/%(Filename).obj'
	
rule "HLSL2SPIRVCompile"
	display "HLSL->SPIRV Compiler"
	fileextension ".hlsl"
	
	propertydefinition {
		name = "profile",
		kind = "string",
		value = ""
	}
	
	propertydefinition {
		name = "asm_file_output",
		kind = "string",
		value = ""
	}
	
	buildmessage 'Compiling %(Filename) with HLSLCompile'
	buildcommands '"$(WindowsSdkDir)bin/$(TargetPlatformVersion)/x64/fxc.exe" /T [profile] /Ni [asm_file_output] /Fo "$(TargetPath)/%(Filename).obj" "%(FullPath)"'
	buildoutputs '$(TargetPath)/%(Filename).obj'
	
project "shaders_hlsl"
	kind "Utility"
	targetdir "bin/%{cfg.buildcfg}"
	
	rules { "HLSLCompile" }
	files { "src/shaders_hlsl/**" }
	
	filter "files:**.vs.hlsl"
		HLSLCompileVars {
			profile = "vs_5_0"
		}
		
	filter "files:**.ps.hlsl"
		HLSLCompileVars {
			profile = "ps_5_0"
		}
		
	filter "configurations:debug"
		HLSLCompileVars {
			asm_file_output = "/Fc $(TargetPath)/%(Filename).asm"
		}

project "common"
	kind "StaticLib"
		
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	pchheader "stdafx.h"
	pchsource "src/common/stdafx.cpp"
	files {"src/common/**.h", "src/common/**.cpp"}
	
	includedirs { "src/common" }
	
project "device_d3d12"
	kind "WindowedApp"
	entrypoint "WinMainCRTStartup"
	
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	pchheader "stdafx.h"
	pchsource "src/device_d3d12/stdafx.cpp"
	files {"src/device_d3d12/**.h", "src/device_d3d12/**.cpp"}
	
	includedirs { "src", "src/device_d3d12" }
	
	links { "d3d12", "dxgi", "d3dcompiler", "common", "shaders_hlsl" }
	
	debugdir "$(TargetDir)"

project "device_d3d11"
	kind "WindowedApp"
	entrypoint "WinMainCRTStartup"
	
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	pchheader "stdafx.h"
	pchsource "src/device_d3d11/stdafx.cpp"
	files {"src/device_d3d11/**.h", "src/device_d3d11/**.cpp"}
	
	includedirs { "src", "src/device_d3d11" }
	
	links { "d3d11", "dxgi", "d3dcompiler", "common", "shaders_hlsl" }
	
	debugdir "$(TargetDir)"
	
project "device_vulkan"
	kind "WindowedApp"
	entrypoint "WinMainCRTStartup"
	
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	pchheader "stdafx.h"
	pchsource "src/device_vulkan/stdafx.cpp"
	files {"src/device_vulkan/**.h", "src/device_vulkan/**.cpp"}
	
	includedirs { "$(VK_SDK_PATH)/include", "src", "src/device_vulkan" }
	
	libdirs { "$(VK_SDK_PATH)/bin" }
	links { "vulkan-1.lib", "common" }
	
	debugdir "$(TargetDir)"