#pragma once
#ifndef console_h__
#define console_h__

#include <Windows.h>
#include "common.hpp"

class Console
{
public:
    Console();
    virtual ~Console();

    void hideConsole();
    void showConsole();
    void switchHideShowConsole();

    HWND getConsoleHandle() const;

    static void command(const char* _command);
    static void clearConsole();

private:
    HWND m_console_handle;
    bool m_is_hide;
};

declareSynonymFor(Console);

#endif // console_h__