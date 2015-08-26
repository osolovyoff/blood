#include "fn/encoding.hxx"
#include <sstream>
#include <algorithm>
#include <windows.h>

std::string blood::fn::text::convert_unicode_to_multy(std::wstring in)
{
        
}

std::wstring blood::fn::text::convert_multy_to_unicode(std::string in)
{
    std::wstring buffer;
    const int len = MultiByteToWideChar(CP_UTF8, 0, in.c_str(), -1, NULL, 0);
    if (len)
    {
        buffer.resize(len);
        MultiByteToWideChar(CP_UTF8, 0, in.c_str(), -1, &buffer[0], (int)buffer.size());
    }
    return buffer;
}

std::wstring blood::fn::text::convert_string_to_unicode(std::string in)
{
    std::wstring(in.begin(), in.end());
}

std::vector<std::string> blood::fn::text::split_string(std::string in, const char symbol)
{
    std::vector<std::string> elements;
    std::stringstream ss(symbol);
    std::string item;
    while (std::getline(ss, item, symbol))
        elements.push_back(item);
    return elements;
}

std::vector<std::wstring> blood::fn::text::split_wstring(std::wstring in, const wchar_t symbol)
{
    std::vector<std::wstring> elements;
    std::wstringstream ss(symbol);
    std::wstring item;
    while (std::getline(ss, item, symbol))
        elements.push_back(item);
    return elements;
}

std::string blood::fn::text::replace(std::string& in, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while ((start_pos = in.find(from, start_pos)) != std::string::npos) {
        in.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return in;
}

std::wstring blood::fn::text::replace(std::wstring& in, const std::wstring& from, const std::wstring& to)
{
    size_t start_pos = 0;
    while ((start_pos = in.find(from, start_pos)) != std::wstring::npos) {
        in.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return in;
}

bool blood::fn::text::is_number(std::string in)
{
    return !in.empty() && std::find_if(in.begin(),
        in.end(), [](char c) { return !isdigit(c); }) == in.end();
}