--==<lua_library>==--
project("nw_lua")
	kind("staticlib")
	staticruntime("on")
	language("c")
	systemversion("latest")
	targetdir (dir_out_res)
	objdir (dir_out_int)
	files { "%{dir_cpp.own}**.c**", "%{dir_cpp.own}**.h**", }
	includedirs { "%{dir_cpp.own}" }
--==</lua_library>==--