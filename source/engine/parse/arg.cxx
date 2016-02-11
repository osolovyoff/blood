#include "../../../include/blood/parse/arg.hxx"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace blood { namespace parse {

// _______________________________ Argument ___________________________________
    Argument::Argument()
        : m_name("")
        , m_title("")
        , m_fn(nullptr)
        , m_data(nullptr)
        , m_is_prefix(false)
    {
    }

    Argument::Argument(String name)
        : m_name(name)
        , m_title("")
        , m_fn(nullptr)
        , m_data(nullptr)
        , m_is_prefix(false)
    {
        m_name = name;
    }

    bool Argument::runFunction()
    {
        if (m_fn)
            return m_fn(m_next_value,m_data);
        return false;
    }

    void Argument::setFunction(RunFn _fn)
    {
        m_fn = _fn;
    }

    void Argument::setPrefix(bool is_prefix)
    {
        m_is_prefix = is_prefix;
    }

    void Argument::setTitle(String _title)
    {
        m_title = _title;
    }

    void Argument::setData(void* _data)
    {
        m_data = _data;
    }

    Argument& Argument::operator=(const Argument& r)
    {
        Argument& l = *this;
        l.m_fn = r.m_fn;
        l.m_data = r.m_data;
        l.m_title = r.m_title;
        l.m_is_prefix = r.m_is_prefix;
        l.m_next_value = r.m_next_value;
        return *this;
    }

// __________________________ CommandLineParser _______________________________

CommandLineParser::CommandLineParser()
{
    // Not all members of a class are initialized inside the constructor. Consider inspecting: m_use_help, m_use_block
    m_use_block = false;
}

CommandLineParser::CommandLineParser(int _count, char* _array[])
{
    m_path = _array[0];
    for (int i = 1; i < _count; ++i)
    {
        const char* arg_char = _array[i];
        const std::string arg = arg_char;
        m_args.emplace_back(arg);
    }
}

CommandLineParser::CommandLineParser(const std::vector<std::string> _args)
{
    m_path = _args[0];
    for (unsigned int i = 1; i < _args.size(); ++i)
    {
        const auto arg = _args[i];
        m_args.emplace_back(arg);
    }
}

void CommandLineParser::addArgument(Argument arg)
{
    const auto a = std::make_pair(arg.m_name, arg);
    m_keys.emplace(a);
}

bool CommandLineParser::processing()
{
    bool is_help = false;
    const auto v = { "-help", "--help" };
    for (auto word : v)
    {
        const auto i =
            std::find(m_args.begin(), m_args.end(), word);
        if (i != m_args.end())
        {
            is_help = true;
            break;
        }
    }

    bool any_argument_found = false;
    if (!is_help)
    {
        for (auto i = m_args.begin(); i != m_args.end(); ++i)
        {
            auto arg = *i;
            std::string word = arg;
            while (word.front() == '-')
            {
                word.erase(0, 1);
                auto k = m_keys.find(word);
                if (k != m_keys.end())
                {
                    any_argument_found = true;
                    Argument argument = k->second;
                    if (argument.m_is_prefix)
                    {
                        std::string value = "";
                        if (++i != m_args.end())
                            value = *i;
                        argument.m_next_value = value;

                        
                    }
                    const auto success =
                        argument.runFunction();

                    if (!success)
                        return false;
                }
            }
        }
    }
    else
    {
        help();
        return false;
    }

    if (m_use_help && !any_argument_found)
    {
        help();
        return false;
    }

    return !m_use_block;
}

void CommandLineParser::useEmptyListBlock(bool _block)
{
    m_use_block = _block;
}
void CommandLineParser::useEmptyListHelp(bool _help)
{
    m_use_help = _help;
}

void CommandLineParser::help() const
{
// USING BLOCK
    std::cout
        << std::endl
        << "Usage: program";
    for (auto& keyword:m_keys)
    {
        if (keyword.second.m_is_prefix)
        {
            std::cout
                << " [-"
                << keyword.first
                << " <value>]";
        }
        else
        {
            std::cout
                << " [-"
                << keyword.first
                << "]";
        }
    }
// DESCRIPTION        
    std::cout
        << std::endl
        << std::endl
        << "Options: "
        << std::endl;
    for (auto& keyword :m_keys)
    {
        Argument arg = keyword.second;
        if (!arg.m_title.empty())
        {
            std::cout
                << std::left
                << std::setw(5)
                << "    -"
                << std::setw(14)
                << keyword.first;

            std::cout
                << std::left
                << std::setw(30)
                << arg.m_title
                << std::endl;
        }
    }
    std::cout << std::endl;
}

}} // end blood::parse
