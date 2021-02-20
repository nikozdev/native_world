workspace "native_world"
	architecture "x64"
	configurations { "debug", "release" }
	shadermodel("4.0")
	shaderassembler("assemblycode")
	defines
	{
		"NW_BUILD_EXE",
		--"NW_BUILD_LIB",
		--"NW_BUILD_DLL"

		"NWL_BUILD_LIB",
		--"NWL_BUILD_DLL"
		
		"NWG_BUILD_LIB",
		--"NWC_BUILD_DLL",
		
		"NWC_BUILD_DLL",
		--"NWC_BUILD_DLL"
	}
	filter "system:windows"
	systemversion "latest"
	defines
	{
		"NW_PLATFORM_WINDOWS",
		"NWL_PLATFORM_WINDOWS",
		"NWG_PLATFORM_WINDOWS",
		"NWC_PLATFORM_WINDOWS",
	}
	filter "configurations:debug"
	defines
	{
		"NW_DEBUG",
		"NWL_DEBUG",
		"NWG_DEBUG",
		"NWC_DEBUG",
	}
	symbols "on"
	filter "configurations:release"
	defines
	{
		"NW_RELEASE",
		"NWL_RELEASE",
		"NWG_RELEASE",
		"NWC_RELEASE",
	}
	filter("files:**.hlsl")
		flags("excludefrombuild")
		shaderobjectfileoutput("../nw_gfx/src_hlsl/%{file.basename}.cso")
		shaderassembleroutput("../nw_gfx/src_hlsl/%{file.basename}.asm")
	filter("files:**_pxl.hlsl")
		removeflags("excludefrombuild")
		shadertype("pixel")
	filter("files:**_vtx.hlsl")
		removeflags("excludefrombuild")
		shadertype("vertex")
	filter("files:**_gmt.hlsl")
		removeflags("excludefrombuild")
		shadertype("geometry")


dir_out_res = "bin/"
dir_out_int = "bin/int/"

dir_cpp = {}
dir_cpp["own"] = "src_cpp/"
dir_cpp["ext"] = "ext/"
dir_cpp["nw_lib"] = "../nw_lib/src_cpp/"
dir_cpp["nw_gfx"] = "../nw_gfx/src_cpp/"
dir_cpp["nw_cmd"] = "../nw_cmd/src_cpp/"
dir_cpp["nw_engine"] = "../nw_engine/src_cpp/"
dir_cpp["imgui"] = "../ext/imgui/"
dir_cpp["lualib"] = "../ext/lualib/src/"

dir_data = "../data/"
dir_lua = "src_lua/"
dir_hlsl = "src_hlsl/"
dir_glsl = "src_glsl/"

dir_lib = {}
dir_lib["nw_lib"] = "../nw_lib/"
dir_lib["nw_cmd"] = "../nw_cmd/"
dir_lib["nw_engine"] = "../nw_engine/"
dir_lib["nw_game"] = "../nw_game/"
dir_lib["imgui"] = "../ext/imgui/"
dir_lib["lualib"] = "../ext/lualib/"

--==<library project>==--
include "nw_lib"
--==<graphics project>==--
include "nw_gfx"
--==<console project>==--
include "nw_cmd"
--==<engine project>==--
include "nw_engine"

--==<imgui project>==--
include "ext/imgui"
--==<lualib project>==--
include "ext/lualib"