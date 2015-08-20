#include "window.hxx"
#include <exception>

static const int ms_bits = 32;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM wl)
{
    switch (msg)                
    {
    case WM_ACTIVATE:           
    {
        if (!HIWORD(wp))   
        {
            //active = true; 
        }
        else
        {
            //active = false;
        }
    return 0;            
    }
    case WM_SYSCOMMAND:      
    {
        switch (wp)          
        {
        case SC_SCREENSAVE:
        case SC_MONITORPOWER:
            return 0;
        }
        break;
    }
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_KEYDOWN:       
        {
            //keys[wParam] = true;        
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wp, wl);
}


Window::Window()
: m_hwnd(NULL)
, m_instance(NULL)
, m_fullscreen(false)
{}

Window::~Window()
{
    release();
}

//bool Window::isCreated() const
//{
//    return (m_hwnd != NULL);
//}

void Window::initialize(const char* title, int w, int h)
{
    WNDCLASS wc;
    DWORD dwExStyle;
    DWORD dwStyle;
    RECT WindowRect;
    WindowRect.top = (long)0;
    WindowRect.left = (long)0;
    WindowRect.right = (long)w;
    WindowRect.bottom = (long)h;
    m_instance = GetModuleHandle(NULL);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = m_instance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = ms_class_name;

    if (!RegisterClass(&wc))
        throw std::exception("window class not registred");

    if (m_fullscreen)
    {
        DEVMODE dmScreenSettings;
        memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = w;
        dmScreenSettings.dmPelsHeight = h;
        dmScreenSettings.dmBitsPerPel = ms_bits;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            m_fullscreen = false;
        }
    }

    if (m_fullscreen)
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle = WS_POPUP;
        ShowCursor(false);
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle = WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);
    
    m_hwnd = CreateWindowEx
        ( dwExStyle
        , ms_class_name
        , title
        , WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle
        , 0 // position x
        , 0 // position y
        , WindowRect.right - WindowRect.left
        , WindowRect.bottom - WindowRect.top
        , NULL
        , NULL
        , m_instance
        , NULL);

    if (m_hwnd)
    {
        //release();
        //KillGLWindow();
    }
}

void Window::show()
{
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);
}

void Window::release()
{
    if (m_fullscreen)
    {
        ChangeDisplaySettings(NULL, 0);
        ShowCursor(true);
    }

    if (m_hwnd && !DestroyWindow(m_hwnd))
    {
        m_hwnd = NULL;
    }

    if (!UnregisterClass(ms_class_name, m_instance))
    {
        m_instance = NULL;
    }
}

void Window::setFullscreen(bool fullscreen /*= true*/)
{
    m_fullscreen = fullscreen;
}