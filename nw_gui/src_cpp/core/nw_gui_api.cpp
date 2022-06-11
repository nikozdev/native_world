#include "nw_gui_pch.hpp"
#include "nw_gui_api.h"
#if (defined NW_API)
#include "nw_gui_wapi.h"
#include "nw_gui_gapi.h"
namespace NW
{
    // --data
    ImGuiIO* s_imlib_io = NULL;
    ImGuiPlatformIO* s_imsys_io = NULL;
    ImGuiStyle* s_imstyle = NULL;
    v1bit s_is_docks_enabled = NW_FALSE;
    // --setters
    v1nil gui_set_mouse_enabled(v1bit enable) {
        if (s_imlib_io == NULL) { return; }
        if (enable == NW_TRUTH) { s_imlib_io->ConfigFlags &= ~ImGuiConfigFlags_NoMouse; }
        if (enable == NW_FALSE) { s_imlib_io->ConfigFlags |= ImGuiConfigFlags_NoMouse; }
    }
    v1nil gui_set_docks_enabled(v1bit enable) {
        if (s_imlib_io == NULL) { return; }
        s_is_docks_enabled = enable;
        // if (enable == NW_TRUTH) { s_imlib_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; }
        // if (enable == NW_FALSE) { s_imlib_io->ConfigFlags &= ~ImGuiConfigFlags_DockingEnable; }
    }
    v1nil gui_set_viewp_enabled(v1bit enable) {
        if (s_imlib_io == NULL) { return; }
        if (enable == NW_TRUTH) { s_imlib_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; }
        if (enable == NW_FALSE) { s_imlib_io->ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable; }
    }
    // --predicates
    v1bit gui_is_mouse_enabled() { return !(s_imlib_io->ConfigFlags & ImGuiConfigFlags_NoMouse); }
    v1bit gui_is_docks_enabled() { return s_is_docks_enabled; }
    v1bit gui_is_viewp_enabled() { return s_imlib_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable; }
    // --==<core_methods>==--
    v1bit gui_init(window_handle wndh, device_handle dvch, context_handle ctxh)
    {
        if (s_imlib_io != NULL) { return NW_FALSE; }
        if (s_imsys_io != NULL) { return NW_FALSE; }
        if (s_imstyle != NULL) { return NW_FALSE; }
        
        IMGUI_CHECKVERSION();
        NW_GUI::CreateContext();
        
        s_imlib_io = &NW_GUI::GetIO();
        s_imsys_io = &NW_GUI::GetPlatformIO();
        s_imstyle = &NW_GUI::GetStyle();

        s_imlib_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        s_imlib_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        s_imlib_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        s_imlib_io->ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
        s_imlib_io->ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
        s_imlib_io->ConfigDockingWithShift = NW_TRUTH;

        gui_set_docks_enabled(NW_TRUTH);
        gui_set_viewp_enabled(NW_FALSE);

        NW_GUI::StyleColorsDark();
        if (gui_is_viewp_enabled()) {
            s_imstyle->WindowRounding = 0.0f;
            if constexpr (NW_TRUTH) {
                // button
                s_imstyle->Colors[ImGuiCol_Button] = { 0.3f, 0.3f, 0.3f, 1.0f };
                s_imstyle->Colors[ImGuiCol_ButtonHovered] = { 0.5f, 0.5f, 0.5f, 1.0f };
                s_imstyle->Colors[ImGuiCol_ButtonActive] = { 0.7f, 0.7f, 0.7f, 1.0f };
                // window
                s_imstyle->Colors[ImGuiCol_WindowBg] = { 0.05f, 0.05f, 0.05f, 1.0f };
                // title
                s_imstyle->Colors[ImGuiCol_TitleBg] = { 0.5f, 0.5f, 0.5f, 1.0f };
                s_imstyle->Colors[ImGuiCol_TitleBgActive] = { 0.5f, 0.5f, 0.5f, 1.0f };
                s_imstyle->Colors[ImGuiCol_TitleBgCollapsed] = { 0.5f, 0.5f, 0.5f, 1.0f };
                // resize_grip
                s_imstyle->Colors[ImGuiCol_ResizeGrip] = { 0.5f, 0.5f, 0.5f, 1.0f };
                // header
                s_imstyle->Colors[ImGuiCol_Header] = { 0.35f, 0.35f, 0.35f, 1.0f };
                s_imstyle->Colors[ImGuiCol_HeaderHovered] = { 0.35f, 0.35f, 0.35f, 1.0f };
                s_imstyle->Colors[ImGuiCol_HeaderActive] = { 0.5f, 0.5f, 0.5f, 1.0f };
                // border
                s_imstyle->Colors[ImGuiCol_Border] = { 0.2f, 0.2f, 0.2f, 1.0f };
                s_imstyle->Colors[ImGuiCol_BorderShadow] = { 0.1f, 0.1f, 0.1f, 1.0f };
                // frame
                s_imstyle->Colors[ImGuiCol_FrameBg] = { 0.1f, 0.1f, 0.1f, 1.0f };
                s_imstyle->Colors[ImGuiCol_FrameBgHovered] = { 0.3f, 0.3f, 0.3f, 1.0f };
                s_imstyle->Colors[ImGuiCol_FrameBgActive] = { 0.5f, 0.5f, 0.5f, 1.0f };
                // docking
                s_imstyle->Colors[ImGuiCol_DockingEmptyBg] = { 0.35f, 0.35f, 0.35f, 1.0f };
                s_imstyle->Colors[ImGuiCol_DockingPreview] = { 0.55f, 0.55f, 0.55f, 1.0f };
            }
        }
        if (!gui_wapi_init(wndh)) { return NW_FALSE; }
        if (!gui_gapi_init(dvch, ctxh)) { return NW_FALSE; }

        return NW_TRUTH;
    }
    v1bit gui_quit()
    {
        if (s_imlib_io == NULL) { return NW_FALSE; }
        if (s_imsys_io == NULL) { return NW_FALSE; }
        if (s_imstyle == NULL) { return NW_FALSE; }
        
        if (gui_is_viewp_enabled()) { NW_GUI::DestroyPlatformWindows(); }
        gui_wapi_quit();
        gui_gapi_quit();
        NW_GUI::DestroyContext();

        s_imlib_io = NULL;
        s_imsys_io = NULL;
        s_imstyle = NULL;

        return NW_TRUTH;
    }
    v1nil gui_begin_frame()
    {
        static v1bit s_enable_dockspace = NW_TRUTH;

        gui_wapi_update();
        NW_GUI::NewFrame();
        // setup dockspace
        if (gui_is_docks_enabled()) {
            v1bit full_screen = NW_TRUTH;
            static ImGuiDockNodeFlags dock_flags = ImGuiConfigFlags_None;
            ImGuiWindowFlags wnd_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

            if (full_screen) {
                ImGuiViewport* viewport = NW_GUI::GetMainViewport();
                NW_GUI::SetNextWindowPos(viewport->Pos);
                NW_GUI::SetNextWindowSize(viewport->Size);
                NW_GUI::SetNextWindowViewport(viewport->ID);
                NW_GUI::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                NW_GUI::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                wnd_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                wnd_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            NW_GUI::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            NW_GUI::Begin("dockspace", &s_enable_dockspace, wnd_flags);
            NW_GUI::PopStyleVar();

            if (full_screen) { NW_GUI::PopStyleVar(2); }

            v1f min_size_x = s_imstyle->WindowMinSize.x;
            s_imstyle->WindowMinSize.x = 100.0f;
            s_imstyle->Alpha = 1.0f;
            if (gui_is_docks_enabled()) {
                ImGuiID dockspace_id = NW_GUI::GetID("dockspace");
                NW_GUI::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dock_flags);
            }
            s_imstyle->WindowMinSize.x = min_size_x;
        }
    }
    v1nil gui_end_frame()
    {
        if (gui_is_docks_enabled()) { NW_GUI::End(); }
        NW_GUI::Render();
        gui_gapi_update(NW_GUI::GetDrawData());
        NW_GUI::EndFrame();
        if (gui_is_viewp_enabled()) {
            NW_GUI::UpdatePlatformWindows();
            NW_GUI::RenderPlatformWindowsDefault();
        }
    }
}
#endif  // NW_API