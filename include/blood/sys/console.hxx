#pragma once

#include "blood/sys/console_win.hxx"
#include "blood/sys/console_nix.hxx"

namespace blood { namespace sys { 

class Console : public ConsoleWin, public ConsoleNix
{
public:
    static void executeByShell(const char* _path_to_file, const char* _arg_list);
    static void execute(const char* _path_to_file, const char* _arg_list);
    static void executeByProcess(const char* _path_to_file, const char* _arg_list);
    static void command(const char* _command);
    static void clearConsole();
};

}} // end blood::sys

//declareSynonymFor(Console);