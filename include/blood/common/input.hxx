#pragma once
#include <string>

namespace blood { namespace fn { namespace input {

    std::string get_string();
    int         get_menu_number();
    bool        get_yes_no();
    int         get_number();
    int         convert_symbol_to_number(char _symbol);
    void        press_any_keys();
    char        getch();

}}} // blood::fn::input
