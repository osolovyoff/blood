#include "routes.hpp"

Routes::Stations Routes::ms_null_station;

void Routes::addStationToNetwork(const NetworkWithMask& _nmw, const StationNumber& _number)
{

}

void Routes::setStationByNetwork(const NetworkWithMask& _nwm, Stations& _isp)
{
    m_routes[_nwm] = _isp;
}

Routes::Stations& Routes::getStationRef(const NetworkWithMask& _nwm)
{
    auto i = m_routes.find(_nwm);
    return ( i != m_routes.end() )
        ? i->second
        : ms_null_station;
}

Routes::StationNumber Routes::getFirstStation(const NetworkWithMask& _nwm)
{
    StationNumber sn;
    auto i = m_routes.find(_nwm);

    if (i != m_routes.end())
        sn = i->second.front();

    return sn;
}

UInt Routes::getNetworkCount() const
{
    return m_routes.size();
}

UInt Routes::getStationCount(const NetworkWithMask& _nmw) const
{
    auto i = m_routes.find(_nmw);

    return (i != m_routes.end())
        ? i->second.size()
        : 0;
}

Routes::StationNumber Routes::findStationNumberForNetwork(const InternetProtocol& _ip)
{
    String isp;

    for (auto& i:m_routes)
    {
        NetworkWithMask nwm = i.first;
        Stations& stations = i.second;

        if (nwm == "0.0.0.0")
            continue;

        auto range = InternetProtocol::convertIPMaskToRange(nwm);
        auto& from = range.first;
        auto& to = range.second;

        const Bool is_finded = InternetProtocol::checkForNetworkAppliances(_ip, from, to);
        if (is_finded)
        {
            isp = getFirstStation(nwm);
            break;
        }
    }
    return isp;
}