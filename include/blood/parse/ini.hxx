#pragma once
#ifndef blood_parse_ini_hxx_
#define blood_parse_ini_hxx_

#include <map>
#include <vector>

namespace blood { namespace parse {

class INI
{
    typedef std::pair<std::string,std::string> wordpair;

public:
    INI(const char* _path);
    INI(const std::string& _data_string);
    ~INI();

    void copy(const INI& _cfg);
    bool save();
    void clear();

    std::string getValue(const std::string& _key) const;
    std::string getValue(const char* _key) const;

    int         getNumericValue(const std::string& _key) const;
    int         getNumericValue(const char* _key) const;

    void        setValue(const std::string _key, const std::string& _value);
    void        setValue(const std::string _key, const char* _value);
    void        setValue(const char* _key, const std::string& _value);
    void        setValue(const char* _key, const char* _value);

    std::vector<wordpair>& getData();
private:
    bool isLineBeSkipped(const std::string& _string);
    void parseStringStream(std::stringstream& _ss);
    wordpair findValue(const char* _key);

private:
    std::vector<wordpair>              m_data;
    std::string                        m_path;
    std::pair<std::string,std::string> m_empty;

public:
    const int ERROR_NUMERIC = 2147483646;
};

}} // end blood::parse

#endif // blood_parse_ini_hxx_
