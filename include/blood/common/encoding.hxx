#pragma once
#include <vector>
#include <string>
namespace blood { namespace fn { namespace text {

    std::string               convert_wstring_to_unicode(std::wstring in);
    std::wstring              convert_multybyte_to_wstring(std::string  in);
    std::wstring              convert_string_to_wstring(std::string in);
    void                      convert_cp1251_to_unicode(std::string input, char* out);
    std::string               narrow_string(const std::wstring& str, const char* local_name = "C");
    std::string               replace(std::string& in, const std::string& from, const std::string& to);
    std::wstring              replace(std::wstring& in, const std::wstring& from, const std::wstring& to);

    std::vector<std::string>  split_string(std::string in, const char symbol);
    std::vector<std::wstring> split_wstring(std::wstring in, const wchar_t symbol);

	std::string               string_format(const std::string format, ...);

    bool is_number(std::string in);


}}} // end blood::fn::text;
