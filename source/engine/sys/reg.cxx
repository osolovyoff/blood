#include "reg.hxx"
#pragma comment(lib, "Advapi32.lib")


#define HKEY_ERROR (( HKEY ) (ULONG_PTR)((LONG)0x80000013) )
#define REG_ERROR -1
typedef unsigned __int64 QWORD;

Reg::Reg()
: m_path(0)
, m_is_open(false)
{
}

inline std::vector<std::string> split_string(std::string in, const char symbol)
{
    std::vector<std::string> elements;
    std::stringstream ss(in);
    std::string item;
    while (std::getline(ss, item, symbol))
        elements.push_back(item);
    return elements;
}

 Reg::Reg(std::string path)
 : m_is_open(false)
 {
     const auto v = split_string(path, '\\');
     std::cout << "handle:" << v.front() << std::endl;

     if (v.size() >= 3)
     {
         m_hkey_handle = convert_string_to_hkey(v.front());

         for (int i = 1; i < v.size() - 1; ++i)
             m_path += v[i] + '\\';
         m_path = m_path.substr(0, m_path.size() - 1);

         m_name = v[v.size() - 1];

         std::cout << "path:" << m_path << std::endl;
         std::cout << "name:" << m_name << std::endl;
     }
 }

Reg::~Reg()
{
    close();
}


void Reg::zero()
{

}

void Reg::open()
{
    if (!m_is_open)
    {
        const long status = RegOpenKeyEx(m_hkey_handle, m_path.c_str(), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &m_hkey);
        if (status == ERROR_SUCCESS)
        {
            m_is_open = true;
            std::cout << "success open" << std::endl;
        }
        else
        {
            m_is_open = false;
            std::cout << "error open" << std::endl;
        }
    }
}

void Reg::close()
{
    if (m_is_open)
    {
        RegCloseKey(m_hkey_handle);
        m_is_open = false;
    }
}

void Reg::print()
{
    if (m_is_open)
    {
        const std::string hkey = convert_hkey_to_string(m_hkey_handle);
        if (!hkey.empty())
        {
            std::cout << hkey << '\\' << m_path << '\\' << m_name << std::endl;
        }
    }
}

void Reg::create()
{

}

void Reg::remove()
{
    if (m_is_open)
    {
        RegDeleteValue(m_hkey, m_name.c_str());
        close();
    }
}

void Reg::set_value(std::string value, std::string type)
{
    if (m_is_open)
    {
        m_type = convert_string_to_type(type);
        if (m_type != REG_ERROR)
        {
            const long is_success = RegSetValueEx(m_hkey, m_name.c_str(), 0, m_type, (LPBYTE)value.c_str(), value.size());
            if (is_success == ERROR_SUCCESS)
            {
                std::cout << "new value:" << value << std::endl;
                m_is_open = true;
            }
        }
    }
}

std::string Reg::get_value(std::string type)
{
    char  data[255];
    unsigned long buf_size = 255;
    const long is_success = RegQueryValueEx(m_hkey, m_name.c_str(), 0, 0, (LPBYTE)data, &buf_size);

    const int itype = convert_string_to_type(type);
    if (itype == REG_SZ)
        return std::string(data);
    if (itype == REG_BINARY)
        return convert_bool_to_string(data, buf_size);
    if (itype == REG_DWORD)
        return convert_dword_to_string(data, buf_size);
    if (itype == REG_QWORD)
        return convert_qword_to_string(data, buf_size);

    return std::string();
}

void Reg::operator=(std::string registry_key_string)
{
    *this = Reg(registry_key_string);
}

void Reg::operator=(const Reg& reg)
{
    m_path = reg.m_path;
    m_is_open = reg.m_is_open;
}

std::string Reg::convert_hkey_to_string(HKEY hkey)
{
    if (HKEY_CLASSES_ROOT == hkey)
        return "HKEY_CLASSES_ROOT";
    else if (HKEY_CURRENT_USER == hkey)
        return "HKEY_CURRENT_USER";
    else if (HKEY_LOCAL_MACHINE == hkey)
        return "HKEY_LOCAL_MACHINE";
    else if (HKEY_USERS == hkey)
        return "HKEY_USERS";
    else if (HKEY_CURRENT_CONFIG == hkey)
        return "HKEY_CURRENT_CONFIG";
    else
        return std::string();
}

HKEY Reg::convert_string_to_hkey(std::string regpath)
{
    if (regpath == "HKEY_CLASSES_ROOT")
        return HKEY_CLASSES_ROOT;
    else if (regpath == "HKEY_CURRENT_USER")
        return HKEY_CURRENT_USER;
    else if (regpath == "HKEY_LOCAL_MACHINE")
        return HKEY_LOCAL_MACHINE;
    else if (regpath == "HKEY_USERS")
        return HKEY_USERS;
    else if (regpath == "HKEY_CURRENT_CONFIG")
        return HKEY_CURRENT_CONFIG;
    else
        return HKEY_ERROR;
}

int Reg::convert_string_to_type(std::string type_string)
{
    if (type_string == "REG_BINARY")
        return REG_BINARY;
    if (type_string == "REG_DWORD")
        return REG_DWORD;
    if (type_string == "REG_QWORD")
        return REG_QWORD;
    if (type_string == "REG_EXPAND_SZ")
        return REG_EXPAND_SZ;
    if (type_string == "REG_FULL_RESOURCE_DESCRIPTOR")
        return REG_FULL_RESOURCE_DESCRIPTOR;
    if (type_string == "REG_LINK")
        return REG_LINK;
    if (type_string == "REG_MULTI_SZ")
        return REG_MULTI_SZ;
    if (type_string == "REG_RESOURCE_LIST")
        return REG_RESOURCE_LIST;
    if (type_string == "REG_RESOURCE_REQUIREMENTS_LIST")
        return REG_RESOURCE_REQUIREMENTS_LIST;
    if (type_string == "REG_SZ")
        return REG_SZ;
    if (type_string == "REG_NONE")
        return REG_NONE;

    return REG_ERROR;
}

std::string Reg::convert_bool_to_string(char* data, unsigned long size)
{
    bool result;
    memcpy(&result, data, size);
    return (result == 0) ? "1" : "0";
}

std::string Reg::convert_dword_to_string(char* data, unsigned long size)
{
    DWORD result;
    memcpy(&result, data, size);
    return std::to_string(result);
}

std::string Reg::convert_qword_to_string(char* data, unsigned long size)
{
    QWORD result;
    memcpy(&result, data, size);
    return std::to_string(result);
}

std::vector<std::string> Reg::split_string(std::string in, const char symbol)
{
    std::vector<std::string> elements;
    std::stringstream ss(in);
    std::string item;
    while (std::getline(ss, item, symbol))
        elements.push_back(item);
    return elements;
}