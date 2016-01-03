#pragma once
#if defined(WIN32)
#ifndef blood_register_h__
#define blood_register_h__

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <windows.h>
#include <winnt.h>

#if defined(UNICODE)
typedef std::wstring RegString;
typedef WCHAR        RegSymbol;
#else
typedef std::string  RegString;
typedef char         RegSymbol;
#endif

class RegVariable
{
    RegVariable() = delete;
public:
    RegVariable(HKEY* parent, RegString path, DWORD type);

public:
    RegString get_value() const;
    RegString get_name() const;

    friend std::ostream& operator<<(std::ostream& os, const RegVariable& rv);

private:
    HKEY*       m_parent_key;
    RegString   m_name;
    DWORD       m_type;
};

class RegKey
{
    friend class RegVariable;
    RegKey() = delete;
public:
	RegKey(RegString regpath);
    ~RegKey();

public:
	RegString                  get_path() const;
	std::vector<RegString>&    get_subkeys();
	std::vector<RegVariable>&  get_variables();
    HKEY*                      get_hkey();

    void operator= (RegString hkey_string);
    void operator= (const RegKey& reg);
    friend std::ostream& operator<<(std::ostream& os, const RegKey& rk);

private:
    bool         m_is_open;
    HKEY         m_hkey;
    HKEY         m_handle;
    RegString    m_path;

private:
    std::vector<RegString>   m_subkeys;
    std::vector<RegVariable> m_variables;

public: // static
    static RegString    convert_hkey_to_string(const HKEY& hkey);
    static HKEY         convert_string_to_hkey(const RegString& regpath);

    static RegString    convert_bool_to_string(RegSymbol* data, unsigned long size);
    bool                is_correctly_type(const DWORD& type);

    template <class T>
    static RegString convert_registry_data_to_string(RegSymbol* data, unsigned long size)
    {
        T result;
        memcpy(&result, data, sizeof(T));
#if defined(UNICODE)
        return std::to_wstring(result);
#else
        return std::to_string(result);
#endif
        
    }
};

#endif//blood_register_h__
#endif// WIN32