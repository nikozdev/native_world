workspace "native_world"
	architecture "x64"
	configurations
	{
		"debug",
		"release"
	}

dir_out = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

dir_cpp = {}
dir_cpp["nw_lib"] = "nw_lib/src_cpp/"
dir_cpp["nw_glib"] = "nw_glib/src_cpp/"
dir_cpp["nw_engine"] = "nw_engine/src_cpp/"
dir_cpp["nw_cmd"] = "nw_cmd/src_cpp/"
dir_cpp["nw_game"] = "nw_game/src_cpp/"
dir_cpp["glfw"] = "ext/glfw/include/"
dir_cpp["glad"] = "ext/glad/include/"
dir_cpp["stbi"] = "ext/stbi/"
dir_cpp["imgui"] = "ext/imgui/"
dir_cpp["lualib"] = "ext/lualib/src/"

dir_lib = {}
dir_lib["nw_lib"] = "nw_lib/"
dir_lib["nw_glib"] = "nw_glib/"
dir_lib["nw_engine"] = "nw_engine/"
dir_lib["nw_cmd"] = "nw_cmd/"
dir_lib["nw_game"] = "nw_game/"
dir_lib["glfw"] = "ext/glfw/"
dir_lib["glad"] = "ext/glad/"
dir_lib["imgui"] = "ext/imgui/"
dir_lib["lualib"] = "ext/lualib/"

--==<library project>==--
project "nw_lib"
	location "nw_lib"
	kind "staticlib"
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
	targetdir("bin/"..dir_out.."%{prj.name}")
	objdir("bin/"..dir_out.."%{prj.name}")

	files
	{
		"%{dir_cpp.nw_lib}/**.cpp",
		"%{dir_cpp.nw_lib}/**.h**",
	}
	includedirs
	{
		"%{dir_cpp.nw_lib}",
		"%{prj.name}/"
	}
	links
	{
	}
	defines
	{
		--"NWL_LINK_DYNAMIC",
		"NWL_LINK_STATIC",
		"NWL_BUILD_LIB"
	}
	filter "system:windows"
		systemversion "latest"
		defines "NWL_PLATFORM_WINDOWS"
	filter "configurations:debug"
		defines {"NWL_DEBUG"}
		symbols "on"
	filter "configurations:release"
		defines {"NWL_RELEASE"}
		optimize "on"
--==</library project>==--


--==<graphics_library_project>==--
project "nw_glib"
	location "nw_glib"
	kind "staticlib" --.lib
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
	targetdir ("bin/"..dir_out.."%{prj.name}")
	objdir ("bin/int/"..dir_out.."%{prj.name}")
	pchheader "glib_pch.hpp"
	pchsource "%{dir_cpp.nw_glib}glib_pch.cpp"
	files
	{
		"%{dir_cpp.nw_glib}**.cpp",
		"%{dir_cpp.nw_glib}**.h**",
		"%{prj.name}/src_glsl/**.glsl",
	}
	includedirs
	{
		"%{dir_cpp.nw_glib}",
		"%{dir_cpp.nw_lib}",
		"%{dir_cpp.glad}",
		"%{dir_cpp.imgui}",
		"%{dir_cpp.glm}",
		"%{dir_cpp.stbi}"
	}
	libdirs
	{
		"%{dir_lib.nw_glib}",
		"%{dir_lib.nw_lib}",
		"%{dir_lib.glfw}",
		"%{dir_lib.glad}",
		"%{dir_lib.imgui}"
	}
	links
	{
		"glad",
		"imgui",
		"opengl32.lib"
	}
	defines
	{
		"GLIB_BUILD_LIB",
		"GLIB_LINK_STATIC"
		--"GLIB_LINK_DYNAMIC"
	}
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"GLIB_PLATFORM_WINDOWS"
		}
	filter "configurations:debug"
		defines "GLIB_DEBUG"
		symbols "on"
	filter "configurations:release"
		defines "GLIB_RELEASE"
		optimize "on"
--==</graphics_library_project>==--

--==<console_project>==--
project "nw_cmd"
	location "nw_cmd"
	kind "staticlib" --.lib
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
	targetdir ("bin/"..dir_out.."%{prj.name}")
	objdir ("bin/int/"..dir_out.."%{prj.name}")
	pchheader "cmd_pch.hpp"
	pchsource "%{dir_cpp.nw_cmd}cmd_pch.cpp"
	files
	{
		"%{dir_cpp.nw_cmd}**.cpp",
		"%{dir_cpp.nw_cmd}**.h**"
	}
	includedirs
	{
		"%{dir_cpp.nw_cmd}",
		"%{dir_cpp.nw_lib}"
	}
	libdirs
	{
		"%{dir_lib.nw_cmd}",
		"%{dir_lib.nw_lib}"
	}
	links
	{
		"nw_lib"
	}
	defines
	{
		"CMD_BUILD_LIB",
		"CMD_LINK_STATIC"
		--"CMD_LINK_DYNAMIC"
	}
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"CMD_PLATFORM_WINDOWS"
		}
	filter "configurations:debug"
		defines "CMD_DEBUG"
		symbols "on"
	filter "configurations:release"
		defines "CMD_RELEASE"
		optimize "on"
--==</console_project>==--

--==<engine project>==--
project "nw_engine"
	location "nw_engine"
	kind "staticlib" --.lib
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
	targetdir("bin/"..dir_out.."%{prj.name}")
	objdir("bin/int/"..dir_out.."%{prj.name}")
	pchheader "nw_pch.hpp"
	pchsource "%{dir_cpp.nw_engine}nw_pch.cpp"
	files
	{
		"%{dir_cpp.nw_engine}/**.cpp",
		"%{dir_cpp.nw_engine}**.h**",
		"%{prj.name}/src_glsl/**.glsl",
		"%{prj.name}/src_lua/**.lua",
	}
	includedirs
	{
		"%{dir_cpp.nw_engine}",
		"%{dir_cpp.nw_lib}",
		"%{dir_cpp.glfw}",
		"%{dir_cpp.glad}",
		"%{dir_cpp.imgui}",
		"%{dir_cpp.stbi}",
		"%{dir_cpp.lualib}"
	}
	libdirs
	{
		"%{dir_lib.nw_engine}",
		"%{dir_lib.nw_lib}",
		"%{dir_lib.glfw}",
		"%{dir_lib.glad}",
		"%{dir_lib.imgui}",
		"%{dir_lib.lualib}"
	}
	links
	{
		"nw_lib",
		"glfw",
		"glad",
		"imgui",
		"lualib",
		"opengl32.lib"
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
		defines {"NW_DEBUG"}
		symbols "on"
	filter "configurations:release"
		defines {"NW_RELEASE"}
		optimize "on"
--==</engine_project>==--

--==<editor_project>==--
project "nw_game"
	location "nw_game"
	kind "consoleapp" --.exe
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
	targetdir ("bin/"..dir_out.."%{prj.name}")
	objdir ("bin/int/"..dir_out.."%{prj.name}")
	pchheader "nwg_pch.hpp"
	pchsource "%{dir_cpp.nw_game}nwg_pch.cpp"
	files
	{
		"%{dir_cpp.nw_game}**.cpp",
		"%{dir_cpp.nw_game}**.h**",
		"%{prj.name}/src_glsl/**.glsl",
		"%{prj.name}/src_lua/**.lua"
	}
	includedirs
	{
		"%{dir_cpp.nw_game}",
		"%{dir_cpp.nw_lib}",
		"%{dir_cpp.nw_glib}",
		"%{dir_cpp.nw_cmd}",
		"%{dir_cpp.nw_engine}",
		"%{dir_cpp.glm}",
		"%{dir_cpp.imgui}"
	}
	libdirs
	{
		"%{dir_lib.nw_game}",
		"%{dir_lib.nw_lib}",
		"%{dir_lib.nw_glib}",
		"%{dir_lib.nw_cmd}",
		"%{dir_lib.nw_engine}",
		"%{dir_lib.lualib}",
		"%{dir_lib.imgui}",	
	}
	links
	{
		"nw_lib",
		"nw_glib",
		"nw_cmd",
		"nw_engine"
	}
	defines
	{
		"NW_BUILD_EXE",
		"NW_LINK_STATIC",
		--"NW_LINK_DYNAMIC",
		
		"GLIB_BUILD_EXE",
		"GLIB_LINK_STATIC",
		--"GLIB_LINK_DYNAMIC",

		"CMD_BUILD_EXE",
		"CMD_LINK_STATIC",
		--"CMD_LINK_DYNAMIC"

		"NWL_BUILD_EXE",
		"NWL_LINK_STATIC"
		--"NWL_LINK_DYNAMIC"
	}
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"NW_PLATFORM_WINDOWS",
			"NWG_PLATFORM_WINDOWS",
			"GLIB_PLATFORM_WINDOWS",
			"CMD_PLATFORM_WINDOWS"
		}
	filter "configurations:debug"
		defines "NWG_DEBUG"
		symbols "on"
	filter "configurations:release"
		defines "NWG_RELEASE"
		optimize "on"
--==</editor_project>==--

--==<glfw Project>==--
include "ext/glfw"
--==<glad_Project>==--
include "ext/glad"
--==<imgui_project>==--
include "ext/imgui"
--==<lualib_project>==--
include "ext/lualib"