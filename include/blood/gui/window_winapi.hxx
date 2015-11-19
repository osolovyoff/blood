#pragma once
#ifndef blood_gui_window_winapi_hxx_
#define blood_gui_window_winapi_hxx_

#if defined(MSVS) || defined(WIN32)

#include <windows.h>
#include <memory>

namespace blood { namespace gui {

class WindowWinApi
{
    typedef LRESULT CALLBACK callback_wnd_procedure(HWND, UINT, WPARAM, LPARAM);
public:
    WindowWinApi();
    ~WindowWinApi();
    
    void initialize(const char* title, int w, int h);
    void release();

    void set_fullscreen(bool fullscreen = true);
    void set_procedure(callback_wnd_procedure* fn);

    void show();

    bool is_created() const;

    int  get_w() const { return m_w; }
    int  get_h() const { return m_h; }
    HWND get_hwnd() const { return m_hwnd; }

private:
    static POINT get_display_center();

private:
    int         m_w;
    int         m_h;
    HWND        m_hwnd;
    HINSTANCE   m_instance;
    bool        m_fullscreen;

    callback_wnd_procedure* m_wnd_procedure;

private:
    const char * const ms_class_name = "blood_window";
};

}} // end blood::system
#endif // USE_MSVS && USE_WIN32
#endif // blood_gui_window_winapi_hxx_