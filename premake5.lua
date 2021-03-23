workspace("native_world")
	architecture ("x64")
	configurations { "debug", "release" }
	rtti("off")
	characterset("ascii")
	removeflags("excludefrombuild")
--filters
	filter("system:windows")
		systemversion("latest")
		defines { "NW_PLATFORM_WINDOWS" }
	filter("configurations:debug")
		symbols("on")
		defines { "DEBUG", "NW_DEBUG" }
	filter("configurations:release")
		optimize("on")
		defines { "NDEBUG", "NW_RELEASE" }
	filter("files:**.hlsl")
		--flags("excludefrombuild")
		--shadermodel("5.0")
		--shaderassembler("assemblycode")
		--shaderobjectfileoutput("../nw_gfx/src_hlsl/%{file.basename}.cso")
		--shaderassembleroutput("../nw_gfx/src_hlsl/%{file.basename}.asm")
		--removeflags("excludefrombuild")
	filter("files:**vtx_*.hlsl")
		shadertype("vertex")
	filter("files:**pxl_*.hlsl")
		shadertype("pixel")
	filter("files:**gmt_*.hlsl")
		shadertype("geometry")
	filter("files:**.lua")
		removeflags("excludefrombuild")
	filter("files:**.c")
		pchheader("")
		pchsource("")

dir_out_res = "../bin/"
dir_out_int = "../bin/int/"

dir_cpp = {}
dir_cpp["own"] = "src_cpp/"
dir_cpp["ext"] = "ext/"
dir_cpp["nw_lib"] = "../nw_lib/src_cpp/"
dir_cpp["nw_lua"] = "../nw_lua/src_cpp/"
dir_cpp["nw_gfx"] = "../nw_gfx/src_cpp/"
dir_cpp["nw_gui"] = "../nw_gui/src_cpp/"
dir_cpp["nw_cmd"] = "../nw_cmd/src_cpp/"
dir_cpp["nw_eng"] = "../nw_eng/src_cpp/"

dir_data = "../data/"

dir_lib = {}
dir_lib["nw_lib"] = "../nw_lib/"
dir_lib["nw_lua"] = "../nw_lua/"
dir_lib["nw_gfx"] = "../nw_gfx/"
dir_lib["nw_gui"] = "../nw_gui/"
dir_lib["nw_cmd"] = "../nw_cmd/"
dir_lib["nw_eng"] = "../nw_eng/"
--==<library project>==--
include("nw_lib")
--==<graphics project>==--
include("nw_gfx")
--==<graphics_user_interface project>==--
include("nw_gui")
--==<console project>==--
include("nw_cmd")
--==<engine project>==--
include("nw_eng")
--==<lua_library project>==--
include("nw_lua")