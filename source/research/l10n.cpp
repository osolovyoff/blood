#include "l10n.hpp"

Localization::Localization()
{
    
    
}

WString Localization::convertKeycodeToKeyname(const ULong _keycode)
{
    auto i = m_russian.find(_keycode);
    if (i != m_russian.end())
        return i->second;
    else
        return L" unknown key ";
}

WString Localization::convertKeycodeToKeynameForKeydownSituation(const ULong _keycode)
{
    return convertKeycodeToKeyname(_keycode);
}

WString Localization::convertKeycodeToKeynameForKeyupSituation(const ULong _keycode)
{
    for (auto& i : m_allowed_keyup_list)
    {
        if (_keycode == i)
        return convertKeycodeToKeyname(_keycode);
    }
    return L"";
}

const std::list< DWORD > Localization::m_allowed_keyup_list = 
{
    { VK_ESCAPE },
    { VK_LMENU },
    { VK_RMENU },
};

const std::map< ULong, WString> Localization::m_english =
{
// 1 row
    { VK_ESCAPE,  L" ESC " }, // ESC
    { VK_F1,      L" F1 "  }, // F1
    { VK_F2,      L" F2 "  }, // F2
    { VK_F3,      L" F3 "  }, // F3
    { VK_F4,      L" F4 "  }, // F4
    { VK_F5,      L" F5 "  }, // F5
    { VK_F6,      L" F6 "  }, // F6
    { VK_F7,      L" F7 "  }, // F7
    { VK_F8,      L" F8 "  }, // F8
    { VK_F9,      L" F9 "  }, // F9
    { VK_F10,     L" F10 " }, // F10
    { VK_F11,     L" F11 " }, // F11
    { VK_F12,     L" F12 " }, // F12
// 2 row
    { 192, L" ~ " }, // ~       
    { 49,  L" 1 " }, // 1
    { 50,  L" 2 " }, // 2
    { 51,  L" 3 " }, // 3
    { 52,  L" 4 " }, // 4
    { 53,  L" 5 " }, // 5 
    { 54,  L" 6 " }, // 6
    { 55,  L" 7 " }, // 7
    { 56,  L" 8 " }, // 8
    { 57,  L" 9 " }, // 9
    { 48,  L" 0 " }, // 0
    { 189, L" - " }, // -
    { 187, L" + " }, // +
// main control
    { VK_LCONTROL, L" LEFTCTRL" }, // left control
    { VK_RCONTROL, L" RIGHTCTRL " }, // right control
    { VK_SNAPSHOT, L" PRINT " }, // snapshot
    { VK_CAPITAL,  L" CAPS " }, // caps
    { VK_LSHIFT,   L" SHIFT " }, // left shift
    { VK_RSHIFT,   L" SHIFT " }, // right shift
    { VK_SCROLL,   L" ScrLk " }, // ScrLk
    { VK_PAUSE,    L" PAUSE " }, // pause
    { VK_LMENU,    L" LEFTALT" }, // left alt
    { VK_RMENU,    L" RIGHTALT " }, // right alt
    { VK_SPACE,    L" SPACE " }, // space
    { VK_BACK,     L" BACK " }, // back
    { VK_TAB,      L" TAB " }, // tab
    { VK_END,      L" ENTER " }, // enter
// text_editor
    { 45, L" INSERT " },
    { 36, L" HOME " },
    { 33, L" PgUP " },
    { 46, L" DELETE " },
    { 35, L" END " },
    { 34, L" PgDn " },
// arrows
    { VK_UP,    L" up "    },
    { VK_LEFT,  L" left "  },
    { VK_DOWN,  L" down "  },
    { VK_RIGHT, L" right " },
// num
    { 144, L" NumLock " },
    { 96, L" num_0 " },
    { 97, L" num_1 " },
    { 98, L" num_2 " },
    { 99, L" num_3 " },
    { 100, L" num_4 " },
    { 101, L" num_5 " },
    { 102, L" num_6 " },
    { 103, L" num_7 " },
    { 104, L" num_8 " },
    { 105, L" num_9 " },
    { 107, L" num_+ " },
    { 110, L" num_. " },
    { 111, L" num_/ " },
    { 106, L" num_* " },
    { 109, L" num_- " },
// symbols
    { 65, L"a" },
    { 66, L"b" },
    { 67, L"c" },
    { 68, L"d" },
    { 69, L"e" },
    { 70, L"f" },
    { 71, L"g" },
    { 72, L"h" },
    { 73, L"i" },
    { 74, L"j" },
    { 75, L"k" },
    { 76, L"l" },
    { 77, L"m" },
    { 78, L"n" },
    { 79, L"o" },
    { 80, L"p" },
    { 81, L"q" },
    { 82, L"r" },
    { 83, L"s" },
    { 84, L"t" },
    { 85, L"u" },
    { 86, L"v" },
    { 87, L"w" },
    { 88, L"x" },
    { 89, L"y" },
    { 90, L"z" },
// other symbols
    { 186, L";" },
    { 222, L"'" },
    { 188, L"," },
    { 190, L"." },
    { 191, L"/" },
    { 219, L" [ " },
    { 221, L" ] " },
    { 220, L" \\ " },
};

const std::map< ULong, WString> Localization::m_russian =
{
    // 1 row
    { VK_ESCAPE, L" ESC " }, // ESC
    { VK_F1, L" F1 " }, // F1
    { VK_F2, L" F2 " }, // F2
    { VK_F3, L" F3 " }, // F3
    { VK_F4, L" F4 " }, // F4
    { VK_F5, L" F5 " }, // F5
    { VK_F6, L" F6 " }, // F6
    { VK_F7, L" F7 " }, // F7
    { VK_F8, L" F8 " }, // F8
    { VK_F9, L" F9 " }, // F9
    { VK_F10, L" F10 " }, // F10
    { VK_F11, L" F11 " }, // F11
    { VK_F12, L" F12 " }, // F12
    // 2 row
    { 192, L" ~ " }, // ~       
    { 49, L" 1 " }, // 1
    { 50, L" 2 " }, // 2
    { 51, L" 3 " }, // 3
    { 52, L" 4 " }, // 4
    { 53, L" 5 " }, // 5 
    { 54, L" 6 " }, // 6
    { 55, L" 7 " }, // 7
    { 56, L" 8 " }, // 8
    { 57, L" 9 " }, // 9
    { 48, L" 0 " }, // 0
    { 189, L" - " }, // -
    { 187, L" + " }, // +
    // main control
    { VK_LCONTROL, L" LEFTCTRL" }, // left control
    { VK_RCONTROL, L" RIGHTCTRL " }, // right control
    { VK_SNAPSHOT, L" PRINT " }, // snapshot
    { VK_CAPITAL, L" CAPS " }, // caps
    { VK_LSHIFT, L" SHIFT " }, // left shift
    { VK_RSHIFT, L" SHIFT " }, // right shift
    { VK_SCROLL, L" ScrLk " }, // ScrLk
    { VK_PAUSE, L" PAUSE " }, // pause
    { VK_LMENU, L" LEFTALT" }, // left alt
    { VK_RETURN, L" RETURN " }, // ENTER
    { VK_RMENU, L" RIGHTALT " }, // right alt
    { VK_SPACE, L" SPACE " }, // space
    { VK_BACK, L" BACK " }, // back
    { VK_TAB, L" TAB " }, // tab
    { VK_END, L" END " }, // enter
    // text_editor
    { 45, L" INSERT " },
    { 36, L" HOME " },
    { 33, L" PgUP " },
    { 46, L" DELETE " },
    { 35, L" END " },
    { 34, L" PgDn " },
    // arrows
    { VK_UP, L" up " },
    { VK_LEFT, L" left " },
    { VK_DOWN, L" down " },
    { VK_RIGHT, L" right " },
    // num
    { 144, L" NumLock " },
    { 96, L" num_0 " },
    { 97, L" num_1 " },
    { 98, L" num_2 " },
    { 99, L" num_3 " },
    { 100, L" num_4 " },
    { 101, L" num_5 " },
    { 102, L" num_6 " },
    { 103, L" num_7 " },
    { 104, L" num_8 " },
    { 105, L" num_9 " },
    { 107, L" num_+ " },
    { 110, L" num_. " },
    { 111, L" num_/ " },
    { 106, L" num_* " },
    { 109, L" num_- " },
    // symbols
    { 65, L"ô" },
    { 66, L"è" },
    { 67, L"ñ" },
    { 68, L"â" },
    { 69, L"ó" },
    { 70, L"à" },
    { 71, L"ï" },
    { 72, L"ð" },
    { 73, L"ø" },
    { 74, L"î" },
    { 75, L"ë" },
    { 76, L"ä" },
    { 77, L"ü" },
    { 78, L"ò" },
    { 79, L"ù" },
    { 80, L"ç" },
    { 81, L"é" },
    { 82, L"ê" },
    { 83, L"û" },
    { 84, L"å" },
    { 85, L"ã" },
    { 86, L"ì" },
    { 87, L"ö" },
    { 88, L"÷" },
    { 89, L"í" },
    { 90, L"ÿ" },
    { 188, L"á" },
    { 190, L"þ" },
    { 186, L"æ" },
    { 222, L"ý" },
    { 219, L"õ" },
    { 221, L"ú" },
    // other symbols
    { 191, L"/" },
    { 220, L" \\ " },
};

Bool Localization::isReturn(const ULong& _keycode)
{
    return (_keycode == VK_RETURN);
}

std::string Localization::to_utf8(const wchar_t* buffer, int len)
{
    int nChars = ::WideCharToMultiByte(
        CP_UTF8,
        0,
        buffer,
        len,
        NULL,
        0,
        NULL,
        NULL);
    if (nChars == 0) return "";

    String newbuffer;
    newbuffer.resize(nChars);
    ::WideCharToMultiByte(
        CP_UTF8,
        0,
        buffer,
        len,
        const_cast<char*>(newbuffer.c_str()),
        nChars,
        NULL,
        NULL);

    return newbuffer;
}

std::string Localization::to_utf8(const std::wstring& str)
{
    return to_utf8(str.c_str(), (int)str.size());
}

WString Localization::convertStrintToWString(const String& _src)
{
    return WString(_src.begin(), _src.end());
}


