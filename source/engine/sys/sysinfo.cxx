#include "sys/sysinfo.hxx"
#include <thread>

unsigned int SystemInfo::getHardwareConcurrency()
{
    return std::thread::hardware_concurrency();
}

std::string SystemInfo::getCurrentPath()
{
#if defined(BLOOD_VS) || defined(BLOOD_32)
    return SystemInfoWin::getCurrPath();
#else
    return SystemInfoNix::getCurrPath();
#endif
}

