#if defined(BLOOD_32) || defined(BLOOD_VS)
#include <Windows.h>
#endif

class ConsoleWin
{
#if defined(BLOOD_32) || defined(BLOOD_VS)
public:
    ConsoleWin();
    
    void hideConsole();
    void showConsole();

    static void executeByProcess(const char* _path_to_file, const char* _arg_list);
    static void executeByShell(const char* _path_to_file, const char* _arg_list);
    static void execute(const char* _path_to_file, const char* _arg_list);

private:
    HWND m_console_handle;
#endif
};