--==<engine project>==--
project "nw_engine"
	kind "consoleapp"
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
	targetdir(dir_out_res)
	objdir(dir_out_int)
	pchheader "nw_pch.hpp"
	pchsource "src_cpp/nw_pch.cpp"
	files
	{
		"%{dir_cpp.own}**.c**",
		"%{dir_cpp.own}**.h**",
		dir_lua.."**.**",
		dir_data.."**.h", dir_data.."**.rc",  dir_data.."**.aps",
	}
	includedirs
	{
		dir_data,
		"%{dir_cpp.own}",
		"%{dir_cpp.ext}",
		"%{dir_cpp.nw_lib}",
		"%{dir_cpp.nw_gfx}",
		"%{dir_cpp.nw_cmd}",
		"%{dir_cpp.lualib}",
	}
	libdirs
	{
		"%{dir_lib.nw_lib}",
		"%{dir_lib.nw_gfx}",
		"%{dir_lib.nw_cmd}",
		"%{dir_lib.lualib}",
	}
	links
	{
		"nw_lib",
		"nw_gfx",
		"nw_cmd",
		"lualib",
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