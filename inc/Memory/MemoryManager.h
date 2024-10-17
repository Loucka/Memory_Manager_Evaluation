#ifndef MEMORY_MANAGER
#define MEMORY_MANAGER

/// MemoryManager.h

#include "MemoryPool.h"
#include "EventLogger.h"
#include "BluetoothManager.h"

#include <memory>

/// @brief Provides management functionality surrounding a fixed-size memory pool relative to a connected BT device.
class MemoryManager 
{
public:

    MemoryManager(size_t poolSize);
    ~MemoryManager();

    /// @brief Allocates a provided number of blocks within the pool.
    /// @param size The number of blocks to allocate
    /// @param attemptCompact True if compaction is to be attempted following a failed allocation.
    /// @return Pointer to newly generated allocation info instance.
    AllocInfo* allocate(size_t size, bool attemptCompact = false);

    /// @brief Deallocates an existing allocation.
    /// @param pAlloc Pointer to the existing allocation info to be deallocated.
    void deallocate(AllocInfo* pAlloc);

    /// @brief Initiates compaction within the pool.
    void compact();

    /// @brief Collects statistics from the memory pool for sending to connected BT device.
    void reportStatistics() const;

    /*//#TODO: Provide functionality for interacting with BT management.
        - Async command receipt
        - Elegant management of pairing/broadcasting.
    */

private:
    
    std::unique_ptr<MemoryPool> pMemoryPool_;   ///< Pointer to managed memory pool instance.

    BluetoothManager *pBluetoothManager_;   ///< Pointer to singleton bluetooth manager instance.
    EventLogger* pEventLogger_;             ///< Pointer to singleton event logger instance.
};

#endif