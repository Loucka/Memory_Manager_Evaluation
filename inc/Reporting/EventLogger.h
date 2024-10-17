#ifndef EVENT_LOGGER
#define EVENT_LOGGER

/// ErrorLogger.h

#include "Utils.h"

#include <vector>
#include <mutex>

/// @brief Manages functionality surrounding the storage and display of event logs.
class EventLogger 
{
public:

    /// @brief Static function for interacting with the singleton instance.
    /// @param reset True if the instance is to be reset.
    /// @return Pointer to the singleton instance.
    static EventLogger* getInstance(bool reset = false);

    /// @brief Mutator for current verbosity setting.
    /// @param verbosity The verbosity level to be set.
    void setVerbosity(Verbosity verbosity) { verbosity_ = verbosity; }

    /// @brief Adds an individual event to the log.
    /// @param verbosity The intended verbosity level of the event.
    /// @param message Associated message to the event.
    void logEvent(const Verbosity verbosity, const std::string& message);

    /// @brief Clears the log.
    /// @param reallocate True if reallocation of memory is occur post clear.
    void clearLog(bool reallocate = false);

    /// @brief Prints the contents of the log relative with respect to desired verbosity.
    /// @param fullLog True if the full log is to be printed regardless of verbosity.
    void printLog(bool fullLog = false);

private:
    EventLogger();
    ~EventLogger() = default;

    /// @brief Structure to be used for maintaining event log messages relative to their verbosity.
    //#TODO: Promote and possibly use with NVM manager for persistence of logs.
    struct EventInfo
    {
        Verbosity verbosity;
        std::string message;
    };
    
    static EventLogger *pInstance_; ///< Pointer to singleton instance.

    Verbosity verbosity_;           ///< Current verbosity level for printing purposes.
    std::vector<EventInfo> events_; ///< List of logged events.
    std::mutex mutex_;              ///< Mutex to be used for thread safety.
};

#endif