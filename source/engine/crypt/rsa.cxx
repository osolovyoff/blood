#if defined(USE_OPENSSL)
#include "crypt/rsa.hxx"
                        
#include "openssl/err.h"
#include "openssl/pem.h"
#include <string>
#include <fstream>

using namespace blood::crypt;

const int           RSA_OpenSSL::ms_key_length = 2048;
const int           RSA_OpenSSL::ms_max_word_length = RSA_OpenSSL::ms_key_length/8;
const int           RSA_OpenSSL::ms_padding = RSA_PKCS1_OAEP_PADDING;

const unsigned long RSA_OpenSSL::ms_pub_exp = 3;


RSA_OpenSSL::RSA_OpenSSL()
: m_rsa_keypair(nullptr)
{
    
}

RSA_OpenSSL::~RSA_OpenSSL()
{
    RSA_free(m_rsa_keypair);
    m_rsa_keypair = nullptr;
}

bool RSA_OpenSSL::initialize()
{
    try
    {
        m_rsa_keypair = RSA_generate_key(ms_key_length, ms_pub_exp, NULL, NULL);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::string RSA_OpenSSL::encryptMessage(const char* _message, const unsigned int _message_len) const
{
    if (m_rsa_keypair)
    {
        char encrypt[ms_max_word_length];

        const int encrypt_len = RSA_public_encrypt(_message_len, (unsigned char*)_message, (unsigned char*)encrypt, m_rsa_keypair, ms_padding);

        return (encrypt_len == -1)
            ? std::string()
            : std::string(encrypt, encrypt_len);
    }
    else
    {
        return std::string();
    }
}

std::string RSA_OpenSSL::decryptMessage(const char* _encrypt, const unsigned int _encrypt_len) const
{
    if (m_rsa_keypair)
    {
        char decrypt[ms_key_length];

        const int decrypt_len = RSA_private_decrypt(_encrypt_len, (unsigned char*)_encrypt, (unsigned char*)decrypt, m_rsa_keypair, ms_padding);

        return (decrypt_len == -1)
            ? std::string()
            : std::string(decrypt, decrypt_len);
    }
    else
    {
        return std::string();
    }
}

std::string RSA_OpenSSL::getPrivateKey() const
{
    if (m_rsa_keypair)
    {
        BIO* pri = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPrivateKey(pri, m_rsa_keypair, NULL, NULL, 0, NULL, NULL);

        const size_t pri_len = BIO_pending(pri);

        char* pri_key = (char*)malloc(pri_len + 1);

        BIO_read(pri, pri_key, pri_len);
        std::string result(pri_key, pri_len);

        BIO_free(pri);
        free(pri_key);
        return result;
    }
    else
    {
        return std::string();
    }
}

std::string RSA_OpenSSL::getPublicKey() const
{
    if (m_rsa_keypair)
    {
        BIO* pub = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(pub, m_rsa_keypair);

        const size_t pub_len = BIO_pending(pub);

        char* pub_key = (char*)malloc(pub_len + 1);

        BIO_read(pub, pub_key, pub_len);
        std::string result(pub_key, pub_len);

        BIO_free(pub);
        free(pub_key);
        return result;
    }
    else
    {
        return std::string();
    }
}

bool RSA_OpenSSL::savePrivateKeyToFile(const char* _file_path) const
{
    const std::string pri = getPrivateKey();
    return saveKeyToFile(_file_path, pri);
}

bool RSA_OpenSSL::savePublicKeyToFile(const char* _file_path) const
{
    const std::string pub = getPublicKey();
    return saveKeyToFile(_file_path, pub);
}

bool RSA_OpenSSL::saveKeyToFile(const char* _file_path, const std::string& _key) const
{
    if (m_rsa_keypair)
    {
        std::ofstream fout(_file_path, std::ios::out);
        if (fout.is_open())
        {
            fout << _key;
            fout.close();
            return true;
        }
    }
    return false;
}

bool RSA_OpenSSL::loadPrivateKeyFromFile(const char* _file_path)
{
    std::string key;
    loadKeyFromFile(_file_path, key);
    BIO* pri = BIO_new(BIO_s_mem());

    BIO_write(pri, key.c_str(), key.size());
    PEM_read_bio_RSAPrivateKey(pri, &m_rsa_keypair, NULL, NULL);

    BIO_free(pri);

    return true;
}

bool RSA_OpenSSL::loadPublicKeyFromFile(const char* _file_path)
{
    std::string key;
    loadKeyFromFile(_file_path, key);
    BIO* pri = BIO_new(BIO_s_mem());

    BIO_write(pri, key.c_str(), key.size());
    PEM_read_bio_RSAPrivateKey(pri, &m_rsa_keypair, NULL, NULL);

    BIO_free(pri);

    return true;
}

bool RSA_OpenSSL::loadKeyFromFile(const char* _file_path, std::string& _key) const
{
    if (m_rsa_keypair)
    {
        std::ifstream fin(_file_path, std::ios::in);
        if (fin.is_open())
        {
            _key.assign( (std::istreambuf_iterator<char>(fin))
                       , (std::istreambuf_iterator<char>()));
            fin.close();
            return true;
        }
    }
    return false;
}

#endif