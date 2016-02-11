#pragma once
#ifndef blood_net_mask_hxx_
#define blood_net_mask_hxx_

#include <string>
#include <map>

namespace blood { namespace net {

typedef unsigned char byte;
typedef std::string string;

class Mask
{
public:
    Mask();
    Mask(byte);
    Mask(string);

    string get_ip() const;
    byte   get_value() const;
    bool   is_empty() const;

public: // convertation
    static string convert(byte);
    static byte   convert(string);

private:
    byte m_data;

private:
    static const std::map<byte, string> bit_to_mask_table;
    static const std::map<string, byte> mask_to_bit_table;
};

}} // end blood::net

#endif // blood_net_mask_hxx_
