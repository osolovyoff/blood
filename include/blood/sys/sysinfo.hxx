#pragma once

#include "../../../include/blood/sys/sysinfo_win.hxx"
#include "../../../include/blood/sys/sysinfo_nix.hxx"

class SystemInfo: public SystemInfoWin, public SystemInfoNix
{
public:
    static unsigned int getHardwareConcurrency();
    static std::string  getCurrentPath();
};
