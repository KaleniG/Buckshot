workspace "Buckshot"
	architecture "x64"
	startproject "Sandbox"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Buckshot/vendor/GLFW/include"
IncludeDir["GLAD"] = "Buckshot/vendor/GLAD/include"
IncludeDir["ImGui"] = "Buckshot/vendor/ImGui"
IncludeDir["glm"] = "Buckshot/vendor/glm"

include "Buckshot/vendor/GLFW"
include "Buckshot/vendor/GLAD"
include "Buckshot/vendor/ImGui"

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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links 
	{ 
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++latest"
		staticruntime "off"
		
		systemversion "latest"

		defines
		{
			"BS_PLATFORM_WINDOWS",
			"BS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		runtime "Debug"
		defines { "BS_ENABLE_ASSERTS", "BS_DEBUG"}
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		defines { "BS_ENABLE_ASSERTS", "BS_RELEASE"}
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"
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
		"Buckshot/src",
		"Buckshot/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Buckshot"
	}

	filter "system:windows"
		cppdialect "C++latest"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			"BS_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		defines { "BS_ENABLE_ASSERTS", "BS_DEBUG"}
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		defines { "BS_ENABLE_ASSERTS", "BS_RELEASE"}
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"
		defines "BS_DIST"
		optimize "On"