#include "app/application.hpp"

#include <cctype>
#include <string>
#include <algorithm>

void parseDirectory(SystemPtr _system, const WString _root);
void fillThreadContainer(std::vector<WString>& _words, SystemPtr _system, WString _root);
void printLoading(const WString& _title, size_t _loop_count, DWORD _time);
bool isFolderAllowed(const WString& _wstr);
void analysisWordData();

static const size_t ms_max_depth_of_nesting_in_directory = 3;

// words data
std::vector< std::vector < WString > > ms_global_data;
std::mutex                             ms_mutex_global_data;

// user
std::unique_ptr<SocialData>            ms_social_portrait;
std::mutex                             ms_mutex_social_portrait;
std::unique_ptr<Statistics>            ms_statistics;

static const std::vector< WString > ms_ignore_directory =
{
    { L"boost-build" },
    { L"boost-build-engine" },
    { L"inetpub" },
    { L"NVIDIA" },
    { L"PerfLogs" },
    { L"Program Files" },
    { L"ProgramData" },
    { L"movie" },
    { L"Program Files (x86)" },
    { L"Windows" },
    { L"alg_data" },
    { L"usr" },
    { L"disfa_algorithms" },
    { L"like cplusplus" },
    { L"develop" },
    { L"portfolio" },
    { L"$Recycle.Bin" }
};

void scriptAnalysisOfUserDataOnDrives()
{
    std::wcout << L"Для начала анализа нажми клавишу Enter" << std::endl;
    Console::command("pause");
    printLoading(L"Скрипт анализа пользовательских данных выполняется", 30, 100);

    Application& app = Application::getApp();
    SystemPtr system = app.getSystemPtr();
    auto drives = system->getLogicalDrives();
    ms_statistics.reset(new Statistics);

    // Getting data
    std::vector<std::thread> thread_group;
    for (auto drive : drives)
    {
        std::wcout << L"Анализ данных на диске " << drive.c_str() << L" запущен в отедельном потоке!"<< std::endl;
        thread_group.push_back(std::thread(&parseDirectory, system, drive));
    }

    for_each(thread_group.begin(), thread_group.end(),
        [](std::thread& t)
    {
        t.join();
    });

    for (auto container:ms_global_data)
    {
        for (auto word:container)
        {
            std::wcout << word.c_str() << std::endl;
        }
    }
    std::wcout.clear();
    std::wcout << std::endl << L"Потоки завершили анализ данных, нажмите Enter для продолжения" << std::endl;
    std::wcin.get();
    Console::command("cls");
    Sleep(200);
    printLoading(L"Подготовка к анализу полученных данных",5,500);

    // Analyst

    if (!thread_group.empty())
    {
        thread_group.clear();
        const size_t max_thread_count = std::thread::hardware_concurrency();
        for (size_t i = 0; i < max_thread_count; ++i)
        {
            thread_group.push_back(std::thread(&analysisWordData));
        }
    }

    for_each(thread_group.begin(), thread_group.end(),
        [](std::thread& t)
    {
        t.join();
    });


    std::wcout.clear();
    
    // DATA OUTPUT
    const wchar_t* user_name = Localization::convertStrintToWString(system->getCurrentUserName()).c_str();

    std::wcout << L"Пользователь:" << user_name << std::endl;
    std::wcout << L"Возраст:" << L"" << std::endl;
    std::wcout << L"Пол:" << L"" << std::endl;
    std::wcout << L"Социальная группа:" << L"" << std::endl;
    std::wcout << L"Пользователь:" << L"" << std::endl;
    std::wcout << L"Пользователь:" << L"" << std::endl;
    std::wcout << L"Пользователь:" << L"" << std::endl;
    std::wcout << L"Пользователь:" << L"" << std::endl;
    std::wcin.get();
}

void analysisWordData()
{
    while (!ms_global_data.empty())
    {
        ms_mutex_global_data.lock();
        std::vector< WString > data;
        if (!ms_global_data.empty())
        {
            data = ms_global_data.back();
            ms_global_data.pop_back();
        }
        ms_mutex_global_data.unlock();

        for (auto word : data)
        {
            auto range = ms_statistics->getYearByMedia(word);
            if (range.first != 0)
            {
                ms_mutex_social_portrait.lock();
                ms_social_portrait->setYear(range.first, range.second);
                ms_mutex_social_portrait.unlock();
            }
        }
    }
}

void parseDirectory(SystemPtr _system, const WString _root)
{
    Sleep(10);
    std::vector<WString> words_in_thread;
    fillThreadContainer(std::ref(words_in_thread), _system, _root);

    ms_mutex_global_data.lock();
    ms_global_data.push_back(words_in_thread);
    std::wcout << L"Поток завершил анализ локального диска!" << std::endl;
    ms_mutex_global_data.unlock();
}

void fillThreadContainer(std::vector<WString>& _words, SystemPtr _system, WString _root)
{
    if (isFolderAllowed(_root))
    {
        std::vector<Path> paths = _system->getFolderNames(_root);
        for (auto& path : paths)
        {
            _words.push_back(path.filename());
            fillThreadContainer(std::ref(_words), _system, path);
        }
    }
}

bool isFolderAllowed(const WString& _wstr)
{
    WString src = _wstr;
    std::transform(src.begin(), src.end(), src.begin(), std::ptr_fun(towlower));

    for (auto ignore:ms_ignore_directory)
    {
        std::transform( ignore.begin()
                      , ignore.end()
                      , ignore.begin()
                      , ::towlower );

        auto pos = std::wcsstr(src.c_str(), ignore.c_str());
        if (pos)
            return false;
    }

    size_t count_catalog_into = 0;
    for (auto symbol : _wstr)
    {
        if (symbol == '\\')
            count_catalog_into++;
    }

    if (count_catalog_into >= ms_max_depth_of_nesting_in_directory)
        return false;

    return true;
}

void printLoading(const WString& _title, size_t _loop_count, DWORD _time)
{
    for (size_t i = 0; i < _loop_count; i++)
    {
        const size_t count_of_dot = i % 4;
        WString dot;
        for (size_t j = 0; j < count_of_dot; j++)
            dot += L".";

        std::wcout << _title.c_str() << dot.c_str() << std::endl;
        Sleep(_time);
        system("cls");
    }
}