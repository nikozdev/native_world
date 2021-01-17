project "glad"
	kind "staticlib"
	staticruntime "on"
	language "c"

	targetdir ("bin/"..dir_out.."%{prj.name}")
	objdir ("bin/int/"..dir_out.."%{prj.name}")
	
	files
	{
		"src/glad.c",
		"include/glad.h",
	}
	includedirs
	{
		"include/"
	}

	filter "system:windows"
		systemversion "latest"

	filter { "system:windows", "configurations:release"}
		buildoptions "/mt"