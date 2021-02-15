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
	shadermodel("6.3")
	shaderassembler("assemblycode")
	files
	{
		"src_cpp/**.c**",
		"src_cpp/**.h**",
		dir_lua.."**.**",
		dir_glsl.."**.**",
		dir_hlsl.."**.**",
		dir_data.."**.**",
	}
	includedirs
	{
		dir_data,
		"src_cpp/",
		"ext/",
		"%{dir_cpp.nw_lib}",
		"%{dir_cpp.nw_cmd}",
		"%{dir_cpp.lualib}",
		"%{dir_cpp.imgui}",
	}
	libdirs
	{
		"%{dir_lib.nw_lib}",
		"%{dir_lib.nw_cmd}",
		"%{dir_lib.lualib}",
		"%{dir_lib.imgui}",
	}
	links
	{
		"nw_lib",
		"nw_cmd",
		"imgui",
		"lualib",
		"opengl32",
		"d3d11",
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
	filter("files:**.hlsl")
		flags("excludefrombuild")
		shaderobjectfileoutput(dir_out_res.."%{file.basename}"..".cso")
		shaderassembleroutput(dir_out_int.."%{file.basename}"..".asm")
	filter("files:**_pxl.hlsl")
		removeflags("excludefrombuild")
		shadertype("pixel")
	filter("files:**_vtx.hlsl")
		removeflags("excludefrombuild")
		shadertype("vertex")
	filter("files:**_gmt.hlsl")
		removeflags("excludefrombuild")
		shadertype("geometry")
--==</engine_project>==--