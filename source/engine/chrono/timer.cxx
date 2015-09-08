#include "../../../include/blood/chrono/timer.hxx"

#include <iomanip>
#include <sstream>
#include <time.h>

#include <stdio.h>
#include <string.h>
#include <time.h>

using namespace blood::chrono;

Timer::Timer()
{
    clear();
}

void Timer::clear()
{
    m_duration.zero();
}

void Timer::start()
{
    m_duration += (m_end - m_start);
    m_start = std::chrono::system_clock::now();
}

void Timer::stop()
{
    m_end = std::chrono::system_clock::now();
    m_duration += (m_end - m_start);
}

double Timer::get_duration_per_seconds()
{
    const double seconds = m_duration.count();
    return seconds;
}

tm Timer::get_struct_time_now()
{
    const auto now = std::chrono::system_clock::now();
    const std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(timeinfo));
    timeinfo = *localtime(&time_now);
    return timeinfo;
}

std::string Timer::get_string_time_now()
{
    char buffer[1024] = { 0 };
    const tm timeinfo = get_struct_time_now();
    sprintf( buffer
             , "%02d-%02d-%02d %02d:%02d:%02d"
             , timeinfo.tm_mday
             , timeinfo.tm_mon
             , timeinfo.tm_year + 1900

             , timeinfo.tm_hour
             , timeinfo.tm_min
             , timeinfo.tm_sec
             );
    std::string result = buffer;
    return result;
}
