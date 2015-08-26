#include "../../../include/blood/common/encoding.hxx"
#include <sstream>
#include <algorithm>
#include <windows.h>

std::string blood::fn::text::convert_wstring_to_unicode(std::wstring in)
{
    return std::string();
}

std::wstring blood::fn::text::convert_multybyte_to_wstring(std::string in)
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

std::wstring blood::fn::text::convert_string_to_wstring(std::string in)
{
    return std::wstring(in.begin(), in.end());
}

std::string blood::fn::text::convert_cp1251_to_unicode(std::string input)
{
    const int size = input.size();
    char *in;
    char *out;
    in = new char[size];
    out = new char[size * 2];

    static const int table[128] = {
            0x82D0, 0x83D0, 0x9A80E2, 0x93D1, 0x9E80E2, 0xA680E2, 0xA080E2, 0xA180E2,
            0xAC82E2, 0xB080E2, 0x89D0, 0xB980E2, 0x8AD0, 0x8CD0, 0x8BD0, 0x8FD0,
            0x92D1, 0x9880E2, 0x9980E2, 0x9C80E2, 0x9D80E2, 0xA280E2, 0x9380E2, 0x9480E2,
            0, 0xA284E2, 0x99D1, 0xBA80E2, 0x9AD1, 0x9CD1, 0x9BD1, 0x9FD1,
            0xA0C2, 0x8ED0, 0x9ED1, 0x88D0, 0xA4C2, 0x90D2, 0xA6C2, 0xA7C2,
            0x81D0, 0xA9C2, 0x84D0, 0xABC2, 0xACC2, 0xADC2, 0xAEC2, 0x87D0,
            0xB0C2, 0xB1C2, 0x86D0, 0x96D1, 0x91D2, 0xB5C2, 0xB6C2, 0xB7C2,
            0x91D1, 0x9684E2, 0x94D1, 0xBBC2, 0x98D1, 0x85D0, 0x95D1, 0x97D1,
            0x90D0, 0x91D0, 0x92D0, 0x93D0, 0x94D0, 0x95D0, 0x96D0, 0x97D0,
            0x98D0, 0x99D0, 0x9AD0, 0x9BD0, 0x9CD0, 0x9DD0, 0x9ED0, 0x9FD0,
            0xA0D0, 0xA1D0, 0xA2D0, 0xA3D0, 0xA4D0, 0xA5D0, 0xA6D0, 0xA7D0,
            0xA8D0, 0xA9D0, 0xAAD0, 0xABD0, 0xACD0, 0xADD0, 0xAED0, 0xAFD0,
            0xB0D0, 0xB1D0, 0xB2D0, 0xB3D0, 0xB4D0, 0xB5D0, 0xB6D0, 0xB7D0,
            0xB8D0, 0xB9D0, 0xBAD0, 0xBBD0, 0xBCD0, 0xBDD0, 0xBED0, 0xBFD0,
            0x80D1, 0x81D1, 0x82D1, 0x83D1, 0x84D1, 0x85D1, 0x86D1, 0x87D1,
            0x88D1, 0x89D1, 0x8AD1, 0x8BD1, 0x8CD1, 0x8DD1, 0x8ED1, 0x8FD1
    };
    while (*in)
    if (*in & 0x80) {
        int v = table[(int)(0x7f & *in++)];
        if (!v)
            continue;
        *out++ = (char)v;
        *out++ = (char)(v >> 8);
        if (v >>= 16)
            *out++ = (char)v;
    }
    else
        *out++ = *in++;
    *out = 0;
return std::string(out);
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