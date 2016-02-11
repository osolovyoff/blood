#include "../../../include/blood/parse/ini.hxx"
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>

using namespace blood::parse;

INI::INI(const char* _path)
{
    std::ifstream fin(_path, std::ios::in);
    if (fin.is_open())
    {
        std::stringstream ss;
        std::copy( std::istreambuf_iterator<char>(fin)
                 , std::istreambuf_iterator<char>()
                 , std::ostreambuf_iterator<char>(ss));

        parseStringStream(ss);
        fin.close();
    }
    else
    {
        printf("config file not open\n");
    }
}

INI::INI(const std::string& _data_string)
{
    std::stringstream ss; ss << _data_string;
    parseStringStream(ss);
}

INI::~INI()
{
    save();
}

void INI::copy(const INI& _cfg)
{
    for (auto& pair : _cfg.m_data)
        this->m_data.emplace_back(pair.first, pair.second);
}

bool INI::save()
{
    if (!m_path.empty())
    {
        std::ofstream fout(m_path.c_str(), std::ios::out);
        if (fout.is_open())
        {
            for (auto i = m_data.begin(); i < m_data.end(); ++i)
            {
                auto pair = *i;
                if (i != m_data.begin())
                    fout << std::endl;

                if (pair.first[0] == '#')
                    fout << pair.second;// comment
                else
                    fout << pair.first << "=" << pair.second;
            }

            fout.close();
            return true;
        }
    }
    return false;
}

void INI::clear()
{
    m_data.clear();
}

void INI::parseStringStream(std::stringstream& _ss)
{
    std::string line;
    while (!_ss.eof())
    {
        std::getline(_ss, line);
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        if (isLineBeSkipped(line) || line.empty())
        {
            const std::string key = "#";
            const std::string value = line;
            m_data.emplace_back(key, value);
        }
        else
        {
            const auto it = std::find(line.begin(), line.end(), '=');
            const size_t pos = std::distance(line.begin(), it);
            line.erase(it);
            const std::string key = line.substr(0, pos);
            const std::string value = line.substr(pos);
            m_data.emplace_back(key, value);
        }

    }
    printf("%s configuration file loaded!\n", m_path.c_str());
}

std::string INI::getValue(const std::string& _key) const
{
    for (auto& i:m_data)
        if (i.first[0]!='#')
            if (i.first == _key)
                return i.second;
    return std::string();
}

std::string INI::getValue(const char* _key) const
{
    for (auto& i : m_data)
    if (i.first[0] != '#')
    if (i.first.compare(_key) == 0)
        return i.second;
    return std::string();
}

int INI::getNumericValue(const std::string& _key) const
{
    const std::string s = getValue(_key);
    return (s.empty())
        ? ERROR_NUMERIC
        : std::atoi(s.c_str());
}

int INI::getNumericValue(const char* _key) const
{
    const std::string str = getValue(_key);
    return std::atoi(str.c_str());
}

bool INI::isLineBeSkipped(const std::string& _string)
{
    std::vector<char> banned_symbols = { ';', '[', '#', '\n' };
    unsigned int i = 0;

    while (true)
    {
        const char symbol = _string[i];

        if (symbol == ' ')
            continue;

        for (auto banned : banned_symbols)
            if (symbol == banned)
                return true;

        return false;
    }
}

void INI::setValue(const std::string _key, const std::string& _value)
{
    std::pair<std::string,std::string> s = findValue(_key.c_str());
    if (s.first.empty())
        m_data.emplace_back(_key, _value);
    else
        s = std::make_pair(_key, _value);
}

void INI::setValue(const std::string _key, const char* _value)
{
    std::pair<std::string, std::string> s = findValue(_key.c_str());
    if (s.first.empty())
        m_data.emplace_back(_key, _value);
    else
        s = std::make_pair(_key, _value);
}

void INI::setValue(const char* _key, const std::string& _value)
{
    std::pair<std::string, std::string> s = findValue(_key);
    if (s.first.empty())
        m_data.emplace_back(_key, _value);
    else
        s = std::make_pair(_key, _value);
}

void INI::setValue(const char* _key, const char* _value)
{
    std::pair<std::string, std::string> s = findValue(_key);
    if (s.first.empty())
        m_data.emplace_back(_key, _value);
    else
        s = std::make_pair(_key, _value);
}

INI::wordpair INI::findValue(const char* _key)
{
    for (auto& i : m_data)
    {
        if (i.first[0] == '#')
            continue;

        if (i.first.compare(_key) == 0)
            return i;
    }
    return m_empty;
}

std::vector<blood::parse::INI::wordpair>& blood::parse::INI::getData()
{
    return m_data;
}
