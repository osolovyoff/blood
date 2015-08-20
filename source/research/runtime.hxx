#pragma once
#include <stack>
#include <map>

class Runtime
{
    typedef std::string string;
public:
    void   add();
    void   sub();
    void   mul();
    void   div();
    void   pop();
    void   push(const string& value);

    void   fetch(const string& variable);
    void   store(const string& variable);

private:
    std::map<string, string>   m_values;
    std::stack<string> m_stack;
};