#include "../../../include/blood/net/network.hxx"
#include "../../../include/blood/common/encoding.hxx"
#include <iostream>
#include <algorithm>

namespace blood { namespace net {

Network::Network()
: m_main_station(0)
{}

Network::Network(string _network)
: m_main_station(0)
{
    m_ip = _network;

    const auto pos = _network.find('/');
    if (pos != _network.npos)
    {
        _network.erase(0, pos + 1);
        m_mask = stoi(_network);
    }
    else
    {
        m_mask = 24;
    }
    m_ip = IP::setMask(m_ip, m_mask);

    // range
    IP from = IP::setValue(m_ip, m_mask, 0);
    from = IP::setValue(from, 31, 1);

    IP to = IP::setValue(m_ip, m_mask, 1);
    to = IP::setValue(to, 31, 0);

    m_range = std::make_pair(from, to);
}

Network::Network(IP _ip, Mask _mask)
: m_main_station(0)
{
    m_mask = _mask;
    m_ip = IP::setMask(_ip, _mask);

    // range
    IP from = IP::setValue(m_ip, m_mask, 0);
    from = IP::setValue(from, 31, 1);

    IP to = IP::setValue(m_ip, m_mask, 1);
    to = IP::setValue(to, 31, 0);

    m_range = std::make_pair(from, to);
}

RangeIP Network::getRange() const
{
    return m_range;
}

string  Network::getString() const
{
    string s
        = m_ip.get_string()
        + '/'
        + m_mask.get_ip();
    return s;
}

int Network::getStation() const
{
    return m_main_station;
}

bool Network::isEmpty() const
{
    return
        (m_ip.is_empty() || m_mask.is_empty());
}

IP& Network::getIP()
{
    return m_ip;
}

Mask& Network::getMask()
{
    return m_mask;
}


RangeIP Network::convertNetworkToRangeIP(Network _network)
{
    return _network.getRange();
}

void Network::addStation(const std::vector<int>& _v)
{
    const auto min = std::min_element(_v.begin(), _v.end());
    if (min != _v.end())
        m_main_station = *min;
}

std::pair<std::string, std::vector<int>> Network::parseQuaggaLine(std::string& _line)
{
    std::pair<std::string, std::vector<int>> result;

    const bool is_network = (_line[0] == '*');
    if (is_network)
    {
        _line.erase(0, 3);

        const unsigned int pos_space = _line.find(' ', 0);
        std::string network = _line.substr(0, pos_space);
        if (!network.empty())
        {
            const unsigned int count_erase_symbols = 2;
            const unsigned int end = _line.size() - count_erase_symbols;
            _line.erase(end, count_erase_symbols);

            result.first = network;

            unsigned int begin;
            for (auto rit = _line.rbegin(); rit != _line.rend(); ++rit)
            {
                const char symbol = *rit;

                if (symbol == '0')
                {
                    const char next_symbol = *(rit + 1);

                    if (next_symbol == ' ')
                    {
                        begin = std::distance(rit, _line.rend()) + 1;
                        break;
                    }
                }
            }

            const std::string as_line = _line.substr(begin, end - begin);

            std::vector<std::string> data = blood::fn::text::split_string(as_line, ' ');
            if (!data.empty())
            {
                for (auto& elem : data)
                {
                    const int station = atoi(elem.c_str());
                    result.second.push_back(station);
                }
            }
        }
    }
    return result;
}

}} // end blood::net