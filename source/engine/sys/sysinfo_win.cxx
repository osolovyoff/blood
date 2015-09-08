#if defined(BLOOD_32) || defined(BLOOD_VS)
#include "../../../include/blood/sys/sysinfo_win.hxx"
#include <Windows.h>
#include <Userenv.h>
#include <direct.h>
#include <string>

#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Advapi32.lib")

std::string SystemInfoWin::getUserName()
{
    unsigned long size = ms_max_path;
    char* buffer = new char[size];
    GetUserNameA(buffer, &size);

    const std::string str(buffer);

    delete[] buffer;
    return str;
}

std::string SystemInfoWin::getUserPath()
{
    char szHomeDirBuf[MAX_PATH] = { 0 };

    // We need a process with query permission set
    HANDLE hToken = 0;
    OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);

    DWORD BufSize = MAX_PATH;
    GetUserProfileDirectoryA(hToken, szHomeDirBuf, &BufSize);

    // Close handle opened via OpenProcessToken
    CloseHandle(hToken);
    return std::string(szHomeDirBuf);
}

std::string SystemInfoWin::getAppPath()
{
    const std::string app_path = getUserPath() + "\\AppData";
    return app_path;
}

std::string SystemInfoWin::getLocalPath()
{
    const std::string local_path = getAppPath() + "\\Local";
    return local_path;
}

std::string SystemInfoWin::getTempPath()
{
    const std::string temp_path = getLocalPath() + "\\Temp";
    return temp_path;
}

std::string SystemInfoWin::getCurrPath()
{
    char path[256];
    _getcwd(path, sizeof(path));
    return std::string(path);
}

std::string SystemInfoWin::getMachineName()
{
    char buffer[MAX_COMPUTERNAME_LENGTH];
    DWORD size;
    GetComputerName(buffer, &size);

    std::string result(buffer);
    return result;
}

unsigned long SystemInfoWin::getHardDriveSerialNumber()
{
    const unsigned int drive_type = GetDriveType(NULL);
    wchar_t VolumeNameBuffer[100];
    wchar_t FileSystemNameBuffer[100];
    DWORD sz, fs;
    unsigned long drive_sn;
    GetVolumeInformationW(
        NULL,
        VolumeNameBuffer,
        100,
        &drive_sn,
        &sz,
        &fs,
        FileSystemNameBuffer,
        100
        );

    return drive_sn;
}

std::string SystemInfoWin::getUniqueId()
{
    const auto drive_number = getHardDriveSerialNumber();
    const auto drive_serial = std::to_string(drive_number);
    const auto user_name = getUserName();
    const auto machine_name = getMachineName();
    const auto id = machine_name + "." + user_name + "." + drive_serial;
    return id;
}

#endif
