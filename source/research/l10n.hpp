#ifndef l10n_h__
#define l10n_h__

#include <windows.h>
#include <memory>
#include <list>
#include <map>

#include "common.hpp"

class Localization
{
public:
    Localization();
    WString convertKeycodeToKeynameForKeydownSituation(const ULong _keycode);
    WString convertKeycodeToKeynameForKeyupSituation(const ULong _keycode);
    Bool    isReturn(const ULong& _keycode);

    static WString convertStrintToWString(const String& _src);

private:
    WString convertKeycodeToKeyname(const ULong _keycode);

    static String to_utf8(const wchar_t* buffer, int len);
    static String to_utf8(const WString& _str);

private:
    static const std::map< ULong, WString> m_english;
    static const std::map< ULong, WString> m_russian;
    static const std::list< ULong > m_allowed_keyup_list;
};

#endif // l10n_h__