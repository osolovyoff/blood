#include <string>

class SystemInfoWin
{
#if defined(BLOOD_32) || defined(BLOOD_VS) 
public:
    static std::string getUserName();
    static std::string getUserPath();
    static std::string getAppPath();
    static std::string getLocalPath();
    static std::string getTempPath();
    static std::string getCurrPath();

    static unsigned long getHardDriveSerialNumber();

    static std::string getUniqueId();
    static std::string getMachineName();

private:
    static const unsigned int ms_max_path = 255;
#endif
};
