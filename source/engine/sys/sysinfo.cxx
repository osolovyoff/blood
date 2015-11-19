#include "../../../include/blood/sys/sysinfo.hxx"
#include <thread>

unsigned int SystemInfo::getHardwareConcurrency()
{
    return std::thread::hardware_concurrency();
}

std::string SystemInfo::getCurrentPath()
{
#if defined(WIN32) || defined(MSVS)
    return SystemInfoWin::getCurrPath();
#else
    return SystemInfoNix::getCurrPath();
#endif
}

