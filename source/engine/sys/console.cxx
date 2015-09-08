#include "blood/sys/console.hxx"
#include <iostream>
#include <stdlib.h>

namespace blood { namespace sys {

void Console::executeByShell(const char* _path_to_file, const char* _arg_list)
{
#if defined(ENGINE_WIN)
    ConsoleWin::executeByShell(_path_to_file, _arg_list);
#else
    // NIX
#endif
}

void Console::executeByProcess(const char* _path_to_file, const char* _arg_list)
{
#if defined(ENGINE_WIN)
    ConsoleWin::executeByProcess(_path_to_file, _arg_list);
#else
    //ConsoleNix::execute();
#endif
}

void Console::execute(const char* _path_to_file, const char* _arg_list)
{
#if defined(ENGINE_WIN)
    ConsoleWin::execute(_path_to_file, _arg_list);
#else

#endif
}

void Console::command(const char* _command) /* static */
{
    system(_command);
}

void Console::clearConsole() /* static */
{
#if defined(ENGINE_WIN)
    system("cls");
#else
    system("clear");
#endif
}

}} // end blood::sys