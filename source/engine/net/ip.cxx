#include "../../../include/blood/net/ip.hxx"
#include "../../../include/blood/common/encoding.hxx"

#include <vector>

namespace blood { namespace net {

typedef unsigned int uint;

IP::IP()
: m_data(0)
{

}
IP::IP(const string& _ip)
: m_data(0)
{
    std::vector<std::string> v;
    v = blood::fn::text::split_string(_ip, '.');

    for (int i = 0; i < 4; ++i)
        m_data += atoi(v[i].c_str()) * (int)pow(256, 3 - i);
}
IP::IP(int, int, int, int)
: m_data(0)
{}

IP::IP(byte _ip_1, byte _ip_2, byte _ip_3, byte _ip_4)
: m_data(0)
{
    m_data
        = _ip_1 * 16777216
        + _ip_2 * 65536
        + _ip_3 * 256
        + _ip_4;
}

bool IP::is_empty() const
{
    return !m_data;
}
uint IP::get_data() const
{
    return m_data;
}
digits IP::get_digits() const
{
    return convert_value_to_digits(m_data);
}
string IP::get_string() const
{
    return convert_value_to_string(m_data);
}

bool IP::operator<(const IP& _r) const
{
    return this->m_data < _r.m_data;
}
bool IP::operator>(const IP& _r) const
{
    return this->m_data > _r.m_data;
}
bool IP::operator==(const IP& _r) const
{
    return this->m_data == _r.m_data;
}
bool IP::operator<=(const IP& _r) const
{
    return this->m_data <= _r.m_data;
}
bool IP::operator>=(const IP& _r) const
{
    return this->m_data >= _r.m_data;
}

IP IP::operator&(const IP& _r) const
{
    const IP& _l = *this;
    IP r; r.m_data = _l.m_data & _r.m_data;
    return r;
}

IP IP::operator|(const IP& _r) const
{
    const IP& _l = *this;
    IP r; r.m_data = _l.m_data | _r.m_data;
    return r;
}

IP IP::operator^(const IP& _r) const
{
    const IP& _l = *this;
    IP r; r.m_data = _l.m_data ^ _r.m_data;
    return r;
}

IP IP::setMask(const IP& _ip, Mask _mask)
{
    const IP ip_mask = _mask.get_ip();
    const IP new_ip = _ip & ip_mask;
    return new_ip;
}

IP IP::setValue(const IP& _ip, Mask _mask, int _value)
{
    IP new_ip = _ip;
    int mask = _mask.get_value();

    uint& data = new_ip.m_data;
    if (_value == 0 || _value == 1)
    {
        for (byte i = 0; i < 32-mask; ++i)
            if (_value)
                data |= (1 << i);
            else
                data &= ~(1 << i);
    }
    return new_ip;
}

string IP::convert_value_to_string(const uint& _value)
{
    char buffer[16];
    sprintf(buffer
        , "%d.%d.%d.%d"
        , _value / 16777216
        , _value / 65536 % 256
        , _value / 256 % 256
        , _value % 256);
    return buffer;
}

uint IP::convert_string_to_value(const string& _string)
{
    std::vector<std::string> v =
    blood::fn::text::split_string(_string, '.');

    uint value = 0;
    for (int i = 0; i < 4; ++i)
        value += atoi(v[i].c_str()) * (int)pow(256, 3 - i);
    return value;
}

uint IP::convert_digits_to_value(const digits& _digits)
{
    uint value
        = _digits[0] * 16777216
        + _digits[1] * 65536
        + _digits[2] * 256
        + _digits[3];
    return value;
}

digits IP::convert_value_to_digits(const uint& _value)
{
    digits array;
    array[0] = _value / 16777216;
    array[1] = _value / 65536 % 256;
    array[2] = _value / 256 % 256;
    array[3] = _value % 256;
    return array;
}

}} // end blood::net