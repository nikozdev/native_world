#ifndef NWG_PCH_HPP
#define NWG_PCH_HPP

#include <native_engine.hpp>
using namespace NW;
#include <native_console.hpp>
using namespace NWC;

#include <../ext/glad/glad.h>
#include <glfw/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>
#define GUI_DEFAULT_TREE_FLAGS ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick

#endif	// NWG_PCH_HPP