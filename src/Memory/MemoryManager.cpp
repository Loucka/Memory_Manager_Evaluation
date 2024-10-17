/// MemoryManager.cpp

#include "MemoryManager.h"

MemoryManager::MemoryManager(size_t poolSize)
{ 
    pEventLogger_ = EventLogger::getInstance();
    pBluetoothManager_ = BluetoothManager::getInstance();

    pMemoryPool_ = std::make_unique<MemoryPool>(poolSize);
}

MemoryManager::~MemoryManager()
{
    pMemoryPool_.reset();

    pBluetoothManager_ = BluetoothManager::getInstance(true);
    pEventLogger_ = EventLogger::getInstance(true);
}

AllocInfo* MemoryManager::allocate(size_t size, bool attemptCompact) 
{
    std::string msg_blocks = blocks_to_string(size);
    AllocInfo *pAlloc = pMemoryPool_->allocate(size);

    if (nullptr != pAlloc) { pEventLogger_->logEvent(VERB_INFO, "Allocated " + msg_blocks); }
    else if (attemptCompact)
    {
        pMemoryPool_->compact();
        pEventLogger_->logEvent(VERB_WARNING, "Unable to allocate " + msg_blocks + " Attempting compaction.");

        return allocate(size);
    }
    else { pEventLogger_->logEvent(VERB_ERROR, "Failed to allocate " + msg_blocks); }

    return pAlloc;
}

void MemoryManager::deallocate(AllocInfo* pAllocInfo) 
{
    if (nullptr == pAllocInfo) { pEventLogger_->logEvent(VERB_ERROR, "Unable to deallocate null allocation."); }
    else
    {
        std::string msg_blocks = blocks_to_string(pAllocInfo->size);
        if (pMemoryPool_->deallocate(pAllocInfo)) { pEventLogger_->logEvent(VERB_INFO, "Deallocated " + msg_blocks); }
        else                                      { pEventLogger_->logEvent(VERB_ERROR, "Failed to deallocate " + msg_blocks); }
    }
}

void MemoryManager::compact()
{
    pMemoryPool_->compact();
}

void MemoryManager::reportStatistics() const 
{
    std::string stats = pMemoryPool_->report().toString();

    pEventLogger_->logEvent(VERB_INFO, stats);
    pBluetoothManager_->sendData(stats);
}