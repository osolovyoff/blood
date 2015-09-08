#pragma once
#ifndef network_h__
#define network_h__

#include <map>

#include "common.hpp"
#include "module/internet_protocol.hpp"

class Routes
{
    typedef String          NetworkWithMask;
    typedef VectorOfStrings Stations;
    typedef String          StationNumber;

public:
    void addStationToNetwork(const NetworkWithMask& _nmw, const StationNumber& _number);
    void setStationByNetwork(const NetworkWithMask& _nwm, Stations& _isp);

    Stations&     getStationRef(const NetworkWithMask& _nwm);
    StationNumber getFirstStation(const NetworkWithMask& _nwm);
    StationNumber findStationNumberForNetwork(const InternetProtocol& _ip);

    UInt getNetworkCount() const;
    UInt getStationCount(const NetworkWithMask& _nmw) const;

private:
    std::map< NetworkWithMask, Stations > m_routes;
    static Stations                       ms_null_station;
};
#endif // network_h__
