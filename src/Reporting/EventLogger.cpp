/// ErrorLogger.cpp

#include "EventLogger.h"

// Singleton
EventLogger *EventLogger::pInstance_ = nullptr;

EventLogger* EventLogger::getInstance(bool reset) 
{
    if (reset && nullptr != pInstance_) 
    { 
        delete pInstance_;
        pInstance_ = nullptr;
    }
    else if (!reset && nullptr == pInstance_) { pInstance_ = new EventLogger(); }

    return pInstance_;
}

EventLogger::EventLogger()
{
    // Default verbosity to just information.
    verbosity_ = VERB_INFO;
}

void EventLogger::logEvent(const Verbosity verbosity, const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex_);

    events_.push_back({ verbosity, message });
}

void EventLogger::clearLog(bool reallocate)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (reallocate) { std::vector<EventInfo>().swap(events_); }
    else            { events_.clear(); }
}

void EventLogger::printLog(bool fullLog)
{
    std::lock_guard<std::mutex> lock(mutex_);

    Verbosity verbosity = fullLog ? VERB_ERROR : verbosity_;

    for(EventInfo event : events_)
    {
        if (event.verbosity >= verbosity) { log_info("LOG", event.message); }
    }
}