location "build"

workspace "basecamp"
	configurations { "debug", "profile", "release" }
	platforms { "win64" }
	
	systemversion ("10.0.10586.0:10.0.14393.0")
	
	filter "platforms:win64"
		system "Windows"
		architecture "x64"

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
	buildcommands '"$(WindowsSdkDir)/bin/x64/fxc.exe" /T [profile] /Ni [asm_file_output] /Fo "$(TargetPath)/%(Filename).obj" "%(FullPath)"'
	buildoutputs '$(TargetPath)/%(Filename).obj'
		
project "shaders"
	kind "Utility"
	targetdir "bin/%{cfg.buildcfg}"
	
	rules { "HLSLCompile" }
	files { "src/shaders/**" }
	
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
		
project "winapp"
	kind "WindowedApp"
	flags {"Winmain"}
	
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	pchheader "stdafx.h"
	pchsource "src/stdafx.cpp"
	files {"src/**.h", "src/**.cpp"}
	
	includedirs { "src" }
	
	links { "d3d12", "dxgi", "d3dcompiler" }
	
	debugdir "$(TargetDir)"
	
	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "On"
	
	filter "configurations:profile"
		defines { "NDEBUG" }
		optimize "On"
	
	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "On"

	