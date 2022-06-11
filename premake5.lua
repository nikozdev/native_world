workspace "native_world"
	architecture "x64"
	configurations
	{
		"debug",
		"release"
	}

dir_out = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

--Table with keys-included modules
dir_cpp = {}
dir_cpp["nwg"] = "nw_game/src_cpp/"
dir_cpp["nwe"] = "nw_engine/src_cpp/"
dir_cpp["glfw"] = "nw_engine/ext/glfw/include/"
dir_cpp["glad"] = "nw_engine/ext/glad/include/"
dir_cpp["glm"] = "nw_engine/ext/glm/"
dir_cpp["stbi"] = "nw_engine/ext/stbi/"
dir_cpp["imgui"] = "nw_engine/ext/imgui/"
dir_cpp["lua_lib"] = "nw_engine/ext/lua_lib/src/"

dir_lib = {}
dir_lib["nwg"] = "nw_game/"
dir_lib["nwe"] = "nw_engine/"
dir_lib["glfw"] = "nw_engine/ext/glfw/"
dir_lib["glad"] = "nw_engine/ext/glad/"
dir_lib["imgui"] = "nw_engine/ext/imgui/"
dir_lib["lua_lib"] = "nw_engine/ext/lua_lib/"

--==<engine project>==--
project "nw_engine"
	location "nw_engine"
	kind "staticlib" --.lib
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
	--Build project directories
	targetdir("bin/"..dir_out.."%{prj.name}")
	objdir ("bin/int/"..dir_out.."%{prj.name}")

	--Project files we need to compile and include
	files
	{
		"%{dir_cpp.nwe}/**.cpp",
		"%{dir_cpp.nwe}**.h**",
		"%{dir_cpp.glm}**.h**",
		"%{dir_cpp.glm}**.inl",
		"%{dir_cpp.lua_lib}**.h**",
		"%{prj.name}/src_glsl/**.glsl",
		"%{prj.name}/src_lua/**.lua"
	}
	--external projects and libraries
	includedirs
	{
		"%{dir_cpp.nwe}",
		"%{dir_cpp.glfw}",
		"%{dir_cpp.glad}",
		"%{dir_cpp.imgui}",
		"%{dir_cpp.glm}",
		"%{dir_cpp.stbi}",
		"%{dir_cpp.lua_lib}"
	}
	--libraries location
	libdirs
	{
		"%{dir_lib.nwe}",
		"%{dir_lib.glfw}",
		"%{dir_lib.glad}",
		"%{dir_lib.imgui}",
		"%{dir_lib.lua_lib}"
	}
	links
	{
		"glfw",
		"glad",
		"imgui",
		"lua_lib",
		"opengl32.lib"
	}
	--<PrecompiledHeader>--
	pchheader "nw_pch.hpp"
	pchsource "%{dir_cpp.nwe}nw_pch.cpp"
	--<PrecompiledHeader>--

	defines
	{
		"NW_LINK_STATIC",
		"NW_BUILD_LIB"
	}
	--Configs only for windows platforms
	filter "system:windows"
		systemversion "latest"
		--Predefined macroses	
		defines
		{
			"NW_PLATFORM_WINDOWS"
		}
	filter "configurations:debug"
		defines {"NW_DEBUG"}
		symbols "on"
		buildoptions "/MDd"
	filter "configurations:release"
		defines {"NW_RELEASE"}
		optimize "on"
		buildoptions "/MDd"

--==<editor_project>==--
project "nw_game"
	location "nw_game"
	kind "consoleapp" --.exe
	staticruntime "on"
	language "c++"
	cppdialect "c++17"
	--The final linking product will be here
	targetdir ("bin/"..dir_out.."%{prj.name}")
	--The final compillation product will be here
	objdir ("bin/int/"..dir_out.."%{prj.name}")
	--Project files we need to compile and include
	files
	{
		"%{dir_cpp.nwg}**.cpp",
		"%{dir_cpp.nwg}**.h**",
		"%{prj.name}/src_glsl/**.glsl",
		"%{prj.name}/src_lua/**.lua"
	}
	--External projects and libraries
	includedirs
	{
		"%{dir_cpp.nwg}",
		"%{dir_cpp.nwe}",
		"%{dir_cpp.glm}"
	}
	--Libraries location
	libdirs
	{
		"%{dir_lib.nwg}",
		"%{dir_lib.lua_lib}"
	}
	--Dependencies on the local projects
	links
	{
		"nw_engine"
	}
	--<PrecompiledHeader>--
	pchheader "nwg_pch.hpp"
	pchsource "%{dir_cpp.nwg}nwg_pch.cpp"
	--<PrecompiledHeader>--

	defines
	{
		"NW_BUILD_EXE",
		"NW_LINK_STATIC"
	}
	--Configs only for windows platforms
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"NW_PLATFORM_WINDOWS"
		}
	filter "configurations:debug"
		defines "NW_DEBUG"
		symbols "on"
		buildoptions "/MDd"
	filter "configurations:release"
		defines "NW_RELEASE"
		optimize "on"
		buildoptions "/MDd"

--Copy and paste project premake5.lua files
--==<glfw Project>==--
include "nw_engine/ext/glfw"
--==<glad Project>==--
include "nw_engine/ext/glad"
--==<imgui project>==--
include "nw_engine/ext/imgui"
--==<lua_lib project>==--
include "nw_engine/ext/lua_lib"