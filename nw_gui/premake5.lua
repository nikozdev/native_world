--==<graphical_user_interface>==--
project("nw_gui")
	kind("staticlib")
	staticruntime("on")
	language("c++")
	cppdialect("c++17")
	systemversion("latest")
	targetdir(dir_out_res)
	objdir(dir_out_int)
	pchheader("nw_gui_pch.hpp")
	pchsource("src_cpp/nw_gui_pch.cpp")
	files{ "%{dir_cpp.own}**.c**", "%{dir_cpp.own}**.h**", }
	includedirs{ "%{dir_cpp.own}", "%{dir_cpp.nw_lib}", "%{dir_cpp.nw_gfx}" }
	links{ "nw_lib", "nw_gfx" }
--==</graphical_user_interface>==--