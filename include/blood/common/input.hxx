#pragma once
#include <string>

namespace blood { namespace fn { namespace input {

    static std::string get_string();
    static int         get_menu_number();
    static bool        get_yes_no();
    static int         get_number();
    static int         convert_symbol_to_number(char _symbol);
    static void        press_any_keys();
    static char        getch();

}}} // blood::fn::input
