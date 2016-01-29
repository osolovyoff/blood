#pragma once
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

/// \class RegString
/// System Registry definition of a string or a wide string entry

/// \class RegSymbol
/// definition of a char or wide char
#if defined(UNICODE)
typedef std::wstring RegString;
typedef WCHAR        RegSymbol;
#else
typedef std::string  RegString;
typedef char         RegSymbol;
#endif

/// \class RegVariable 
///
/// RegVariable is are entry of a System Registry 
///
class RegVariable
{
    /// Default constructor was removed
    RegVariable() = delete;
public:
    /// Default 
    /// \param parent is a HKEY pointer
    /// \param path is a RegString value
    /// \param type is a DWORD type of the RefVariable
    RegVariable(HKEY* parent, RegString path, DWORD type);

public:
    /// function get_value
    /// \param void
    /// value is a string data consist from other registry type
    /// also converting any type to registry string
    /// \return RegString (as unicode or wide string value
    RegString get_value() const;
    /// \fn RegString get_value() const
    /// \param void
    /// value is a string name of a registry variable
    /// \return RegString (as unicode or wide string value)
    RegString get_name() const;
    /// \fn RegString get_name() const
    /// \param void
    /// value is a string name of a registry variable
    /// \return RegString (as unicode or wide string value)
    friend std::ostream& operator<<(std::ostream& os, const RegVariable& rv);

private:
    /// key of a HKEY type
    HKEY*       m_parent_key;
    /// name of RegVariable
    RegString   m_name;
    /// type of RegVariable
    DWORD       m_type;
};

/// \class RegKey
///
/// RegKey is are entry of a System Registry with interface as a folder
///
class RegKey
{
    /// RegVariable friend class of a RegKey
    friend class RegVariable;
    /// Default constructor was removed
    RegKey() = delete;
public:
    /// \fn RegKey(RegString regpath)
    /// \param regpath is registry path for example "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Run"
    /// once constructor for the RegKey class
	RegKey(RegString regpath);

    /// \fn ~RegKey()
    /// \param free all dynamical initialize variable
    /// default destructor
    ~RegKey();

public:
    /// \fn RegString get_path() const
    /// \param void
    /// this function get root path for this variable
    /// \return RegString (as unicode or wide string value)
	RegString                  get_path() const;
    /// \fn std::vector<RegString>&    get_subkeys()
    /// \param void
    /// get local subkeys for current key
    /// \return constainer of registry string
	std::vector<RegString>&    get_subkeys();
    /// \fn std::vector<RegVariable>&  get_variables()
    /// \param void
    /// get local subkeys for current key
    /// \return container of string variables
	std::vector<RegVariable>&  get_variables();
    /// \fn HKEY* get_hkey()
    /// \param void
    /// get HKEY class member
    /// \return HKEY poiner
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

/// static block
public: 
    /// \fn static RegString convert_hkey_to_string(const HKEY& hkey);
    /// \param hkey is a HKEY constant from the <winnt.h> header
    /// simple converter from hkey to any string(wide char or char array)
    /// \return RegString value
    static RegString    convert_hkey_to_string(const HKEY& hkey);


    /// \fn static HKEY convert_string_to_hkey(const RegString& regpath)
    /// \param 
    /// simple converter from any string(wide char or char array) to hkey(system constant)
    /// \return hkey value
    static HKEY         convert_string_to_hkey(const RegString& regpath);


    /// \fn static RegString convert_bool_to_string(RegSymbol* data, unsigned long size)
    /// \param 
    /// 
    /// \return
    static RegString    convert_bool_to_string(RegSymbol* data, unsigned long size);


    /// \fn bool is_correctly_type(const DWORD& type)
    /// \param type is a DWORD constant
    /// check type of RegKey on valid 
    /// \return boolean value (true|false)
    bool                is_correctly_type(const DWORD& type);


    /// \fn static RegString convert_registry_data_to_string(RegSymbol* data, unsigned long size)
    /// \param simple char array data
    /// 
    /// \return RegString (as unicode or wide string value)
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