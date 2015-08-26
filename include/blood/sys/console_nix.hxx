

class ConsoleNix
{
#if defined(BLOOD_UNIX)
    static void execute(const wchar_t* _path_to_file, const char* _arg_list);
#endif
};