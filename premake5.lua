-- [A0] Parameters changed/added that are different form the tutorial

workspace "Buckshot"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Buckshot/vendor/GLFW/include"

include "Buckshot/vendor/GLFW"

project "Buckshot"
	location "Buckshot"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "bspch.h"
	pchsource "Buckshot/src/bspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links 
	{ 
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++latest"
		staticruntime "off" -- [A0] Normally is "on"
		
		systemversion "latest"

		defines
		{
			"BS_PLATFORM_WINDOWS",
			"BS_BUILD_DLL",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		runtime "Debug"   -- [A0] Doesn't exist in teh original
		defines "BS_DEBUG"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"   -- [A0] Doesn't exist in teh original
		defines "BS_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"   -- [A0] Doesn't exist in teh original
		defines "BS_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Buckshot/vendor/spdlog/include",
		"Buckshot/src"
	}

	links
	{
		"Buckshot"
	}

	filter "system:windows"
		cppdialect "C++latest"
		staticruntime "on"
		systemversion "latest"

		defines
		{
			"BS_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "BS_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "BS_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "BS_DIST"
		optimize "On"