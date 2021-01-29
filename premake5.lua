workspace "native_world"
	architecture "x64"
	configurations
	{
		"debug",
		"release"
	}

dir_out = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

dir_cpp = {}
dir_cpp["nw_lib"] = "../nw_library/src_cpp/"
dir_cpp["nw_glib"] = "../nw_glib/src_cpp/"
dir_cpp["nw_engine"] = "../nw_engine/src_cpp/"
dir_cpp["nw_cmd"] = "../nw_console/src_cpp/"
dir_cpp["nw_game"] = "../nw_game/src_cpp/"
dir_cpp["glfw"] = "../ext/glfw/include/"
dir_cpp["glad"] = "../ext/glad/include/"
dir_cpp["stbi"] = "../ext/stbi/"
dir_cpp["imgui"] = "../ext/imgui/"
dir_cpp["lualib"] = "../ext/lualib/src/"

dir_lib = {}
dir_lib["nw_lib"] = "../nw_library/"
dir_lib["nw_glib"] = "../nw_glib/"
dir_lib["nw_engine"] = "../nw_engine/"
dir_lib["nw_cmd"] = "../nw_console/"
dir_lib["nw_game"] = "../nw_game/"
dir_lib["glfw"] = "../ext/glfw/"
dir_lib["glad"] = "../ext/glad/"
dir_lib["imgui"] = "../ext/imgui/"
dir_lib["lualib"] = "../ext/lualib/"


--==<library project>==--
include "nw_library"
--==<graphics_library Project>==--
include "nw_glib"
--==<cmd project>==--
include "nw_console"
--==<engine project>==--
include "nw_engine"
--==<editor project>==--
include "nw_game"

--==<glfw project>==--
include "ext/glfw"
--==<glad project>==--
include "ext/glad"
--==<imgui project>==--
include "ext/imgui"
--==<lualib project>==--
include "ext/lualib"