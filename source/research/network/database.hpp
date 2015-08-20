#pragma once
#ifndef database_h__
#define database_h__

#include "common.hpp"
#include "script/routes.hpp"
#include "module/internet_protocol.hpp"

#include <thread>
#include <mutex>
#include <map>

class Database
{
public:
    void   initializeFromFile(const String& _file_path);
    void   initializeByFolder(const char* _folder_path);

    String findAnyAutonomousSystem(const InternetProtocol& _ip);
private:
    void   processing();

private:
    std::mutex               m_files_mutex;
    VectorOfStrings          m_files_list;

    std::mutex               m_mutex;
    Routes                   m_routes;
};
#endif // database_h__