/// test_ErrorLogger.cpp

#include <cassert>

#include "EventLogger.h"
#include "Utils.h"

EventLogger* setup()
{
    return EventLogger::getInstance();
}

EventLogger* teardown()
{
    return EventLogger::getInstance(true);
}

void populateLog()
{
    // Log one of each verbosity type.
    setup()->logEvent(VERB_INFO, "INFO");
    setup()->logEvent(VERB_WARNING, "WARNING");
    setup()->logEvent(VERB_ERROR, "ERROR");
}

void testSingleton()
{
    // Create and destroy the singleton many times while monitoring memory usage.
    //#TODO: Evaluate in a more automated way. A strange memory leak also appears to be present.
    EventLogger *pLog = nullptr;
    for (int i = 0; i < 100000; i++)
    {
        pLog = setup();
        assert(nullptr != pLog);
        pLog = teardown();
        assert(nullptr == pLog);
    }
}

void testLogVerbosity(Verbosity verbosity)
{
    EventLogger *pLog = setup();
    pLog->setVerbosity(verbosity);

    //# TODO: Evaluate in a more automated way. For now manual verification of output is required.
    populateLog();
    pLog->printLog();

    teardown();
}

void testClear(bool reallocate)
{
    EventLogger *pLog = setup();

    //# TODO: Evaluate in a more automated way. For now manual verification of output is required.
    populateLog();
    pLog->clearLog(reallocate);
    pLog->printLog();

    teardown();
}

void testLogFull()
{
    EventLogger *pLog = setup();

    //# TODO: Evaluate in a more automated way. For now manual verification of output is required.
    populateLog();
    pLog->printLog(true);

    teardown();
}

int main()
{
    log_info("LOGGER", "----------------BEGINNING TESTS------------------");

    testSingleton();
    log_info("LOGGER", "----------------SINGLETON PASSED------------------");

    testLogVerbosity(VERB_INFO);
    log_info("LOGGER", "----------------LOG INFO PASSED------------------");

    testLogVerbosity(VERB_WARNING);
    log_info("LOGGER", "----------------LOG WARNING PASSED------------------");

    testLogVerbosity(VERB_ERROR);
    log_info("LOGGER", "----------------LOG ERROR PASSED------------------");

    testClear(false);
    log_info("LOGGER", "----------------LOG CLEAR PASSED------------------");

    testClear(true);
    log_info("LOGGER", "----------------LOG CLEAR REALLOC PASSED------------------");

    testLogFull();
    log_info("LOGGER", "----------------FULL LOG PASSED------------------");

    log_info("LOGGER", "----------------END TESTS------------------");
    return 0;
}