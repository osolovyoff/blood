#pragma once
#ifndef rsa_crypter_h__
#define rsa_crypter_h__

#include <fstream>
#include <vector>
#include <string>

namespace blood { namespace crypt {

class CrypterRSA
{
    static const wchar_t* const default_public_key_name;

    typedef std::wstring wstring;
    typedef std::vector<unsigned short> dvector;

public:
    CrypterRSA();

    void initializeByGeneration();
    void initializeByData(const size_t _n, const size_t _d, const size_t _e);
    bool is_initialize();

    dvector encryptStringToDigits(const wstring& _string, const size_t _n, const size_t _public_key);
    wstring decryptDigitsToString(const dvector& _digits, const size_t _private_key);
    
    wstring encryptString(const wstring& _string, const size_t _n, const size_t _public_key);
    wstring decryptString(const wstring& _string, const size_t _private_key);
    
    size_t getModule() const { return m_n; }
    size_t getPublicKey() const { return m_e; }
    size_t getPrivateKey() const { return m_d; }
    
    bool savePublicKeyToFile(const wchar_t* _path = default_public_key_name) const;
    bool openPublicKey(const wchar_t* _path = default_public_key_name);

    void run();

public:
    static void   setUnicodeEncoding(std::fstream& _file);
    static size_t getSimpleNumberBySieveOfSundaram(const size_t _number_limits);
    static size_t getGreatestCommonDivisorByAlgorithmOfEuclid(size_t _l, size_t _r);

private:
    void generatePrivateAndPublicKey(size_t _private_simple_number, size_t _public_simple_number);

private:
    bool   m_is_initialize;

    size_t m_n;
    
    size_t m_d; // Private Key
    size_t m_e; // Public  Key

}; // CrypterRSA

}} // blood::crypt

#endif // rsa_crypter_h__