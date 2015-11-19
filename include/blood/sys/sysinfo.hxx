#pragma once
#ifndef blood_sys_sysinfo_hxx_
#define blood_sys_sysinfo_hxx_

#include "../../../include/blood/sys/sysinfo_win.hxx"
#include "../../../include/blood/sys/sysinfo_nix.hxx"

class SystemInfo: public SystemInfoWin, public SystemInfoNix
{
public:
    static unsigned int getHardwareConcurrency();
    static std::string  getCurrentPath();
};

#endif//blood_sys_sysinfo_hxx_
