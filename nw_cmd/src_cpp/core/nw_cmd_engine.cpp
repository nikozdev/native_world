#include "nw_cmd_pch.hpp"
#include "nw_cmd_engine.h"
#if (defined NW_WAPI)
#define NW_CMD_INPUT_FLAGS ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT

namespace NW
{
    cmd_engine::cmd_engine() :
        m_fmbuf(*this),
        m_wnd_info(cmd_window_info()),
        m_pxl_info(cmd_pixel_info()),
        m_cout(nullptr), m_cin(nullptr)
    {
        this->set_wnd_size(120, 40);
        this->set_pxl_size(8, 16);
        m_fmbuf.set_size(get_wnd_size_x(), get_wnd_size_y());
        m_cout = GetStdHandle(STD_OUTPUT_HANDLE);
        m_cin = GetStdHandle(STD_INPUT_HANDLE);
        m_wnd_info.title = "console_window";
    }
    cmd_engine::~cmd_engine()
    {
        quit();
    }
    // --setters
    void cmd_engine::stop_running() { m_is_running = false; }
    void cmd_engine::set_title(cstr_t title) { m_wnd_info.title = title; ::SetConsoleTitleA(&title[0]); }
    void cmd_engine::set_wnd_size(v1u16 uwidth, v1u16 uheight) {
        m_wnd_info.wnd_rect.Left = m_wnd_info.wnd_rect.Top = 0;
        m_wnd_info.wnd_rect.Right = uwidth; m_wnd_info.wnd_rect.Bottom = uheight;
    }
    void cmd_engine::set_pxl_size(v1u16 width, v1u16 height){
        m_pxl_info.dwFontSize.X = width;
        m_pxl_info.dwFontSize.Y = height;
        m_pxl_info.FontFamily = FF_DONTCARE;
        m_pxl_info.FontWeight = FW_NORMAL;
        m_pxl_info.nFont = 0;
        swprintf_s(m_pxl_info.FaceName, L"Consolas");
        m_pxl_info.cbSize = sizeof(m_pxl_info);
    }

    void cmd_engine::set_wnd_info(const cmd_window_info& info) { m_wnd_info = info; ::SetConsoleWindowInfo(m_fmbuf.get_handle(), TRUE, &m_wnd_info.wnd_rect); }
    void cmd_engine::set_pxl_info(const cmd_pixel_info& rcpxInfo) { m_pxl_info = rcpxInfo; ::SetCurrentConsoleFontEx(m_fmbuf.get_handle(), TRUE, &m_pxl_info); }
    void cmd_engine::set_crs_info(const cmd_cursor_info& rcrsInfo) { m_cursor_info = rcrsInfo; ::SetConsoleCursorInfo(m_fmbuf.get_handle(), &m_cursor_info); }
    
    // --==<core_methods>==--
    void cmd_engine::run() {
        auto run_loop = [this]()->void {
            if (!init()) { return; }
            while (m_is_running) { update(); }
            quit();
        };
        m_thrRun = thread(run_loop);
    }
    bool cmd_engine::init()
    {
        if (m_is_running) { return false; }
        else { m_is_running = true; }

        set_title(&m_wnd_info.title[0]);
        if (!::SetConsoleMode(m_cin, NW_CMD_INPUT_FLAGS)) { quit(); return false; }
     
        rect min_rect = { 0, 0, 1, 1 };
        if (!::SetConsoleWindowInfo(m_fmbuf.get_handle(), TRUE, &min_rect)) { quit(); return false; }
        if (!::SetCurrentConsoleFontEx(m_fmbuf.get_handle(), TRUE, &m_pxl_info)) { quit(); return false; }

        m_wnd_info.max_size = ::GetLargestConsoleWindowSize(m_cout);
        if (m_wnd_info.get_width() > m_wnd_info.max_size.X) { m_wnd_info.wnd_rect.Left = 0; m_wnd_info.wnd_rect.Right = m_wnd_info.max_size.X; }
        if (m_wnd_info.get_height() > m_wnd_info.max_size.Y) { m_wnd_info.wnd_rect.Top = 0; m_wnd_info.wnd_rect.Bottom = m_wnd_info.max_size.Y; }

        m_fmbuf.remake();
        if (!::SetConsoleWindowInfo(m_fmbuf.get_handle(), TRUE, &m_wnd_info.wnd_rect)) { quit(); return false; }
        if (!::SetConsoleActiveScreenBuffer(m_fmbuf.get_handle())) { quit(); return false; }

        return true;
    }
    void cmd_engine::quit()
    {
        if (!m_is_running) { return; }
        else { m_is_running = false; }

        ::SetConsoleActiveScreenBuffer(m_cout);
    }

    void cmd_engine::update()
    {
        swap_buffers();
        m_fmbuf.clear();
        poll_events();
    }

    void cmd_engine::event_proc(iop_event_t& evt)
    {
        if (evt.has_sort(NW_EVSORT_CURSOR)) {
            auto& evt_cursor = static_cast<iop_event_cursor_t&>(evt);
            m_cursor.event_proc(evt_cursor);
            if (evt_cursor.is_handled()) { return; }
        }
        else if (evt.has_sort(NW_EVSORT_KEYBOD)) {
            auto& evt_keybod = static_cast<iop_event_kbd&>(evt);
            m_kbd.event_proc(evt_keybod);
            if (evt_keybod.is_handled()) { return; }
            if (m_kbd.is_held(NW_KEYCODE_CONTR)) {
                if (m_kbd.is_held(NW_KEYCODE_M)) {
                    m_cursor.set_enabled(!m_cursor.is_enabled());
                }
                else if (m_kbd.is_held(NW_KEYCODE_ESCP)) {
                    stop_running();
                    evt.set_handled(true);
                }
            }
        }
        else if (evt.has_sort(NW_EVSORT_WINDOW)) {
            auto& evt_window = static_cast<iop_event_wnd_t&>(evt);
            if (evt_window.is_handled()) { return; }
            if (evt_window.has_type(NW_EVTYPE_WINDOW_CLOSE)) {
                stop_running();
                evt.set_handled(true);
            }
        }
    }
    // --==</core_methods>==--

    // --==<drawing_methods>==--
    void cmd_engine::draw_line(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_pixel pxl_draw) {
        v1s16 delta_x = x1 - x0;
        v1s16 delta_y = y1 - y0;
        v1s16 dir_x = delta_x < 0 ? -1 : 1;
        v1s16 dir_y = delta_y < 0 ? -1 : 1;
        v1s16 err_val = 0;
        v1s16 delta_err = delta_y;
        for (v1s16 x = x0, y = y0; x != y1; x += dir_x) {
            m_fmbuf.set_pixel(x, y, pxl_draw);
            err_val += delta_err;
            if (err_val > delta_x) {
                y += dir_y;
                err_val -= (delta_x);
            }
        }
    }
    void cmd_engine::draw_rect(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_pixel pxl_draw) {
        v1s16 delta_x = x0 > x1 ? -1 : +1;
        v1s16 delta_y= y0 > y1 ? -1 : +1;
        y1 += delta_y ;
        for (v1s16 iy = y0; iy != y1; iy += delta_y) {
            for (v1s16 ix = x0; ix != x1; ix += delta_x) {
                m_fmbuf.set_pixel(ix, iy, pxl_draw);
            }
        }
    }
    void cmd_engine::draw_bytes(v1s16 x0, v1s16 y0, v1s16 x1, v1s16 y1, cmd_colors color_val, byte_t* str, size_t length) {
        m_fmbuf.set_bytes(x0, y0, x1, y1, color_val, str, length);
    }
    // --==</drawing_methods>==--
    
    // --==<implementation_methods>==--
    inline void cmd_engine::swap_buffers() {
        if (!WriteConsoleOutputW(m_fmbuf.get_handle(), &m_fmbuf.get_pxl_data()[0],
            { m_fmbuf.get_size_x(), m_fmbuf.get_size_y() }, { 0, 0 }, &m_fmbuf.get_info().srWindow)) { stop_running(); return; }
    }
    inline void cmd_engine::poll_events() {
        GetNumberOfConsoleInputEvents(m_cin, &m_evt_info.get_count);
        if (m_evt_info.get_count > 0) {
            ReadConsoleInputA(m_cin, m_evt_info.in_rec, m_evt_info.get_count, &m_evt_info.read_count);
            for (v1u16 evi = 0; evi < m_evt_info.get_count; evi++) {
                v1u evt_type_id = m_evt_info.in_rec[evi].EventType;
                if (evt_type_id == MOUSE_EVENT) {
                    MOUSE_EVENT_RECORD& evt = m_evt_info.in_rec[evi].Event.MouseEvent;
                    if (evt.dwEventFlags == MOUSE_MOVED) {
                        iop_event_cursor_t evt_cursor(NW_EVTYPE_CURSOR_MOVED, evt.dwMousePosition.X, evt.dwMousePosition.Y);
                        event_proc(evt_cursor);
                    }
                    else {
                        for (v1u mi = 0; mi < NW_CURCODE_COUNT; mi++) {
                            auto& btn = m_cursor[static_cast<iop_cursor_t::code_t>(mi)];
                            iop_cursor_t::button_t::state_t old_state = btn.m_state;
                            btn.m_state = (((1 << mi) & evt.dwButtonState) > 0) ? NW_BUTTON_HELD : NW_BUTTON_FREE;
                            if (btn.m_state != old_state) {
                                iop_event_cursor_t evt_cursor(btn.m_state == NW_BUTTON_HELD ?
                                    NW_EVTYPE_CURSOR_PRESS : NW_EVTYPE_CURSOR_RAISE, static_cast<iop_cursor_t::code_t>(mi));
                                event_proc(evt_cursor);
                            }
                        }
                    }
                }
                else if (evt_type_id == KEY_EVENT) {
                    KEY_EVENT_RECORD& evt = m_evt_info.in_rec[evi].Event.KeyEvent;
                    if (evt.bKeyDown) {
                        if (evt.wRepeatCount == 1) {
                            iop_event_kbd evt_keybod(NW_EVTYPE_KEYBOD_PRESS, static_cast<iop_keybod_t::code_t>(evt.wVirtualKeyCode));
                            event_proc(evt_keybod);
                        }
                    }
                    else {
                        iop_event_kbd evt_keybod(NW_EVTYPE_KEYBOD_RAISE, static_cast<iop_keybod_t::code_t>(evt.wVirtualKeyCode));
                        event_proc(evt_keybod);
                        if (evt.uChar.AsciiChar >= ' ' && evt.uChar.AsciiChar <= 'z') {
                            iop_event_kbd evt_keybod(NW_EVTYPE_KEYBOD_CHART, static_cast<iop_keybod_t::code_t>(evt.uChar.AsciiChar));
                            event_proc(evt_keybod);
                        }
                        else {
                        }
                    }
                }
                else if (evt_type_id == FOCUS_EVENT) {
                    iop_event_wnd_t wnd_evt(NW_EVTYPE_WINDOW_FOCUS,m_evt_info.in_rec[evi].Event.FocusEvent.bSetFocus);
                    event_proc(wnd_evt);
                }
                else if (evt_type_id == WINDOW_BUFFER_SIZE_EVENT) {
                    iop_event_wnd_t wnd_evt(
                        NW_EVTYPE_WINDOW_SIZED,
                        m_evt_info.in_rec[evi].Event.WindowBufferSizeEvent.dwSize.X,
                        m_evt_info.in_rec[evi].Event.WindowBufferSizeEvent.dwSize.Y
                    );
                    event_proc(wnd_evt);
                }
                else if (evt_type_id == MENU_EVENT) {
                    MENU_EVENT_RECORD& evt = m_evt_info.in_rec[evi].Event.MenuEvent;
                }
            }
        }
    }
    // --==</implementation_methods>==--
}
#endif	// NW_WAPI