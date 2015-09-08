#pragma once
#ifndef signals_h__
#define signals_h__

#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>
#include "common.hpp"

class Signals
{
public:
    Signals(){};
    ~Signals(){};

    Bool isEvent() const;

private:
protected:
private:
    std::condition_variable m_data_event;
};


#endif // signals_h__
