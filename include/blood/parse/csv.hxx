#pragma once
#ifndef blood_parse_csv_hxx_
#define blood_parse_csv_hxx_

#include <memory>
#include <string>
#include <vector>

namespace blood { namespace parse {

class CSV
{
    typedef std::vector<std::string> values;

public:
    CSV(const char* _path, const bool _has_header);
    CSV(const std::string& _data_string, const bool _has_header);

    std::string get_cell(int i, int j) const;

    static values convert_line_to_values(const std::string& _line);

private:
    virtual void parse_string_stream(std::stringstream& _ss);

private:
    std::string m_path;

private:
    std::vector<std::string>              m_header;
    std::vector<std::vector<std::string>> m_table;
};

}} // end blood::parse

#endif // blood_parse_arg_hxx_