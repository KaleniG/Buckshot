project "Buckshot-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("../BuckshotEditor/scripts")
	objdir ("../BuckshotEditor/scripts/intermediates")
	
	files 
	{
		"Source/**.cs",
		"Properties/**.cs"
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