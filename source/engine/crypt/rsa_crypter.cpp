#include "engine/crypter/rsa_crypter.hpp"
#include "engine/platform.hpp"

namespace blood {
namespace crypt {

#if defined(ENGINE_WIN)
#include <locale>
#include <codecvt>
#else
#endif

const wchar_t* const CrypterRSA::default_public_key_name = L"public.key";

CrypterRSA::CrypterRSA()
: m_is_initialize(false)
, m_n(0), m_d(0), m_e(0)
{}

void CrypterRSA::initializeByGeneration()
{
    srand((unsigned)time(NULL));
    const size_t prvt_key = rand() % 100;
    const size_t pblc_key = rand() % 100;

    const size_t p = getSimpleNumberBySieveOfSundaram(prvt_key);
    const size_t q = getSimpleNumberBySieveOfSundaram(pblc_key);

    m_n = (p * q);

    generatePrivateAndPublicKey(p, q);
    
    m_is_initialize = true;
}

void CrypterRSA::initializeByData(const size_t _n, const size_t _d, const size_t _e)
{
    m_n = _n;
    m_d = _d;
    m_e = _e;
}

bool CrypterRSA::is_initialize()
{
    return m_is_initialize;
}

CrypterRSA::dvector CrypterRSA::encryptStringToDigits(const wstring& _string, const size_t _n, const size_t _public_key)
{
    const size_t string_len = _string.size();
    dvector crypt_digits; crypt_digits.resize(string_len);

    for (size_t iter = 0; iter < _string.size(); ++iter)
    {
        const wchar_t symbol = _string[iter];
        const size_t symbol_code = static_cast<size_t>(symbol);
        size_t code = 1;
        size_t i = 0;
        while (i < _public_key)
        {
            code = code*symbol_code;
            code = code%m_n;
            i++;
        }
        const wchar_t crypt_symbol = static_cast<wchar_t>(code);
        crypt_digits[iter] = crypt_symbol;
    }
    return crypt_digits;
}

wstring CrypterRSA::decryptDigitsToString(const dvector& _digits, const size_t _private_key)
{
    if (is_initialize())
    {
        const size_t string_len = _digits.size();
        std::wstring decrypt_string; decrypt_string.resize(string_len);

        for (size_t iter = 0; iter < _digits.size(); ++iter)
        {
            const wchar_t symbol = _digits[iter];
            const size_t symbol_code = static_cast<size_t>(symbol);
            size_t nm = 1;
            size_t i = 0;
            while (i < _private_key)
            {
                nm = nm*symbol_code;
                nm = nm%m_n;
                i++;
            }
            const wchar_t decrypt_symbol = static_cast<wchar_t>(nm);
            decrypt_string[iter] = decrypt_symbol;
        }
        return decrypt_string;
    }
    else
    {
        //assert(false);
        return wstring();
    }
}

std::wstring CrypterRSA::encryptString(const std::wstring& _string, const size_t _n, const size_t _public_key)
{
    const size_t string_len = _string.size();
    std::wstring crypt_string; crypt_string.resize(string_len);

    for (size_t iter = 0; iter < _string.size(); ++iter)
    {
        const wchar_t symbol = _string[iter];
        const size_t symbol_code = static_cast<size_t>(symbol);
        size_t code = 1;
        size_t i = 0;
        while (i < _public_key)
        {
            code = code*symbol_code;
            code = code%m_n;
            i++;
        }
        const wchar_t crypt_symbol = static_cast<wchar_t>(code);
        crypt_string[iter] = crypt_symbol;
    }
    return crypt_string;
}

std::wstring CrypterRSA::decryptString(const std::wstring& _string, const size_t _private_key)
{
    if (is_initialize())
    {
        const size_t string_len = _string.size();
        std::wstring decrypt_string; decrypt_string.resize(string_len);

        for (size_t iter = 0; iter < _string.size(); ++iter)
        {
            const wchar_t symbol = _string[iter];
            const size_t symbol_code = static_cast<size_t>(symbol);
            size_t nm = 1;
            size_t i = 0;
            while (i < _private_key)
            {
                nm = nm*symbol_code;
                nm = nm%m_n;
                i++;
            }
            const wchar_t decrypt_symbol = static_cast<wchar_t>(nm);
            decrypt_string[iter] = decrypt_symbol;
        }
        return decrypt_string;
    }
    else
    {
        //assert(false);
        return wstring();
    }
}

void CrypterRSA::generatePrivateAndPublicKey(size_t _private_simple_number, size_t _public_simple_number)
{
    m_d = 0;
    size_t d_simple = 0;
    while (d_simple != 1)
    {
        m_d = rand() % 100;
        d_simple = getGreatestCommonDivisorByAlgorithmOfEuclid(m_d, ((_private_simple_number - 1)*(_public_simple_number - 1)));
    }

    m_e = 0;
    size_t e_simple = 0;
    while (e_simple != 1)
    {
        m_e += 1;
        e_simple = (m_e*m_d) % ((_private_simple_number - 1)*(_public_simple_number - 1));
    }
    //std::cout << "secret key: " << m_d << std::endl;
    //std::cout << "open key: " << m_e << std::endl;
}

void CrypterRSA::setUnicodeEncoding(std::fstream& _file)
{
#if defined(ENGINE_WIN)
    std::locale::global(std::locale(""));
    std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
    _file.imbue(loc);
#endif
}

size_t CrypterRSA::getSimpleNumberBySieveOfSundaram(const size_t _number_limits)
{
    size_t *a = new size_t[_number_limits], i, j, k;
    memset(a, 0, sizeof(size_t)* _number_limits);
    for (i = 1; 3 * i + 1 < _number_limits; i++)
    {
        for (j = 1; (k = i + j + 2 * i*j) < _number_limits && j <= i; j++)
            a[k] = 1;
    }

    size_t result = 0;
    for (i = _number_limits - 1; i >= 1; i--)
    {
        if (a[i] == 0)
        {
            result = (2 * i + 1);
            break;
        }
    }

    delete[] a;
    return result;
}

size_t CrypterRSA::getGreatestCommonDivisorByAlgorithmOfEuclid(size_t _l, size_t _r)
{
    // TODO FINISH on size_t to LONG LONG
    size_t c;
    while (_r)
    {
        c = _l % _r;
        _l = _r;
        _r = c;
    }
    const int value = static_cast<int>(_l);     // TODO CHECK OR DELETE
    _l = static_cast<size_t>(std::abs(value));  // TODO CHECK OR DELETE
    return _l;
}

bool CrypterRSA::savePublicKeyToFile(const wchar_t* _path /*= default_public_key_name*/ ) const
{
    std::ofstream fout(_path, std::ios::binary | std::ios::out);
    if (fout.is_open())
    {
        fout.write((char*)&m_n, sizeof(m_n));
        fout.write((char*)&m_e, sizeof(m_e));
        fout.close();
        return true;
    }
    return false;
}

bool CrypterRSA::openPublicKey(const wchar_t* _path /*= default_public_key_name*/ )
{
    std::ifstream fin(_path, std::ios::binary | std::ios::in);
    if (fin.is_open())
    {
        fin.read((char*)&m_n, sizeof(m_n));
        fin.read((char*)&m_e, sizeof(m_e));
        fin.close();
        return true;
    }
    return false;
}

#include "openssl/rsa.h"

void CrypterRSA::run()
{
    
}

} // end crypt
} // end blood
