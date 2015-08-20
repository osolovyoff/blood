#pragma once
#include <string>
#include <array>

#include "net/mask.hxx"

namespace blood { namespace net {

typedef unsigned char byte;
typedef std::string string;
typedef std::array<byte, 4> digits;

class IP
{
    typedef unsigned int uint;
public:
    IP();
    IP(const string& _ip);
    IP(int, int, int, int);
    IP(byte, byte, byte, byte);

    bool   is_empty() const;
    uint   get_data() const;
    digits get_digits() const;
    string get_string() const;

    bool operator<(const IP& _r) const;
    bool operator>(const IP& _r) const;
    bool operator==(const IP& _r) const;
    bool operator<=(const IP& _r) const;
    bool operator>=(const IP& _r) const;

public: // tools
    static IP setMask(const IP&, Mask);
    static IP setValue(const IP&, Mask, int _value = -1);

public: // binary
    IP operator&(const IP& _r) const;
    IP operator|(const IP& _r) const;
    IP operator^(const IP& _r) const;

private: // conversation
    static string convert_value_to_string(const uint& _value);
    static digits convert_value_to_digits(const uint& _value);
    static uint   convert_string_to_value(const string& _string);
    static uint   convert_digits_to_value(const digits& _digits);

private:
    uint  m_data;
};

// tools
namespace Tool
{
    /*static IP convertBitMaskToIPMask(const byte _bit_mask);
    static IP convertIPMaskToNetwork(const string& _ip_with_mask);
    static std::pair<IP, byte> convertIPMaskToNetworkWithBitMask(const string& _ip_with_mask);
    static std::pair<IP, IP> convertIPMaskToRange(const string& _ip_with_mask);
    static bool checkForNetworkAppliances(const IP& _network, const IP& _from, const IP& _to);*/
}

}} // end blood::net