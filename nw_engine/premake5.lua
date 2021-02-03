--==<engine project>==--
project "nw_engine"
	kind "staticlib" --.lib
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
	targetdir("bin/"..dir_out.."%{prj.name}")
	objdir("bin/int/"..dir_out.."%{prj.name}")
	pchheader "nw_pch.hpp"
	pchsource "src_cpp/nw_pch.cpp"
	files
	{
		"src_cpp/**.c**",
		"src_cpp/**.h**",
		"%{prj.name}/src_glsl/**.glsl",
		"%{prj.name}/src_lua/**.lua",
	}
	includedirs
	{
		"src_cpp/",
		"%{dir_cpp.nw_lib}",
		"%{dir_cpp.nw_glib}",
		"%{dir_cpp.nw_cmd}",
		"%{dir_cpp.glfw}",
		"%{dir_cpp.stbi}",
		"%{dir_cpp.lualib}"
	}
	libdirs
	{
		"%{dir_lib.nw_lib}",
		"%{dir_lib.nw_glib}",
		"%{dir_lib.nw_cmd}",
		"%{dir_lib.glfw}",
		"%{dir_lib.lualib}"
	}
	links
	{
		"nw_lib",
		"nw_glib",
		"nw_cmd",
		"glfw",
		"lualib"
	}
	defines
	{
		--"NW_LINK_DYNAMIC",
		"NW_LINK_STATIC",
		"NW_BUILD_LIB"
	}
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"NW_PLATFORM_WINDOWS"
		}
	filter "configurations:debug"
		defines
		{
			"NW_DEBUG",
			"NWL_DEBUG",
			"GLIB_DEBUG",
			"CMD_DEBUG",
		}
		symbols "on"
	filter "configurations:release"
		defines {
			"NW_RELEASE",
			"NWL_RELEASE",
			"GLIB_RELEASE",
			"CMD_RELEASE",
		}
		optimize "on"
--==</engine_project>==--