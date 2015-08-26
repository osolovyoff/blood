#pragma once
#ifndef functions_h__
#define functions_h__

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#if defined(BLOOD_32) || defined(BLOOD_VS)
#include <Windows.h>
#include <conio.h>
#endif
#if defined(BLOOD_UNIX)
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#endif

namespace blood {
namespace fn
    {
        inline std::vector<std::string> splitString(const std::string& _string, const char _separate_symbols);
        inline std::vector<std::string>& splitString(const std::string& _string, const char _separate_symbols, std::vector<std::string>& _elements);
        inline std::string replaseAll(std::string _str, const std::string& _from, const std::string& _to);
        inline std::wstring convertFromUTF8(const std::string& _string);
        inline std::wstring localeFromUTF8(const std::string& _utf8_string);
        inline char getch();
        inline bool isNumber(const std::string& s);
    }
}


namespace blood
{
    namespace fn
    {
        //_____________________________________________________________________________________________
        inline std::vector<std::string>
            splitString( const std::string& _s
                       , const char _separate_symbols)
        {
            std::vector<std::string> elements;
            std::stringstream ss(_s);
            std::string item;
            while (std::getline(ss, item, _separate_symbols)) {
                elements.push_back(item);
            }
            return elements;
        }
    //_____________________________________________________________________________________________
    inline std::vector<std::string>&
        splitString( const std::string& _s
                   , const char _separate_symbols
                   , std::vector<std::string>& _elements)
    {
            std::stringstream ss(_s);
            std::string item;
            while (std::getline(ss, item, _separate_symbols)) {
                _elements.push_back(item);
            }
            return _elements;
    }
    //_____________________________________________________________________________________________
    inline std::string
        replaseAll(std::string _str
        , const std::string& _from
        , const std::string& _to)
    {
        size_t start_pos = 0;
        while ((start_pos = _str.find(_from, start_pos)) != std::string::npos) {
            _str.replace(start_pos, _from.length(), _to);
            start_pos += _to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return _str;
    }
    //_____________________________________________________________________________________________
    inline std::wstring convertFromUTF8(const std::string& _string)
    {
        return std::wstring(_string.begin(), _string.end());
    }
    //_____________________________________________________________________________________________
    inline std::wstring localeFromUTF8(const std::string& _utf8_string)
    {
        std::wstring utf16_buf;
    #if defined(BLOOD_32)
        const int len = MultiByteToWideChar(CP_UTF8, 0, _utf8_string.c_str(), -1, NULL, 0);
        if (len)
        {
            utf16_buf.resize(len);
            MultiByteToWideChar(CP_UTF8, 0, _utf8_string.c_str(), -1, &utf16_buf[0], (int)utf16_buf.size());
            /*std::vector<wchar_t> utf16buf(len);
            MultiByteToWideChar(CP_UTF8, 0, _utf8_string.c_str(), -1, &utf16buf[0], utf16buf.size());*/

        }
    #else
    #endif
            return utf16_buf;
    }

    inline char getch()
    {
        #if defined(BLOOD_UNIX)
        struct termios oldattr, newattr;
        int ch;
        tcgetattr( STDIN_FILENO, &oldattr );
        newattr = oldattr;
        newattr.c_lflag &= ~( ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
        ch = getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
        return ch;
        #endif // defined
        #if defined(BLOOD_32) || defined(BLOOD_VS)
        return _getch();
        #endif // defined
    }
    //_____________________________________________________________________________________________
    inline bool isNumber(const std::string& s)
    {
        return !s.empty() && std::find_if(s.begin(),
            s.end(), [](char c) { return !isdigit(c); }) == s.end();
    }
    //_____________________________________________________________________________________________
    } // end fn
} // end blood

#endif // functions_h__
