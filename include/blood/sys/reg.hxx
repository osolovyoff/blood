#pragma once
#ifndef blood_register_h__
#define blood_register_h__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <windows.h>
#include <winnt.h>

class Reg
{
public:
	Reg();
	Reg(std::string regpath);

    ~Reg();

public:
    void zero();
    void open();
    void close();
    void print();
    void create();
    void remove();

    void operator= (std::string hkey_string);
    void operator= (const Reg& reg);

    void set_value(std::string value, std::string type);
    std::string get_value(std::string type);

private: // additional 
    std::string convert_hkey_to_string(HKEY hkey);
    HKEY        convert_string_to_hkey(std::string regpath);
    int         convert_string_to_type(std::string type_string);

    std::string convert_bool_to_string(char* data, unsigned long size);
    std::string convert_dword_to_string(char* data, unsigned long size);
    std::string convert_qword_to_string(char* data, unsigned long size);

    inline std::vector<std::string> split_string(std::string in, const char symbol);

private:
    bool        m_is_open;
    HKEY        m_hkey;
    int         m_type;

    HKEY        m_hkey_handle;
    std::string m_path;
    std::string m_name;
};

#endif//blood_register_h__