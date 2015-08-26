#pragma once
#if defined(USE_OPENSSL)

#include <string>
#include "openssl/rsa.h"

namespace blood { namespace crypt {

class RSA_OpenSSL
{
public:
    RSA_OpenSSL();
    ~RSA_OpenSSL();

    bool initialize();

    std::string encryptMessage(const char* _message, const unsigned int _message_len) const;
    std::string decryptMessage(const char* _encrypt, const unsigned int _encrypt_len) const;

    std::string getPrivateKey() const;
    std::string getPublicKey() const;

    bool savePrivateKeyToFile(const char* _file_path) const;
    bool savePublicKeyToFile(const char* _file_path) const;

    bool loadPrivateKeyFromFile(const char* _file_path);
    bool loadPublicKeyFromFile(const char* _file_path);

private:
    RSA* m_rsa_keypair;

    bool saveKeyToFile(const char* _file_path, const std::string& _key) const;
    bool loadKeyFromFile(const char* _file_path, std::string& _key) const;

    static const int ms_key_length;
    static const int ms_max_word_length;
    static const int ms_padding;

    static const unsigned long ms_pub_exp;
};

}} // end blood::crypt

#endif // end USE_OPENSSL