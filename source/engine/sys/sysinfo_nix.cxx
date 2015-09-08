#if defined(BLOOD_UNIX)
#include "common.hxx"
#include <unistd.h>

std::string SystemInfoNix::getCurrPath()
{
    char path[256];
    getcwd(path, sizeof(path));
    return std::string(path);
}

#endif