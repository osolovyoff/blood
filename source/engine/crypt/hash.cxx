#if defined(USE_OPENSSL)

#include "crypt/hash.hxx"
#include <openssl/sha.h>

using namespace blood::crypt;

inline std::string convertSHA256ArrayToString(unsigned char hash[SHA256_DIGEST_LENGTH])
{
    std::string result;
    char symbol[3];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(symbol, "%02x", hash[i]);
        result += symbol;
    }
    return result;
}

std::string Hash::getSHA256(std::string s, bool is_file /*= false*/)
{
    return (is_file) 
        ? getSHA256forFile(s)
        : getSHA256forString(s);
}

std::string Hash::getSHA256forString(std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    return convertSHA256ArrayToString(hash);
}

std::string Hash::getSHA256forFile(std::string filepath)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    std::string result;

    FILE* file = fopen(filepath.c_str(), "rb");
    if (file)
    {
        const int buffer_size = 32768;
        unsigned char hash[SHA256_DIGEST_LENGTH];
        unsigned char *buffer = new unsigned char[buffer_size];

        if (buffer)
        {
            int bytesRead = 0;
            SHA256_CTX sha256;
            SHA256_Init(&sha256);

            while ((bytesRead = fread(buffer, 1, buffer_size, file)))
                SHA256_Update(&sha256, buffer, bytesRead);

            delete[] buffer;
            SHA256_Final(hash, &sha256);
            result = convertSHA256ArrayToString(hash);
        }
        fclose(file);
    }
    return result;
}

#endif // end USE_OPENSSL