#include "console.hpp"
#include <locale>

Console::Console()
: m_is_hide(false)
{
    setlocale(LC_ALL, "Russian");
    m_console_handle = GetConsoleWindow();
}

Console::~Console()
{
    m_console_handle = 0;
    m_is_hide = false;
}

void Console::hideConsole()
{
    ShowWindow(m_console_handle, SW_HIDE);
    m_is_hide = true;
}

void Console::showConsole()
{
    ShowWindow(m_console_handle, SW_SHOW);
    m_is_hide = false;
}

void Console::switchHideShowConsole()
{
    m_is_hide = !m_is_hide;
    if (m_is_hide)
        FreeConsole();
    else
        showConsole();
}

HWND Console::getConsoleHandle() const
{
    return m_console_handle;
}

void Console::command(const char* _command)
{
    system(_command);
}

void Console::clearConsole()
{
    system("cls");
}
