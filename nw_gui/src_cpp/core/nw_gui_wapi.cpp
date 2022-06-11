#include "nw_gui_pch.hpp"
#include "nw_gui_wapi.h"
#if (defined NW_WAPI)
#   if (NW_WAPI & NW_WAPI_WIN)
namespace NW
{
    // --types
    struct win_viewport_data
    {
    public:
        HWND    Hwnd;
        bool    HwndOwned;
        DWORD   DwStyle;
        DWORD   DwExStyle;
    public:
        win_viewport_data() { Hwnd = NW_NULL; HwndOwned = NW_FALSE;  DwStyle = DwExStyle = 0; }
        ~win_viewport_data() { IM_ASSERT(Hwnd == NW_NULL); }
    };
    // --data
    static HWND s_wnd = NW_NULL;
    static WNDCLASSEX s_wnd_class = { 0 };
    static ImGuiMouseCursor s_last_crs = ImGuiMouseCursor_COUNT;
    static LONG s_time = 0;
    static LONG s_ticks_per_sec = 0;
    static BOOL s_update_monitors = NW_TRUTH;
    // --aux_functions
    NW_API BOOL WINAPI win_update_monitors_enum(HMONITOR monitor, HDC device, LPRECT rect, LPARAM lparam)
    {
        MONITORINFO info = { 0 };
        info.cbSize = sizeof(MONITORINFO);
        if (!GetMonitorInfo(monitor, &info)) { return NW_TRUTH; }
        ImGuiPlatformMonitor immonitor;
        immonitor.MainPos = ImVec2(static_cast<v1f>(info.rcMonitor.left), static_cast<v1f>(info.rcMonitor.top));
        immonitor.MainSize = ImVec2(static_cast<v1f>(info.rcMonitor.right - info.rcMonitor.left), static_cast<v1f>(info.rcMonitor.bottom - info.rcMonitor.top));
        immonitor.WorkPos = ImVec2(static_cast<v1f>(info.rcWork.left), static_cast<v1f>(info.rcWork.top));
        immonitor.WorkSize = ImVec2(static_cast<v1f>(info.rcWork.right - info.rcWork.left), static_cast<v1f>(info.rcWork.bottom - info.rcWork.top));
        if (info.dwFlags & MONITORINFOF_PRIMARY) { s_imsys_io->Monitors.push_front(immonitor); }
        else { s_imsys_io->Monitors.push_back(immonitor); }
        return NW_TRUTH;
    }
    NW_API LRESULT WINAPI gui_wapi_wnd_event_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (NW_GUI::gui_wapi_event_proc(wnd, msg, wparam, lparam)) { return NW_TRUTH; }

        if (ImGuiViewport* viewport = NW_GUI::FindViewportByPlatformHandle(wnd)) {
            switch (msg) {
            case WM_CLOSE: { viewport->PlatformRequestClose = NW_TRUTH; return 0; break; }
            case WM_MOVE: { viewport->PlatformRequestMove = NW_TRUTH; break; }
            case WM_SIZE: { viewport->PlatformRequestResize = NW_TRUTH; break; }
            case WM_MOUSEACTIVATE: if (viewport->Flags & ImGuiViewportFlags_NoFocusOnClick) { return MA_NOACTIVATE; break; }
            case WM_NCHITTEST: {
                // let mouse pass-through the window; this will allow the back-end to set io.MouseHoveredViewport properly (which is optional);
                // the ImGuiViewportFlags_NoInputs flag is set while dragging a viewport, as want to detect the window behind the one we are dragging;
                // if you cannot easily access those viewport flags from your windowing/event code: you may manually synchronize its state e.g. in
                // your main loop after calling UpdateWindows(). Iterate all viewports/platform windows and pass the flag to your windowing system.
                if (viewport->Flags & ImGuiViewportFlags_NoInputs) { return HTTRANSPARENT; }
                break;
            }
            }
        }
        return ::DefWindowProc(wnd, msg, wparam, lparam);
    }
    v1nil gui_wapi_update_monitors()
    {
        s_imsys_io->Monitors.resize(0);
        ::EnumDisplayMonitors(NW_NULL, NW_NULL, win_update_monitors_enum, NW_NULL);
        s_update_monitors = NW_FALSE;
    }
    v1nil gui_wapi_get_viewport_flags_imstyle(ImGuiViewportFlags flags, DWORD* out_style, DWORD* out_ex_style)
    {
        if (flags & ImGuiViewportFlags_NoDecoration) { *out_style = WS_POPUP; }
        else { *out_style = WS_OVERLAPPEDWINDOW; }

        if (flags & ImGuiViewportFlags_NoTaskBarIcon) { *out_ex_style = WS_EX_TOOLWINDOW; }
        else { *out_ex_style = WS_EX_APPWINDOW; }

        if (flags & ImGuiViewportFlags_TopMost) { *out_ex_style |= WS_EX_TOPMOST; }
    }
    v1bit gui_wapi_update_mouse_coord()
    {
        // set os mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
        // (when multi-viewports are enabled, all imgui positions are same as os positions)
        if (s_imlib_io->WantSetMousePos) {
            POINT pos = { static_cast<v1s>(s_imlib_io->MousePos.x), static_cast<int>(s_imlib_io->MousePos.y) };
            if ((s_imlib_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) == 0) { ::ClientToScreen(s_wnd, &pos); }
            ::SetCursorPos(pos.x, pos.y);
        }

        s_imlib_io->MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
        s_imlib_io->MouseHoveredViewport = 0;
        // set imgui mouse position
        POINT mouse_screen_pos;
        if (!::GetCursorPos(&mouse_screen_pos)) { return NW_FALSE; }
        if (HWND focused_hwnd = ::GetForegroundWindow()) {
            if (::IsChild(focused_hwnd, s_wnd)) { focused_hwnd = s_wnd; }
            if (s_imlib_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                // Multi-viewport mode: mouse position in OS absolute coordinates (io.MousePos is (0,0) when the mouse is on the upper-left of the primary monitor)
                // This is the position you can get with GetCursorPos(). In theory adding viewport->Pos is also the reverse operation of doing ScreenToClient().
                if (NW_GUI::FindViewportByPlatformHandle(static_cast<ptr_t>(focused_hwnd)) != NW_NULL) {
                    s_imlib_io->MousePos = ImVec2(static_cast<v1f>(mouse_screen_pos.x), static_cast<v1f>(mouse_screen_pos.y));
                }
            }
            else {
                // single viewport mode: mouse position in client window coordinates (io.MousePos is (0,0) when the mouse is on the upper-left corner of the app window.)
                // this is the position you can get with GetCursorPos() + ScreenToClient() or from Wm_cursorMOVE.
                if (focused_hwnd == s_wnd) {
                    POINT mouse_client_pos = mouse_screen_pos;
                    ::ScreenToClient(focused_hwnd, &mouse_client_pos);
                    s_imlib_io->MousePos = ImVec2(static_cast<v1f>(mouse_client_pos.x), static_cast<v1f>(mouse_client_pos.y));
                }
            }
        }
        // (optional) when using multiple viewports: set io.MouseHoveredViewport to the viewport the os mouse cursor is hovering;
        // important: this information is not easy to provide and many high-level windowing library won't be able to provide it corImGuiRect(ly, because
        // - this is _ignoring_ viewports with the ImGuiViewportFlags_NoInputs flag (pass-through windows);
        // - this is _regardless_ of whether another viewport is focused or being dragged from;
        // if ImGuiBackendFlags_HasMouseHoveredViewport is not set by the back-end, imgui will ignore this field and infer the information by relying on the
        // ImGuiRect(angles and last focused time of every viewports it knows about;
        // it will be unaware of foreign windows that may be sitting between or over your windows.
        if (HWND hovered_hwnd = ::WindowFromPoint(mouse_screen_pos)) {
            if (ImGuiViewport* viewport = NW_GUI::FindViewportByPlatformHandle(static_cast<ptr_t>(hovered_hwnd))) {
                if ((viewport->Flags & ImGuiViewportFlags_NoInputs) == 0) {
                    // FIXME: we still get our NoInputs window with WM_NCHITTEST/HTTRANSPARENT code when decorated?
                    s_imlib_io->MouseHoveredViewport = viewport->ID;
                }
            }
        }
        
        return NW_TRUTH;
    }
    v1bit gui_wapi_update_mouse_cursor()
    {
        if (s_imlib_io->ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) { return NW_FALSE; }

        ImGuiMouseCursor cursor = NW_GUI::GetMouseCursor();
        if (cursor == ImGuiMouseCursor_None || s_imlib_io->MouseDrawCursor) {
            // hide operation_system mouse cursor if imgui is drawing it or if it wants no cursor
            ::SetCursor(NW_NULL);
        }
        else {
            // show operation_system mouse cursor
            LPTSTR win32_cursor = IDC_ARROW;
            switch (cursor) {
            case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
            case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
            case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
            case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
            case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
            case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
            case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
            case ImGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
            case ImGuiMouseCursor_NotAllowed:   win32_cursor = IDC_NO; break;
            }
            ::SetCursor(LoadCursor(NW_NULL, win32_cursor));
        }
        return NW_TRUTH;
    }
}
namespace NW
{
    v1bit gui_wapi_init(window_handle window)
    {
        if (s_wnd != NW_NULL) { return NW_FALSE; }
        s_wnd = window;
        if constexpr (NW_TRUTH) {
            if (!::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&s_ticks_per_sec))) { return NW_FALSE; }
            if (!::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&s_time))) { return NW_FALSE; }
        }
        // setup back-end capabilities flags
        if constexpr (NW_TRUTH) {
            s_imlib_io->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // we can honor GetMouseCursor() values (optional)
            s_imlib_io->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // we can honor io.WantSetMousePos requests (optional, rarely used)
            s_imlib_io->BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;    // we can create multi-viewports on the Platform side (optional)
            s_imlib_io->BackendFlags |= ImGuiBackendFlags_HasMouseHoveredViewport; // we can set io.MouseHoveredViewport corImGuiRect(ly (optional, not easy)
            s_imlib_io->BackendPlatformName = "nw_gfx_win";
            // our mouse update function expect platform handle to be filled for the main viewport
        }
        // initialize imgui platform interface
        if constexpr (NW_TRUTH) {
            if (gui_is_viewp_enabled()) { if (gui_wapi_wnd_init() == NW_FALSE) { return NW_FALSE; } }
        }
        // keyboard mapping;
        // imgui will use those indices to peek into the io.KeysDown[] array that we will update during the application lifetime.
        if constexpr (NW_TRUTH) {
            s_imlib_io->KeyMap[ImGuiKey_Tab] = VK_TAB;
            s_imlib_io->KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
            s_imlib_io->KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
            s_imlib_io->KeyMap[ImGuiKey_UpArrow] = VK_UP;
            s_imlib_io->KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
            s_imlib_io->KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
            s_imlib_io->KeyMap[ImGuiKey_PageDown] = VK_NEXT;
            s_imlib_io->KeyMap[ImGuiKey_Home] = VK_HOME;
            s_imlib_io->KeyMap[ImGuiKey_End] = VK_END;
            s_imlib_io->KeyMap[ImGuiKey_Insert] = VK_INSERT;
            s_imlib_io->KeyMap[ImGuiKey_Delete] = VK_DELETE;
            s_imlib_io->KeyMap[ImGuiKey_Backspace] = VK_BACK;
            s_imlib_io->KeyMap[ImGuiKey_Space] = VK_SPACE;
            s_imlib_io->KeyMap[ImGuiKey_Enter] = VK_RETURN;
            s_imlib_io->KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
            s_imlib_io->KeyMap[ImGuiKey_KeyPadEnter] = VK_RETURN;
            s_imlib_io->KeyMap[ImGuiKey_A] = 'A';
            s_imlib_io->KeyMap[ImGuiKey_C] = 'C';
            s_imlib_io->KeyMap[ImGuiKey_V] = 'V';
            s_imlib_io->KeyMap[ImGuiKey_X] = 'X';
            s_imlib_io->KeyMap[ImGuiKey_Y] = 'Y';
            s_imlib_io->KeyMap[ImGuiKey_Z] = 'Z';
        }

        return NW_TRUTH;
    }
    v1bit gui_wapi_quit()
    {
        if (s_wnd != NW_NULL) { return NW_FALSE; }

        s_wnd = NW_NULL;

        if (gui_is_viewp_enabled()) { gui_wapi_wnd_quit(); }
        
        return NW_TRUTH;
    }
    v1nil gui_wapi_update()
    {
        IM_ASSERT(s_imlib_io->Fonts->IsBuilt() && "font atlas is not built; it is generally built by the renderer back-end;");
        // setup display size (every frame to accommodate for window resizing)
        RECT rect{ 0u };
        ::GetClientRect(s_wnd, &rect);
        s_imlib_io->DisplaySize = ImVec2(static_cast<v1f>(rect.right - rect.left), static_cast<v1f>(rect.bottom - rect.top));
        if (s_update_monitors) { gui_wapi_update_monitors(); }
        // setup delta time
        INT64 current_time = 0;
        ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_time));
        s_imlib_io->DeltaTime = static_cast<v1f>(current_time - s_time) / s_ticks_per_sec;
        s_time = current_time;
        // read keyboard modifiers inputs
        s_imlib_io->KeyCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
        s_imlib_io->KeyShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
        s_imlib_io->KeyAlt = (::GetKeyState(VK_MENU) & 0x8000) != 0;
        s_imlib_io->KeySuper = NW_FALSE;
        // io.KeysDown[], io.MousePos, io.MouseDown[], io.MouseWheel: filled by the window procedure handler below.
        // update os mouse position
        gui_wapi_update_mouse_coord();
        // update os mouse cursor with the cursor requested by imgui
        ImGuiMouseCursor mouse_cursor = s_imlib_io->MouseDrawCursor ? ImGuiMouseCursor_None : NW_GUI::GetMouseCursor();
        if (s_last_crs != mouse_cursor) {
            s_last_crs = mouse_cursor;
            gui_wapi_update_mouse_cursor();
        }
    }
}
namespace NW
{
    // --wnd_core_functions
    v1bit gui_wapi_wnd_init()
    {
        if (s_imsys_io == NW_NULL) { return NW_FALSE; }
       
        if constexpr (NW_TRUTH) {
            s_imsys_io->Platform_CreateWindow = gui_wapi_wnd_create;
            s_imsys_io->Platform_DestroyWindow = gui_wapi_wnd_delete;
            s_imsys_io->Platform_UpdateWindow = gui_wapi_wnd_update;
            s_imsys_io->Platform_RenderWindow = gui_wapi_wnd_render;
            s_imsys_io->Platform_ShowWindow = gui_wapi_wnd_show;
            s_imsys_io->Platform_SwapBuffers = gui_wapi_wnd_swap;
            s_imsys_io->Platform_OnChangedViewport = NW_NULL;
            s_imsys_io->Platform_CreateVkSurface = NW_NULL;
            s_imsys_io->Platform_GetWindowPos = gui_wapi_wnd_get_coord;
            s_imsys_io->Platform_GetWindowSize = gui_wapi_wnd_get_size;
            s_imsys_io->Platform_GetWindowDpiScale = NW_NULL;
            s_imsys_io->Platform_SetWindowPos = gui_wapi_wnd_set_coord;
            s_imsys_io->Platform_SetWindowSize = gui_wapi_wnd_set_size;
            s_imsys_io->Platform_SetWindowTitle = gui_wapi_wnd_set_title;
            s_imsys_io->Platform_SetWindowAlpha = gui_wapi_wnd_set_alpha;
            s_imsys_io->Platform_SetWindowFocus = gui_wapi_wnd_set_focused;
            s_imsys_io->Platform_SetImeInputPos = NW_NULL;
            s_imsys_io->Platform_GetWindowFocus = gui_wapi_wnd_is_focused;
            s_imsys_io->Platform_GetWindowMinimized = gui_wapi_wnd_is_minimized;
        }
        ImGuiViewport* main_viewport = NW_GUI::GetMainViewport();
        main_viewport->PlatformHandle = main_viewport->PlatformHandleRaw = s_wnd;
        // register gui-viewport-window class;
        if constexpr (NW_TRUTH) {
            s_wnd_class.cbSize = sizeof(WNDCLASSEX);
            s_wnd_class.style = CS_HREDRAW | CS_VREDRAW;
            s_wnd_class.lpfnWndProc = gui_wapi_wnd_event_proc;
            s_wnd_class.cbClsExtra = 0;
            s_wnd_class.cbWndExtra = 0;
            s_wnd_class.hInstance = ::GetModuleHandle(NW_NULL);
            s_wnd_class.hIcon = NW_NULL;
            s_wnd_class.hIconSm = NW_NULL;
            s_wnd_class.hCursor = NW_NULL;
            s_wnd_class.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND + 1);
            s_wnd_class.lpszMenuName = NW_NULL;
            s_wnd_class.lpszClassName = _T("imwindow");
            if (::RegisterClassEx(&s_wnd_class) == NW_FALSE) { throw init_error(__FILE__, __LINE__); return NW_FALSE; }
            gui_wapi_update_monitors();
        }
        // register main window handle (which is owned by the main application, not by us)
        // this is mostly for simplicity and consistency, so that our code (e.g. mouse handling etc.)
        // can use same logic for main and secondary viewports.
        if constexpr (NW_TRUTH) {
            win_viewport_data* data = IM_NEW(win_viewport_data)();
            data->Hwnd = s_wnd;
            data->HwndOwned = NW_FALSE;
            main_viewport->PlatformUserData = data;
            main_viewport->PlatformHandle = s_wnd;
        }

        return NW_TRUTH;
    }
    v1nil gui_wapi_wnd_quit()
    {
        if (s_imsys_io == NW_NULL) { return; }
        ::UnregisterClass(s_wnd_class.lpszClassName, s_wnd_class.hInstance);
        s_wnd_class = { 0 };
        NW_GUI::DestroyPlatformWindows();
    }
    v1nil gui_wapi_wnd_create(ImGuiViewport* viewport)
    {
        win_viewport_data* data = IM_NEW(win_viewport_data)();
        viewport->PlatformUserData = data;
        // select style and parent window
        gui_wapi_get_viewport_flags_imstyle(viewport->Flags, &data->DwStyle, &data->DwExStyle);
        HWND parent_window = NW_NULL;
        if (viewport->ParentViewportId != 0) {
            if (ImGuiViewport* parent_viewport = NW_GUI::FindViewportByID(viewport->ParentViewportId)) {
                parent_window = (HWND)parent_viewport->PlatformHandle;
            }
        }
        // create window
        if constexpr (NW_TRUTH) {
            RECT rect = { (LONG)viewport->Pos.x, (LONG)viewport->Pos.y, (LONG)(viewport->Pos.x + viewport->Size.x), (LONG)(viewport->Pos.y + viewport->Size.y) };
            ::AdjustWindowRectEx(&rect, data->DwStyle, NW_FALSE, data->DwExStyle);
            data->Hwnd = ::CreateWindowEx(
                data->DwExStyle,            // extended style
                s_wnd_class.lpszClassName,  // class name
                _T("imgui_viewport"),       // window name
                data->DwStyle,              // style
                rect.left, rect.top,        // coord
                rect.right - rect.left,     // width
                rect.bottom - rect.top,     // height
                parent_window,              // parrent
                NW_NULL,                       // menu
                s_wnd_class.hInstance,      // instance handle
                NW_NULL                        // param
            );
            if (data->Hwnd == NW_NULL) { throw init_error(__FILE__, __LINE__); return; }
            data->HwndOwned = NW_TRUTH;
            viewport->PlatformRequestResize = NW_FALSE;
            viewport->PlatformHandle = viewport->PlatformHandleRaw = data->Hwnd;
        }
    }
    v1nil gui_wapi_wnd_delete(ImGuiViewport* viewport)
    {
        if (win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData) {
            if (GetCapture() == data->Hwnd) {
                // transfer capture so if we started 
                // dragging from a window that later disappears, we'll still receive the MOUSEUP event.
                ::ReleaseCapture();
                ::SetCapture(s_wnd);
            }
            if (data->Hwnd && data->HwndOwned) { DestroyWindow(data->Hwnd); }
            data->Hwnd = NW_NULL;
            IM_DELETE(data);
        }
        viewport->PlatformUserData = viewport->PlatformHandle = NW_NULL;
    }
    v1nil gui_wapi_wnd_show(ImGuiViewport* viewport)
    {
        win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData;
        IM_ASSERT(data->Hwnd != NW_NULL);
        if (viewport->Flags & ImGuiViewportFlags_NoFocusOnAppearing) { ::ShowWindow(data->Hwnd, SW_SHOWNA); }
        else { ::ShowWindow(data->Hwnd, SW_SHOW); }
    }
    v1nil gui_wapi_wnd_update(ImGuiViewport* viewport)
    {
        // (optional) update win32 style if it changed _after_ creation
        // generally they won't change unless configuration flags are changed;
        // but advanced uses (such as manually rewriting viewport flags) make this useful
        win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData;
        IM_ASSERT(data->Hwnd != NW_NULL);
        DWORD new_style = 0u;
        DWORD new_ex_style = 0u;
        gui_wapi_get_viewport_flags_imstyle(viewport->Flags, &new_style, &new_ex_style);
        // only reapply the flags that have been changed from our point of view (as other flags are being modified by windows)
        if (data->DwStyle != new_style || data->DwExStyle != new_ex_style) {
            data->DwStyle = new_style;
            data->DwExStyle = new_ex_style;
            ::SetWindowLong(data->Hwnd, GWL_STYLE, data->DwStyle);
            ::SetWindowLong(data->Hwnd, GWL_EXSTYLE, data->DwExStyle);
            RECT rect = { (LONG)viewport->Pos.x, (LONG)viewport->Pos.y, (LONG)(viewport->Pos.x + viewport->Size.x), (LONG)(viewport->Pos.y + viewport->Size.y) };
            ::AdjustWindowRectEx(&rect, data->DwStyle, NW_FALSE, data->DwExStyle); // client to screen
            ::SetWindowPos(data->Hwnd, NW_NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
            ::ShowWindow(data->Hwnd, SW_SHOWNA); // this is necessary when we alter the style
            viewport->PlatformRequestMove = viewport->PlatformRequestResize = NW_TRUTH;
        }
    }
    // --wnd_graphics
    v1nil gui_wapi_wnd_render(ImGuiViewport* viewport, ptr_t draw_arg)
    {
        win_viewport_data* data = reinterpret_cast<win_viewport_data*>(viewport->RendererUserData);
        ::UpdateWindow(data->Hwnd);
    }
    v1nil gui_wapi_wnd_swap(ImGuiViewport* viewport, ptr_t draw_arg)
    {
        win_viewport_data* data = reinterpret_cast<win_viewport_data*>(viewport->RendererUserData);
        ::SwapBuffers(::GetWindowDC(data->Hwnd));
    }
    // --wnd_getters
    ImVec2 gui_wapi_wnd_get_coord(ImGuiViewport* viewport)
    {
        win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData;
        IM_ASSERT(data->Hwnd != NW_NULL);
        POINT pos = { 0, 0 };
        ::ClientToScreen(data->Hwnd, &pos);
        return ImVec2((float)pos.x, (float)pos.y);
    }
    ImVec2 gui_wapi_wnd_get_size(ImGuiViewport* viewport)
    {
        win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData;
        IM_ASSERT(data->Hwnd != NW_NULL);
        RECT rect{ 0u };
        ::GetClientRect(data->Hwnd, &rect);
        return ImVec2(float(rect.right - rect.left), float(rect.bottom - rect.top));
    }
    // --wnd_setters
    v1nil gui_wapi_wnd_set_coord(ImGuiViewport* viewport, ImVec2 pos)
    {
        win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData;
        IM_ASSERT(data->Hwnd != NW_NULL);
        RECT rect = { (LONG)pos.x, (LONG)pos.y, (LONG)pos.x, (LONG)pos.y };
        ::AdjustWindowRectEx(&rect, data->DwStyle, NW_FALSE, data->DwExStyle);
        ::SetWindowPos(data->Hwnd, NW_NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    v1nil gui_wapi_wnd_set_size(ImGuiViewport* viewport, ImVec2 size)
    {
        win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData;
        IM_ASSERT(data->Hwnd != NW_NULL);
        RECT rect { 0, 0, (LONG)size.x, (LONG)size.y };
        ::AdjustWindowRectEx(&rect, data->DwStyle, NW_FALSE, data->DwExStyle); // client to Screen
        ::SetWindowPos(
            data->Hwnd, NW_NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
            SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE
        );
    }
    v1nil gui_wapi_wnd_set_title(ImGuiViewport* viewport, cstr_t title)
    {
        // SetWindowTextA() doesn't properly handle utf-8 so we explicitely convert our string;
        win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData;
        IM_ASSERT(data->Hwnd != NW_NULL);
        v1s n = ::MultiByteToWideChar(CP_UTF8, 0, title, -1, NW_NULL, 0);
        ImVector<wchar_t> title_w;
        title_w.resize(n);
        ::MultiByteToWideChar(CP_UTF8, 0, title, -1, title_w.Data, n);
        ::SetWindowTextW(data->Hwnd, title_w.Data);
    }
    v1nil gui_wapi_wnd_set_alpha(ImGuiViewport* viewport, v1f alpha)
    {
        win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData;
        IM_ASSERT(data->Hwnd != NW_NULL);
        IM_ASSERT(alpha >= 0.0f && alpha <= 1.0f);
        if (alpha < 1.0f) {
            DWORD style = ::GetWindowLongW(data->Hwnd, GWL_EXSTYLE) | WS_EX_LAYERED;
            ::SetWindowLongW(data->Hwnd, GWL_EXSTYLE, style);
            ::SetLayeredWindowAttributes(data->Hwnd, 0, (BYTE)(255 * alpha), LWA_ALPHA);
        }
        else {  // get rid of transparency
            DWORD style = ::GetWindowLongW(data->Hwnd, GWL_EXSTYLE) & ~WS_EX_LAYERED;
            ::SetWindowLongW(data->Hwnd, GWL_EXSTYLE, style);
        }
    }
    v1nil gui_wapi_wnd_set_focused(ImGuiViewport* viewport)
    {
        win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData;
        IM_ASSERT(data->Hwnd != NW_NULL);
        ::BringWindowToTop(data->Hwnd);
        ::SetForegroundWindow(data->Hwnd);
        ::SetFocus(data->Hwnd);
    }
    // --wnd_predicates
    v1bit gui_wapi_wnd_is_focused(ImGuiViewport* viewport)
    {
        win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData;
        IM_ASSERT(data->Hwnd != NW_NULL);
        return ::GetForegroundWindow() == data->Hwnd;
    }
    v1bit gui_wapi_wnd_is_minimized(ImGuiViewport* viewport)
    {
        win_viewport_data* data = (win_viewport_data*)viewport->PlatformUserData;
        IM_ASSERT(data->Hwnd != NW_NULL);
        return ::IsIconic(data->Hwnd) != 0;
    }
}
namespace NW
{
    // win32 message handler (process Win32 mouse/keyboard inputs, etc.)
    // call from your application's message handler.
    // when implementing your own back-end, you can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if Dear ImGui wants to use your inputs;
    // --when io.WantCaptureMouse is NW_TRUTH, do not dispatch mouse input data to your main application;
    // --when io.WantCaptureKeyboard is NW_TRUTH, do not dispatch keyboard input data to your main application;
    // generally you may always pass all inputs to Dear ImGui, and hide them from your application based on those two flags;
    // ps: in this win32 handler, we use the capture api (GetCapture/SetCapture/ReleaseCapture) to be able to read mouse coordinates when dragging mouse outside of our window bounds.
    // ps: we treat DBLCLK messages as regular mouse down messages, so this code will work on windows classes that have the CS_DBLCLKS flag set. our own example app code doesn't set this flag.
    NW_API LRESULT WINAPI gui_wapi_event_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (NW_GUI::GetCurrentContext() == NW_NULL) { return 0; }
        switch (msg) {
        case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK: {
            v1s button = 0;
            if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK) { button = 0; }
            if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK) { button = 1; }
            if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK) { button = 2; }
            if (!NW_GUI::IsAnyMouseDown() && ::GetCapture() == NW_NULL) { ::SetCapture(hwnd); }
            s_imlib_io->MouseDown[button] = NW_TRUTH;
            return 0;
        }
        case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP: {
            v1s button = 0;
            if (msg == WM_LBUTTONUP) { button = 0; }
            if (msg == WM_RBUTTONUP) { button = 1; }
            if (msg == WM_MBUTTONUP) { button = 2; }
            s_imlib_io->MouseDown[button] = NW_FALSE;
            if (!NW_GUI::IsAnyMouseDown() && ::GetCapture() == hwnd) { ::ReleaseCapture(); }
            return 0;
        }
        case WM_MOUSEWHEEL:
            s_imlib_io->MouseWheel += static_cast<v1f>(GET_WHEEL_DELTA_WPARAM(wparam)) / static_cast<v1f>(WHEEL_DELTA);
            return 0;
        case WM_MOUSEHWHEEL:
            s_imlib_io->MouseWheelH += static_cast<v1f>(GET_WHEEL_DELTA_WPARAM(wparam)) / static_cast<v1f>(WHEEL_DELTA);
            return 0;
        case WM_KEYDOWN: case WM_SYSKEYDOWN:
            if (wparam < 256) { s_imlib_io->KeysDown[wparam] = NW_TRUTH; }
            return 0;
        case WM_KEYUP: case WM_SYSKEYUP:
            if (wparam < 256) { s_imlib_io->KeysDown[wparam] = NW_FALSE; }
            return 0;
        case WM_CHAR:
            // you can also use ToAscii()+GetKeyboardState() to retrieve characters.
            if (wparam > 0 && wparam < 0x10000) { s_imlib_io->AddInputCharacterUTF16(static_cast<wchar_t>(wparam)); }
            return 0;
        case WM_SETCURSOR:
            if (LOWORD(lparam) == HTCLIENT && gui_wapi_update_mouse_cursor()) { return 1; }
            return 0;
        case WM_DISPLAYCHANGE: s_update_monitors = NW_TRUTH;
            return 0;
        }
        return 0;
    }
}
#   endif   // WAPI_WIN
#endif  // NW_WAPI