#pragma once
#ifndef blood_net_network_hxx_
#define blood_net_network_hxx_

#include "ip.hxx"
#include "mask.hxx"
#include <vector>

namespace blood { namespace net {

typedef std::string string;
typedef std::pair<IP, IP> RangeIP;
typedef std::vector<int> Stations;

class Network
{
public:
    Network();
    Network(string);
    Network(IP, Mask);

    IP&     getIP();
    Mask&   getMask();

    RangeIP getRange() const;
    string  getString() const;
    int     getStation() const;

    bool    isEmpty() const;
    void    addStation(const std::vector<int>& _v);

public: // parse
    static std::pair<std::string, std::vector<int>> parseQuaggaLine(std::string& _line);

public:
    static RangeIP convertNetworkToRangeIP(Network);

private:
    IP       m_ip;
    Mask     m_mask;
    RangeIP  m_range;

    int      m_main_station;
};

}} // blood::net

#endif // blood_net_network_hxx_