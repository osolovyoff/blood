#include "runtime.hxx"
#include <string>

void   Runtime::add()
{
    const string l = m_stack.top();
    const string r = m_stack.top();
    if (l[0] == '\"' && r[0] == '\"')
    {
        const string out = l + r;
        push(out);
    }
    else
    {
        const double a = std::atof(l.c_str());
        const double b = std::atof(r.c_str());
        const double c = a + b;
        const string out = std::to_string(c);
        push(out);
    }
}

void   Runtime::sub()
{
}

void   Runtime::mul()
{
}

void   Runtime::div()
{
}

void   Runtime::pop()
{
    m_stack.pop();
}

void   Runtime::push(const string& value)
{
    m_stack.push(value);
}

void   Runtime::fetch(const string& variable)
{
    const auto i = m_values.find(variable);
    if (i != m_values.end())
    {
        const string value = i->second;
        push(value);
    }
    else
    {
        const std::string text = "variable " + variable + " not finded";
        throw std::runtime_error(text);
    }
}

void   Runtime::store(const string& variable)
{
    const std::string value = m_stack.top();
    m_values[variable] = value;
}