#pragma once
#include "parse/any.hxx"

#include <memory>
#include <string>
#include <vector>

namespace blood { namespace parse {

class CSV: public ANY_Parser
{
    typedef std::vector<std::string> values;

public:
    CSV(const char* _path, const bool _has_header);
    CSV(const std::string& _data_string, const bool _has_header);

    std::string getCell(int i, int j) const;

    static values convertLineToValues(const std::string& _line);

private:
    virtual void parseStringStream(std::stringstream& _ss) override;

private:
    std::vector<std::string>              m_header;
    std::vector<std::vector<std::string>> m_table;
};

}} // end blood::parse