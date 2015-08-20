#pragma once
#include <chrono>
#include <string>
#include <ctime>

namespace blood { namespace chrono { 

class Timer
{
public:
    Timer();

    void clear();
    void start();
    void stop();
    
    double get_duration_per_seconds();

    static tm get_struct_time_now();
    static std::string get_string_time_now();

private:
    std::chrono::duration<double>         m_duration;
    std::chrono::system_clock::time_point m_start;
    std::chrono::system_clock::time_point m_end;
};

}} // end blood::chrono