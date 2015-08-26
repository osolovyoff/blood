#pragma once
#if defined(USE_OPENSSL)

#include <string>

namespace blood { namespace crypt { 

class Hash
{
public:
    static std::string get_SHA256(std::string s, bool is_file = false);

private:
    static std::string get_SHA256_for_string(std::string string);
    static std::string get_SHA256_for_file(std::string filepath);
};

}} // end blood::crypt

#endif // end USE_OPENSSL