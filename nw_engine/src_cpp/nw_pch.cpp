#include <nw_pch.hpp>
/// PreCompiled source.cpp file
/// Gets all the code of nw_pch and compile creates .pch from it
#include <examples/imgui_impl_win32.cpp>
#if (NW_GAPI & NW_GAPI_OGL)
#include <../ext/glad/glad.c>
#include <examples/imgui_impl_opengl3.cpp>
#endif
#if (NW_GAPI & NW_GAPI_DX)
#include <examples/imgui_impl_dx11.cpp>
#endif