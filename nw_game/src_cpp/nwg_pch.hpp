#ifndef NWG_PCH_H
#define NWG_PCH_H
/// PreCompiledHeader
/// This file contains the most usable include files
/// cn_pch.cpp file gets it and generates .pch file in the intermediate files
/// .pch header isn't essentially compiles every time we want to run the app
/// .pch header includes in every cpp file of project

#include <native_world.hpp>
#include <cmd_engine.h>
#include <glib_engine.h>

#if (NW_GUI & NW_GUI_IMGUI)
#include <imgui.h>
#include <imgui_internal.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>
#define GUI_DEFAULT_TREE_FLAGS ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick
#endif	// NW_GUI

#endif	// NWG_PCH