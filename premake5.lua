location "build"

workspace "basecamp"
	configurations { "debug", "profile", "release" }
	platforms { "win64" }
	
	systemversion "10.0.14393.0"
	
	filter "platforms:win64"
		system "Windows"
		architecture "x64"
		
project "shaders"
	kind "Utility"
	
	files { "src/shaders/**.hlsl" }
	
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
	
	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "On"
	
	filter "configurations:profile"
		defines { "NDEBUG" }
		optimize "On"
	
	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "On"

	