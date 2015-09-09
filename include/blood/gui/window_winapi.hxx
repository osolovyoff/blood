#pragma once
#if defined(BLOOD_VS) || defined(BLOOD_32)

#include <windows.h>
#include <memory>

namespace blood { namespace sys {

class WindowWinApi
{
    typedef LRESULT CALLBACK callback_wnd_procedure(HWND, UINT, WPARAM, LPARAM);
public:
    WindowWinApi();
    ~WindowWinApi();
    
    void initialize(const char* title, int w, int h);
    void release();

    void show();
    void setFullscreen(bool fullscreen = true);
    bool isCreated() const;

    void set_procedure(callback_wnd_procedure* fn);

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
#endif