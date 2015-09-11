#include "app.hxx"
#include "window.hxx"

#include <windows.h>
#include <winuser.h>

LRESULT CALLBACK window_processing(HWND, UINT, WPARAM, LPARAM);

void App::start()
{
    m_window.reset(new blood::gui::WindowWinApi);
    m_device.reset(new DXDevice);

    m_window->set_procedure(window_processing);
    m_window->initialize("informer", 800, 600);
    m_device->initialize(m_window->get_hwnd(), 800, 600, false);
    m_window->show();
    MSG msg = { 0 };
    while (m_window->is_created())
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_device->render();
        }
    }
}

LRESULT CALLBACK window_processing(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        App::get_instance().m_window->release();
    default:
        return DefWindowProc(hwnd, msg, wparam, lparam);
        break;
    }
    return 0;
}