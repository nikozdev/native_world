workspace "native_world"
	architecture "x64"
	configurations
	{
		"debug",
		"release"
	}
	defines
	{
		"NW_LINK_STATIC",
		--"NW_LINK_DYNAMIC",
		
		"NWL_LINK_STATIC",
		--"NWL_LINK_DYNAMIC"

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
		"CMD_PLATFORM_WINDOWS"
	}
	filter "configurations:debug"
	defines
	{
		"NW_DEBUG",
		"NWL_DEBUG",
		"CMD_DEBUG",
		"NWG_DEBUG",
	}
	symbols "on"
	filter "configurations:release"
	defines
	{
		"NW_RELEASE",
		"NWL_RELEASE",
		"CMD_RELEASE",
		"NWG_RELEASE"
	}


dir_out = "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

dir_cpp = {}
dir_cpp["nw_lib"] = "../nw_lib/src_cpp/"
dir_cpp["nw_cmd"] = "../nw_cmd/src_cpp/"
dir_cpp["nw_engine"] = "../nw_engine/src_cpp/"
dir_cpp["nw_game"] = "../nw_game/src_cpp/"
dir_cpp["glfw"] = "../ext/glfw/include/"
dir_cpp["imgui"] = "../ext/imgui/"
dir_cpp["lualib"] = "../ext/lualib/src/"

dir_lib = {}
dir_lib["nw_lib"] = "../nw_lib/"
dir_lib["nw_cmd"] = "../nw_cmd/"
dir_lib["nw_engine"] = "../nw_engine/"
dir_lib["nw_game"] = "../nw_game/"
dir_lib["glfw"] = "../ext/glfw/"
dir_lib["imgui"] = "../ext/imgui/"
dir_lib["lualib"] = "../ext/lualib/"

--==<library project>==--
include "nw_lib"
--==<console project>==--
include "nw_cmd"
--==<engine project>==--
include "nw_engine"
--==<editor project>==--
include "nw_game"

--==<glfw project>==--
include "ext/glfw"
--==<imgui project>==--
include "ext/imgui"
--==<lualib project>==--
include "ext/lualib"