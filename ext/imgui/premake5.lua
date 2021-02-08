project "imgui"
	kind "staticlib"
	language "c++"

	targetdir ("bin/".. dir_out.."/%{prj.name}")
	objdir ("bin/int/"..dir_out.."/%{prj.name}")

	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "configurations:debug"
		runtime "debug"
		symbols "on"

	filter "configurations:release"
		runtime "release"
		optimize "on"