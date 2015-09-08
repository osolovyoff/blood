#include "database.hpp"

#include <fstream>
#include <filesystem>

#include "module/parser.hpp"
#include "module/system.hpp"
#include "script/routes.hpp"
#include "app/application.hpp"

using namespace std::tr2::sys;

void Database::initializeByFolder(const char* _folder_path)
{
    directory_iterator end;
    for (directory_iterator dir(_folder_path); dir != end; dir++)
    {
        const String path = dir->path();
        m_files_list.push_back(path);
    }
    
    createThreadsAndRun(&Database::processing, this);
    joinAll(thread_group);
}

void Database::processing()
{
    m_files_mutex.lock();
    while (!m_files_list.empty())
    {
        const String file_path =
        m_files_list.back();
        m_files_list.pop_back();
        m_files_mutex.unlock();

        {
            initializeFromFile(file_path);
        }

        m_files_mutex.lock();
    }
    m_files_mutex.unlock();
}

void Database::initializeFromFile(const String& _file_path)
{
    std::ifstream file(_file_path.c_str(), std::ios::in);
    try
    {
        if (file.is_open())
        {
            std::cout << _file_path << std::endl;
            String line;
            while (!file.eof())
            {
                std::getline(file, line);
                const Bool is_network = (line[0] == '*');

                std::cout << _file_path << " " << line << std::endl;

                if (!is_network)
                    continue;
                else line.erase(0, 3);

                //std::cout << _file_path.c_str() << " " << line.c_str() << std::endl;

                const UInt pos_space = line.find(' ', 0);
                const String network = line.substr(0, pos_space);

                const UInt count_erase_symbols = 2;
                const UInt end = line.size()-count_erase_symbols;
                line.erase(end, count_erase_symbols);

                UInt begin;
                for (auto rit = line.rbegin(); rit != line.rend(); ++rit)
                {
                    const char symbol = *rit;

                    if (symbol == '0')
                    {
                        const char next_symbol = *(rit + 1);

                        if (next_symbol == ' ')
                        {
                            begin = std::distance(rit, line.rend()) + 1;
                            break;
                        }
                    }
                }

                const String as_line = line.substr(begin, end - begin);

                VectorOfStrings data;
                splitString(as_line, ' ', data);

                auto stations = m_routes.getStationRef(network);
                if (!stations.empty())
                {
                    for (auto& isp : data)
                    {
                        stations.push_back(isp);
                    }
                }
            }
        }
        file.close();
    }
    catch (...)
    {
        std::cerr << "tracer.db is corrupted" << std::endl;
        if (file.is_open())
            file.close();
    }
}

String Database::findAnyAutonomousSystem(const InternetProtocol& _ip)
{
    const auto result = m_routes.findStationNumberForNetwork(_ip);
    return result;
}

