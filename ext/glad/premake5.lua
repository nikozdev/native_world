project "glad"
	kind "staticlib"
	language "c"
	staticruntime "on"
	targetdir ("bin/"..dir_out.."/%{prj.name}")
	objdir ("bin/int/"..dir_out.."/%{prj.name}")
	files
	{
		"src/**.cpp",
		"include/**.h**"
	}
	includedirs
	{
		"include/"
	}
	filter "configurations:debug"
		runtime "debug"
		symbols "on"

	filter "configurations:release"
		runtime "release"
		optimize "on"
