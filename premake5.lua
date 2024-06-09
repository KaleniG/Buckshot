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
IncludeDir["spdlog"] 			= "Buckshot/vendor/spdlog/include"
IncludeDir["GLFW"] 				= "Buckshot/vendor/GLFW/include"
IncludeDir["GLAD"] 				= "Buckshot/vendor/GLAD/include"
IncludeDir["ImGui"] 			= "Buckshot/vendor/ImGui"
IncludeDir["glm"] 				= "Buckshot/vendor/glm"
IncludeDir["EnTT"]				= "Buckshot/vendor/EnTT/include"
IncludeDir["stb_image"] 		= "Buckshot/vendor/stb_image"
IncludeDir["YAML"] 				= "Buckshot/vendor/YAML/include"
IncludeDir["ImGuizmo"]  		= "Buckshot/vendor/ImGuizmo"
IncludeDir["Box2D"]  			= "Buckshot/vendor/Box2D/include"
IncludeDir["mono"]  			= "Buckshot/vendor/mono/include"
IncludeDir["filewatch"] 		= "Buckshot/vendor/filewatch"
IncludeDir["msdf_atlas_gen"]	= "Buckshot/vendor/msdf-atlas-gen/msdf-atlas-gen"
IncludeDir["msdfgen"] 			= "Buckshot/vendor/msdf-atlas-gen/msdfgen"

group "Dependencies"
	include "Buckshot/vendor/GLFW"
	include "Buckshot/vendor/GLAD"
	include "Buckshot/vendor/ImGui"
	include "Buckshot/vendor/YAML"
	include "Buckshot/vendor/Box2D"
	include "Buckshot/vendor/msdf-atlas-gen"
group ""

group "Core"
	include "Buckshot-ScriptCore"

project "Buckshot"
	location "Buckshot"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "off"

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
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/**.h",
		"%{prj.name}/vendor/ImGuizmo/**.cpp",
		"%{prj.name}/vendor/filewatch/**.hpp",
		"%{prj.name}/vendor/filewatch/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.YAML}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.filewatch}",
		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.msdfgen}"
	}

	links 
	{ 
		"GLFW",
		"GLAD",
		"ImGui",
		"YAML",
		"Box2D",
		"msdf-atlas-gen",
		"opengl32.lib",
		"Buckshot/vendor/mono/lib/%{cfg.buildcfg}/libmono-static-sgen.lib" -- Mono
	}

	filter "files:Buckshot/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		links 
		{
			"Ws2_32.lib", -- Mono -> Windows
			"Winmm.lib",
			"Version.lib",
			"Bcrypt.lib"
		}

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
group ""

group "Misc"
project "BuckshotEditor"
	location "BuckshotEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Buckshot/src",
		"Buckshot/vendor",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGuizmo}"
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
group ""