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
		"src_cpp/**.c**",
		"src_cpp/**.h**",
		"src_glsl/**.glsl",
		"src_lua/**.lua"
	}
	includedirs
	{
		"src_cpp/",
		"%{dir_cpp.nw_lib}",
		"%{dir_cpp.nw_glib}",
		"%{dir_cpp.nw_cmd}",
		"%{dir_cpp.nw_engine}",
		"%{dir_cpp.glm}",
		"%{dir_cpp.glad}",
		"%{dir_cpp.glfw}",
		"%{dir_cpp.imgui}"
	}
	libdirs
	{
		"%{dir_lib.nw_lib}",
		"%{dir_lib.nw_glib}",
		"%{dir_lib.nw_cmd}",
		"%{dir_lib.nw_engine}",
		"%{dir_lib.lualib}",
		"%{dir_lib.glfw}",
		"%{dir_lib.glaw}",
		"%{dir_lib.imgui}"
	}
	links
	{
		"nw_lib",
		"nw_glib",
		"nw_cmd",
		"nw_engine",
		"glfw",
		"glad",
		"imgui"
	}
	defines
	{
		"NW_BUILD_EXE",
		"NW_LINK_STATIC",
		--"NW_LINK_DYNAMIC",
		
		"NWL_BUILD_EXE",
		"NWL_LINK_STATIC",
		--"NWL_LINK_DYNAMIC"

		"GLIB_BUILD_EXE",
		"GLIB_LINK_STATIC",
		--"GLIB_LINK_DYNAMIC",

		"CMD_BUILD_EXE",
		"CMD_LINK_STATIC",
		--"CMD_LINK_DYNAMIC"
	}
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"NW_PLATFORM_WINDOWS",
			"NWL_PLATFORM_WINDOWS",
			"NWG_PLATFORM_WINDOWS",
			"GLIB_PLATFORM_WINDOWS",
			"CMD_PLATFORM_WINDOWS"
		}
	filter "configurations:debug"
		defines
		{
			"NW_DEBUG",
			"NWL_DEBUG",
			"GLIB_DEBUG",
			"CMD_DEBUG",
			"NWG_DEBUG",
		}
		symbols "on"
	filter "configurations:release"
		defines
		{
			"NW_RELEASE",
			"NWL_RELEASE",
			"GLIB_RELEASE",
			"CMD_RELEASE",
			"NWG_RELEASE"
		}
		optimize "on"
--==</editor_project>==--