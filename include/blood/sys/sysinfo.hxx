#pragma once

#include "sys/sysinfo_win.hxx"
#include "sys/sysinfo_nix.hxx"

class SystemInfo: public SystemInfoWin, public SystemInfoNix
{
public:
    static unsigned int getHardwareConcurrency();
    static std::string  getCurrentPath();
};
