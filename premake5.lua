workspace "Buckshot"
	architecture "x86_64"
	startproject "BuckshotEditor"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	defines
	{
		"_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING",
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS",
		"_SILENCE_ALL_CXX23_DEPRECATION_WARNINGS",
		"_CRT_NONSTDC_NO_DEPRECATE",
		"_CRT_SECURE_NO_DEPRECATE",
		"_CRT_SECURE_NO_WARNINGS"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] 		= "Buckshot/vendor/GLFW/include"
IncludeDir["GLAD"] 		= "Buckshot/vendor/GLAD/include"
IncludeDir["ImGui"] 	= "Buckshot/vendor/ImGui"
IncludeDir["glm"] 		= "Buckshot/vendor/glm"
IncludeDir["EnTT"]		= "Buckshot/vendor/EnTT/include"
IncludeDir["stb_image"] = "Buckshot/vendor/stb_image"
IncludeDir["YAML"] 		= "Buckshot/vendor/YAML/include"

group "Dependencies"
	include "Buckshot/vendor/GLFW"
	include "Buckshot/vendor/GLAD"
	include "Buckshot/vendor/ImGui"
	include "Buckshot/vendor/YAML"
group ""

project "Buckshot"
	location "Buckshot"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "bspch.h"
	pchsource "Buckshot/src/bspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
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
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.YAML}"
	}

	links 
	{ 
		"GLFW",
		"GLAD",
		"ImGui",
		"YAML",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BS_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		runtime "Debug"
		defines "BS_DEBUG"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		defines "BS_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		defines "BS_DIST"
		optimize "on"

project "BuckshotEditor"
	location "BuckshotEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

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
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Buckshot"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BS_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		defines "BS_DEBUG"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		defines "BS_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		defines "BS_DIST"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"
	
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
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.glm}"
	}
	
	links
	{
		"Buckshot"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BS_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		defines "BS_DEBUG"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		defines "BS_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		defines "BS_DIST"
		optimize "on"