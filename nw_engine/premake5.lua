--==<engine project>==--
project "nw_engine"
	kind "staticlib" --.lib
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
	targetdir("bin/"..dir_out.."%{prj.name}")
	objdir("bin/int/"..dir_out.."%{prj.name}")
	pchheader "nw_pch.hpp"
	pchsource "src_cpp/nw_pch.cpp"
	files
	{
		"../data/**.**",
		"src_cpp/**.c**",
		"src_cpp/**.h**",
		"%{prj.name}/src_glsl/**.glsl",
		"%{prj.name}/src_hlsl/**.hlsl",
		"%{prj.name}/src_lua/**.lua",
	}
	includedirs
	{
		"../data/",
		"src_cpp/",
		"%{dir_cpp.nw_lib}",
		"%{dir_cpp.glfw}",
		"%{dir_cpp.lualib}"
	}
	libdirs
	{
		"%{dir_lib.nw_lib}",
		"%{dir_lib.glfw}",
		"%{dir_lib.lualib}"
	}
	links
	{
		"nw_lib",
		"glfw",
		"lualib",
		"opengl32"
	}
	filter "system:windows"
		systemversion "latest"
		defines { "NW_PLATFORM_WINDOWS" }
	filter "configurations:debug"
		defines { "NW_DEBUG" }
		symbols "on"
	filter "configurations:release"
		defines { "NW_RELEASE" }
		optimize "on"
--==</engine_project>==--