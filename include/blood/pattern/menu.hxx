#pragma once
#ifndef blood_pattern_menu_hxx_
#define blood_pattern_menu_hxx_

#include <string>
#include <functional>
#include <unordered_map>

namespace blood { namespace pattern {

class Menu
{
    typedef std::string string;
    typedef std::pair<int, const char*> ConditionTitle;

public:
    Menu(int _start_condition, const string& _condition_name = "");
    void run();

private:
    virtual void initialize() = 0;

protected:
    template<class O, class M>
    void setCallBack(int _condition, O* _obj, M _method){ m_functions_condition[_condition] = std::bind(_method, _obj); };
    void writeMenuItem(int _state, const string& _desc);
    void changeState(int _key, std::initializer_list<int> _list);
    void chooseCondition(std::initializer_list< std::pair< int, const string&> > _list);
    void exitMenu();
    void setCondition(int _condition);

// additional functions
private:
    void welcomeTitle(const string& _state);

private:
    int     m_condition;
    string  m_root_condition_name;
    bool    m_is_working;

    std::unordered_map< int, std::function<void()> > m_functions_condition;
};

}} // end blood::pattern

#endif // blood_pattern_menu_hxx_
