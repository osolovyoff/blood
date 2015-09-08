#if defined(BLOOD_32) || defined(BLOOD_VS)
#include "../../../include/blood/sys/console_win.hxx"

#pragma comment(lib, "Shell32.lib")
#include <iostream>
#include <Windows.h>

ConsoleWin::ConsoleWin()
{
    m_console_handle = GetConsoleWindow();
}

void ConsoleWin::hideConsole()
{
    ShowWindow(m_console_handle, SW_HIDE);
}

void ConsoleWin::showConsole()
{
    ShowWindow(m_console_handle, SW_SHOW);
}

void ConsoleWin::executeByProcess(const char* _path_to_file, const char* _arg_list)
{
    //execl(_path_to_file, _arg_list);
    WinExec(_path_to_file, 1);
}

void ConsoleWin::executeByShell(const char* _path_to_file, const char* _arg_list)
{
    ShellExecuteA( NULL
                 , "open"
                 , _path_to_file
                 , NULL, NULL
                 , SW_SHOWNORMAL);
}

void ConsoleWin::execute(const char* _path_to_file, const char* _arg_list)
{
    STARTUPINFOA         startup_info;
    PROCESS_INFORMATION  process_info;

    memset(&startup_info, 0, sizeof(startup_info));
    memset(&process_info, 0, sizeof(process_info));

    startup_info.cb = sizeof(startup_info);

    auto done =
        CreateProcessA(_path_to_file
        , NULL, NULL
        , NULL
        , FALSE, 0
        , NULL, NULL
        , &startup_info
        , &process_info);

    if (!done)
        std::cerr << "Uh-Oh! CreateProcess() failed to start program \"" << _path_to_file << "\"\n";
}
#endif
