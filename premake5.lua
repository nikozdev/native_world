workspace "native_world"
	architecture "x64"
	configurations { "debug", "release" }
	rtti("off")
	defines
	{
		"NW_BUILD_EXE",
		--"NW_BUILD_LIB",
		--"NW_LINK_STATIC",
		--"NW_BUILD_DLL"
		--"NW_LINK_DYNAMIC",

		"NWL_BUILD_LIB",
		"NWL_LINK_STATIC",
		--"NWL_BUILD_DLL"
		--"NWL_LINK_DYNAMIC",
		
		"NWG_BUILD_LIB",
		"NWG_LINK_STATIC",
		--"NWG_BUILD_DLL",
		--"NWG_LINK_DYNAMIC",
		
		"NWC_BUILD_LIB",
		"NWC_LINK_STATIC",
		--"NWC_BUILD_DLL"
		--"NWC_LINK_DYNAMIC",
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
		"DEBUG",
		"NW_DEBUG",
		"NWL_DEBUG",
		"NWG_DEBUG",
		"NWC_DEBUG",
	}
	symbols "on"
	filter "configurations:release"
	defines
	{
		"NDEBUG",
		"NW_RELEASE",
		"NWL_RELEASE",
		"NWG_RELEASE",
		"NWC_RELEASE",
	}
	filter("files:**.hlsl")
		flags("excludefrombuild")
		shadermodel("5.0")
		shaderassembler("assemblycode")
		shaderobjectfileoutput("../nw_gfx/src_hlsl/%{file.basename}.cso")
		shaderassembleroutput("../nw_gfx/src_hlsl/%{file.basename}.asm")
		removeflags("excludefrombuild")
	filter("files:**vtx_*.hlsl")
		shadertype("vertex")
	filter("files:**pxl_*.hlsl")
		shadertype("pixel")
	filter("files:**gmt_*.hlsl")
		shadertype("geometry")
	filter("files:**.lua")
		removeflags("excludefrombuild")

dir_out_res = "bin/"
dir_out_int = "bin/int/"

dir_cpp = {}
dir_cpp["own"] = "src_cpp/"
dir_cpp["ext"] = "ext/"
dir_cpp["nw_lib"] = "../nw_lib/src_cpp/"
dir_cpp["nw_gfx"] = "../nw_gfx/src_cpp/"
dir_cpp["nw_cmd"] = "../nw_cmd/src_cpp/"
dir_cpp["nw_engine"] = "../nw_engine/src_cpp/"
dir_cpp["lualib"] = "../ext/lualib/src/"

dir_data = "../data/"
dir_lua = "../nw_engine/src_lua/"
dir_hlsl = "../nw_gfx/src_hlsl/"
dir_glsl = "../nw_gfx/src_glsl/"

dir_lib = {}
dir_lib["nw_lib"] = "../nw_lib/"
dir_lib["nw_cmd"] = "../nw_cmd/"
dir_lib["nw_engine"] = "../nw_engine/"
dir_lib["nw_game"] = "../nw_game/"
dir_lib["lualib"] = "../ext/lualib/"

--==<library project>==--
include "nw_lib"
--==<graphics project>==--
include "nw_gfx"
--==<console project>==--
include "nw_cmd"
--==<engine project>==--
include "nw_engine"

--==<lualib project>==--
include "ext/lualib"