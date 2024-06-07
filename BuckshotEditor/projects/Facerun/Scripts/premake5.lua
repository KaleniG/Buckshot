workspace "Facerun"
	architecture "x86_64"
	startproject "Facerun"
	
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

group "Buckshot"
	include "../../../../Buckshot-ScriptCore"
group ""

project "Facerun"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Binaries")
	objdir ("Intermediates")

	files 
	{
		"Source/**.cs",
		"Properties/**.cs"
	}

	links
	{
		"Buckshot-ScriptCore"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"
	
	filter "configurations:Release"
		optimize "On"
		symbols "Default"
	
	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"