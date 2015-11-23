#include "../../../include/blood/sys/sysinfo_nix.hxx"

#if defined(UNIX)
#include <unistd.h>
#include <string>

std::string SystemInfoNix::getCurrPath()
{
    char path[256];
    getcwd(path, sizeof(path));
    return std::string(path);
}

#endif // UNIX