#include "reg.hxx"

#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstringstream RegStringStream;
#else
typedef std::stringstream RegStringStream;
#endif

#define HKEY_ERROR (( HKEY ) (ULONG_PTR)((LONG)0x80000013) )
#define REG_ERROR -1
const RegSymbol folder_separator = TEXT('\\');
typedef unsigned __int64 QWORD;
const DWORD MAX_BUFFER_SIZE = 4096;

inline std::vector<RegString> split_string(RegString in, const RegSymbol symbol)
{
    std::vector<RegString> elements;
    RegStringStream ss(in);
    RegString item;
    while (std::getline(ss, item, symbol))
        elements.push_back(item);
    return elements;
}

//____________________________________________________________________________
// RegVariable

RegVariable::RegVariable(HKEY* parent, RegString name, DWORD type)
{
    m_parent_key = parent;
    m_name = name;
	m_type = type;
}

RegString RegVariable::get_value() const
{
    const DWORD BUFFER_SIZE = 1024;
    RegSymbol   data[BUFFER_SIZE];
    unsigned long buf_size = BUFFER_SIZE;
    const long is_success = RegQueryValueEx(*m_parent_key, m_name.c_str(), 0, 0, (LPBYTE)data, &buf_size);
    if (m_type == REG_SZ)
        return RegString(data);
    if (m_type == REG_EXPAND_SZ)
        return RegString(data);
    if (m_type == REG_LINK)
        return RegString(data);
    if (m_type == REG_FULL_RESOURCE_DESCRIPTOR)
        return RegString(data);
    if (m_type == REG_BINARY)
        return RegKey::convert_registry_data_to_string<bool>(data, buf_size);
    if (m_type == REG_DWORD)
        return RegKey::convert_registry_data_to_string<DWORD>(data, buf_size);
    if (m_type == REG_DWORD_LITTLE_ENDIAN)
        return RegKey::convert_registry_data_to_string<DWORD>(data, buf_size);
    if (m_type == REG_DWORD_BIG_ENDIAN)
        return RegKey::convert_registry_data_to_string<DWORD>(data, buf_size);
    if (m_type == REG_QWORD)
        return RegKey::convert_registry_data_to_string<QWORD>(data, buf_size);
    if (m_type == REG_QWORD_LITTLE_ENDIAN)
        return RegKey::convert_registry_data_to_string<QWORD>(data, buf_size);

    return RegString();
}

RegString RegVariable::get_name() const
{
    return m_name;
}

std::ostream& operator<<(std::ostream& os, const RegVariable& rv)
{
    os << rv.m_name << TEXT(" | ") << rv.get_value();
    return os;
}

//____________________________________________________________________________
// RegKey

 RegKey::RegKey(RegString path)
 : m_is_open(false)
 , m_handle(NULL)
 {
     if (folder_separator == *path.rbegin())
         path = RegString(path.begin(), path.end() - 1);

     const std::vector<RegString> v = split_string(path, folder_separator);
     if (v.empty())
         return;

    if (v.size() == 1)
    {
        m_handle = convert_string_to_hkey(path);
        m_hkey = m_handle;
        m_is_open = true;
    }
    else
    {
        const RegString handle_string = v.front();

        m_handle = convert_string_to_hkey(handle_string);
        m_path = path.substr(handle_string.size() + 1);

        const long status = RegOpenKeyEx(m_handle, m_path.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &m_hkey);
        m_is_open = (ERROR_SUCCESS == status);
    }
 }

RegKey::~RegKey()
{
    if (m_is_open)
    {
        RegCloseKey(m_handle);
        m_is_open = false;
    }
}

std::vector<RegString>& RegKey::get_subkeys()
{
	if (m_is_open)
	{
		DWORD i = 0;
		while (true)
		{
            const DWORD size = MAX_BUFFER_SIZE;
			RegSymbol keyname[size];
            const auto result = RegEnumKey(m_hkey, i, keyname, size);
			if (result == 0)
			{
				m_subkeys.emplace_back(keyname);
				++i;
			}
			else
				break;
		}	
	}
	return m_subkeys;
}

std::vector<RegVariable>& RegKey::get_variables()
{
	if (m_is_open)
	{
		int i = 0;
		while (true)
		{
            RegSymbol keyname[MAX_BUFFER_SIZE];
			DWORD size;
			DWORD type;
            BYTE data[MAX_BUFFER_SIZE];
            const auto result = RegEnumValue(m_hkey, i, keyname, &size,/*lpReserved*/ NULL,/*lpType*/ &type,/*lpData*/ NULL,/*lpcbData*/ NULL);
			if (result == ERROR_SUCCESS && is_correctly_type(type))
			{ 
                const auto result_data = RegQueryValueEx(m_hkey, keyname, NULL, &type, data, &size);
                m_variables.emplace_back(&m_hkey, keyname, type);
				++i;	
				size = 32767;
			}
			else
			{
				break;
			}
		}
	}
    return m_variables;
}

HKEY* RegKey::get_hkey()
{
    return &m_hkey;
}

RegString RegKey::get_path() const
{
    return convert_hkey_to_string(m_handle) + folder_separator +
        ( m_path.empty()
        ? m_path
        : m_path + folder_separator);
}

void RegKey::operator=(RegString registry_key_string)
{
    *this = RegKey(registry_key_string);
}

void RegKey::operator=(const RegKey& reg)
{
    m_path = reg.m_path;
    m_is_open = reg.m_is_open;
}

std::ostream& operator<<(std::ostream& os, const RegKey& rk)
{
    os << rk.convert_hkey_to_string(rk.m_handle) << folder_separator << rk.m_path << folder_separator;
    return os;
}

RegString RegKey::convert_hkey_to_string(const HKEY& hkey)
{
    if (HKEY_CLASSES_ROOT == hkey)
        return TEXT("HKEY_CLASSES_ROOT");
    else if (HKEY_CURRENT_USER == hkey)
        return TEXT("HKEY_CURRENT_USER");
    else if (HKEY_LOCAL_MACHINE == hkey)
        return TEXT("HKEY_LOCAL_MACHINE");
    else if (HKEY_USERS == hkey)
        return TEXT("HKEY_USERS");
    else if (HKEY_CURRENT_CONFIG == hkey)
        return TEXT("HKEY_CURRENT_CONFIG");
    else
        return RegString();
}

HKEY RegKey::convert_string_to_hkey(const RegString& regpath)
{
    if (regpath == TEXT("HKEY_CLASSES_ROOT"))
        return HKEY_CLASSES_ROOT;
    else if (regpath == TEXT("HKEY_CURRENT_USER"))
        return HKEY_CURRENT_USER;
    else if (regpath == TEXT("HKEY_LOCAL_MACHINE"))
        return HKEY_LOCAL_MACHINE;
    else if (regpath == TEXT("HKEY_USERS"))
        return HKEY_USERS;
    else if (regpath == TEXT("HKEY_CURRENT_CONFIG"))
        return HKEY_CURRENT_CONFIG;
    else
        return HKEY_ERROR;
}

RegString RegKey::convert_bool_to_string(RegSymbol* data, unsigned long size)
{
    bool result;
    memcpy(&result, data, sizeof(bool));
    return (result == 0) ? TEXT("1") : TEXT("0");
}

bool RegKey::is_correctly_type(const DWORD& type)
{
    switch (type)
    {
    case REG_SZ:
    case REG_BINARY:
    case REG_DWORD:
    case REG_DWORD_BIG_ENDIAN:
    case REG_LINK:
    case REG_EXPAND_SZ:
    case REG_MULTI_SZ:
    case REG_RESOURCE_LIST:
    case REG_FULL_RESOURCE_DESCRIPTOR:
    case REG_RESOURCE_REQUIREMENTS_LIST:
    case REG_QWORD:
        return true;
    default:
        return false;
    }
}