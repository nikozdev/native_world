workspace "native_world"
	architecture "x64"
	configurations
	{
		"debug",
		"release"
	}
	defines
	{
		"NWL_LINK_STATIC",
		--"NWL_LINK_DYNAMIC"

		"NWC_LINK_STATIC",
		--"NWC_LINK_DYNAMIC"
	}
	filter "system:windows"
	systemversion "latest"
	defines
	{
		"NW_PLATFORM_WINDOWS",
		"NWL_PLATFORM_WINDOWS",
		"NWC_PLATFORM_WINDOWS",
	}
	filter "configurations:debug"
	defines
	{
		"NW_DEBUG",
		"NWL_DEBUG",
		"NWC_DEBUG",
	}
	symbols "on"
	filter "configurations:release"
	defines
	{
		"NW_RELEASE",
		"NWL_RELEASE",
		"NWC_RELEASE",
	}


dir_out_res = "bin/"
dir_out_int = "bin/int/"

dir_cpp = {}
dir_cpp["nw_lib"] = "../nw_lib/src_cpp/"
dir_cpp["nw_cmd"] = "../nw_cmd/src_cpp/"
dir_cpp["nw_engine"] = "../nw_engine/src_cpp/"
dir_cpp["imgui"] = "../ext/imgui/"
dir_cpp["lualib"] = "../ext/lualib/src/"

dir_lua = "src_lua/"
dir_hlsl = "src_hlsl/"
dir_glsl = "src_glsl/"
dir_data = "../data/"

dir_lib = {}
dir_lib["nw_lib"] = "../nw_lib/"
dir_lib["nw_cmd"] = "../nw_cmd/"
dir_lib["nw_engine"] = "../nw_engine/"
dir_lib["nw_game"] = "../nw_game/"
dir_lib["imgui"] = "../ext/imgui/"
dir_lib["lualib"] = "../ext/lualib/"

--==<library project>==--
include "nw_lib"
--==<console project>==--
include "nw_cmd"
--==<engine project>==--
include "nw_engine"

--==<imgui project>==--
include "ext/imgui"
--==<lualib project>==--
include "ext/lualib"