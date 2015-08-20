#pragma once
#if defined(BLOOD_VS) || defined(BLOOD_32)

#include <windows.h>
#include <memory>


class Window
{
public:
    Window();
    ~Window();
    
    void initialize(const char* title, int w, int h);
    void release();

    void show();
    void setFullscreen(bool fullscreen = true);
    bool isCreated() const;

private:
    HWND        m_hwnd;
    HINSTANCE   m_instance;
    bool        m_fullscreen;

private:
    const char * const ms_class_name = "blood_window";
};

#endif