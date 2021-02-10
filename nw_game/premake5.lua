--==<editor_project>==--
project "nw_game"
	kind "consoleapp" --.exe
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
	targetdir ("bin/"..dir_out.."%{prj.name}")
	objdir ("bin/int/"..dir_out.."%{prj.name}")
	pchheader "nwg_pch.hpp"
	pchsource "src_cpp/nwg_pch.cpp"
	files
	{
		"../data/**.**",
		"src_cpp/**.c**",
		"src_cpp/**.h**"
	}
	includedirs
	{
		"../data/",
		"src_cpp/",
		"../nw_engine/ext/",
		"%{dir_cpp.nw_lib}",
		"%{dir_cpp.nw_cmd}",
		"%{dir_cpp.nw_engine}",
		"%{dir_cpp.glfw}",
		"%{dir_cpp.imgui}"
	}
	libdirs
	{
		"%{dir_lib.nw_lib}",
		"%{dir_lib.nw_cmd}",
		"%{dir_lib.nw_engine}",
		"%{dir_lib.lualib}",
		"%{dir_lib.glfw}",
		"%{dir_lib.imgui}"
	}
	links
	{
		"nw_lib",
		"nw_cmd",
		"nw_engine",
		"glfw",
		"imgui"
	}
	filter "system:windows"
		systemversion "latest"
		defines { "NG_PLATFORM_WINDOWS" }
	filter "configurations:debug"
		defines { "NWG_DEBUG" }
		symbols "on"
	filter "configurations:release"
		defines { "NWG_RELEASE" }
		optimize "on"
--==</editor_project>==--