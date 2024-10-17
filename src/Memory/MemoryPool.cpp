/// MemoryPool.cpp

#include "MemoryPool.h"
#include <cstring>
#include <stdlib.h>
#include <cstdio>
#include <algorithm>

MemoryPool::MemoryPool(size_t size) : poolSize_(size), poolBlocks_(size),
    freeBlocks_(size), pAllocHead_(nullptr) 
{
    pPoolStart_ = poolBlocks_.data();
    pPoolEnd_ = (char *)pPoolStart_ + size;
}

MemoryPool::~MemoryPool()
{
    while (nullptr != pAllocHead_) { deallocate(pAllocHead_); }

    //#TODO: Do additional deconstruction.
}

AllocInfo* MemoryPool::allocate(size_t size) 
{
    std::lock_guard<std::mutex> lock(mutex_);
    AllocInfo *pAlloc = pAllocHead_;

    // Begin by verifying sufficient room in the pool. Contiguousness will be enforced later.
    // Next, from the start of the pool, begin searching for a suitable group of blocks
    // relative to the list of allocations.
    if (freeBlocks_ < size) { pAlloc = nullptr; }
    else { find_fragment(pAlloc, size); }

    // If a match has been found, decrement the number of free blocks by the newly allocated size.
    if (nullptr != pAlloc) { freeBlocks_ -= size; }
    
    return pAlloc;
}

bool MemoryPool::deallocate(AllocInfo *&pAlloc) 
{
    if (nullptr == pAlloc || nullptr == pAlloc->pPoolStart) { return false; }

    std::lock_guard<std::mutex> lock(mutex_);

    size_t blocks = pAlloc->size;
    if (nullptr == pAlloc->pPrev && nullptr == pAlloc->pNext)
    {
        // Single entry. Head must be replaced with null.
        pAllocHead_ = nullptr;
    }
    else if (nullptr == pAlloc->pPrev && nullptr != pAlloc->pNext)
    {
        // First entry. Head must be replaced with next.
        pAllocHead_ = pAlloc->pNext;
    }
    else if (nullptr != pAlloc->pPrev && nullptr == pAlloc->pNext)
    {
        // Last entry. Head persists.
        pAlloc->pPrev = nullptr;
    }
    else
    {
        // Sandwiched entry. Head persists.
        pAlloc->pPrev->pNext = pAlloc->pNext;
        pAlloc->pNext->pPrev = pAlloc->pPrev;
    }

    // Finally, return the blocks and free the no longer needed struct.
    freeBlocks_ += blocks;
    free(pAlloc);
    pAlloc = nullptr;

    return true;
}

void MemoryPool::compact() 
{
    if (nullptr == pAllocHead_ || 0 == freeBlocks_) { return; }
    else { std::lock_guard<std::mutex> lock(mutex_); }

    AllocInfo *pAlloc = pAllocHead_;
    void *pFragStart = nullptr;
    bool fragsPresent = true;
    size_t fragSize = get_mem_gap(pPoolStart_, pAlloc->pPoolStart);

    // Check for and handle a frag at the beginning.
    if (0 != fragSize) { relocate_alloc(pAlloc, pPoolStart_); }

    // Next, check for frags b/w allocs and handle first occurrence for each loop.
    // Should the end be reached without a frag found, then set frags_present to false.
    while (fragsPresent)
    {
        if (nullptr == pAlloc->pNext)                       { fragsPresent = false; }
        else if (0 != get_alloc_frag(pAlloc, pFragStart))   { relocate_alloc(pAlloc->pNext, pFragStart); }
        else                                                { pAlloc = pAlloc->pNext; }
    }

    //#TODO: Provide one last check to free blocks.
}

PoolInfo MemoryPool::report() const 
{
    PoolInfo poolInfo;

    poolInfo.totalBlocks = poolSize_;
    poolInfo.freeBlocks  = freeBlocks_;

    //#TODO: Implement fragmentation statistics.
    return poolInfo;
}

void MemoryPool::find_fragment(AllocInfo *&pAlloc, size_t size)
{
    // Assume success. Later evaluations will indicate failure.
    bool success = true;
    void *pPool = pPoolStart_;

    // Provide allocation processing only if needed. Should no prior allocations exist,
    // then a fragment of necessary size is guaranteed to be available at the start of the pool.
    // Otherwise, begin traversing the list of allocations searching for a valid "hole" between
    // two allocations or at the beginning of the pool. Should the end of the allocations list be
    // reached without a valid fragment located, provide one last check from the end of the last
    // allocation to the end of the pool.
    if (nullptr != pAlloc)
    {
        bool frag_found = false;

        if (get_mem_gap(pPoolStart_, pAlloc->pPoolStart) >= size)
        {
            frag_found = true;
            pPool = pPoolStart_;
        }
        else
        {
            while (nullptr != pAlloc->pNext && !frag_found)
            {
                if (get_alloc_frag(pAlloc, pPool) >= size) { frag_found = true; }
                else                                       { pAlloc = pAlloc->pNext; }
            }
        }
        
        if (!frag_found && get_end_frag(pAlloc, pPoolEnd_, pPool) < size) { success = false; }
    }

    // Depending on success, add or nullify the new allocation.
    pAlloc = success ? add_alloc_info(pAlloc, pPool, size) : nullptr;
}

AllocInfo* MemoryPool::add_alloc_info(AllocInfo *pAlloc, void *pPoolStart, size_t size)
{
    // Regardless of interaction, a new struct is required, so let's handle that now.
    //#TODO: Use "new" instead of malloc.
    AllocInfo *pAllocNew = (AllocInfo *)malloc(sizeof(AllocInfo));
    pAllocNew->pNext = nullptr;
    pAllocNew->pPrev = nullptr;
    pAllocNew->size = size;
    pAllocNew->pPoolStart = pPoolStart;

    if(nullptr == pAlloc)
    {
        // Generate the first allocation entry.
        pAllocHead_ = pAllocNew;
    }
    else if (nullptr == pAlloc->pNext)
    {
        // Append allocation entry.
        pAlloc->pNext = pAllocNew;
        pAlloc->pNext->pPrev = pAlloc;
    }
    else
    {
        // Insert allocation entry.
        pAllocNew->pPrev = pAlloc;
        pAllocNew->pNext = pAlloc->pNext;

        pAlloc->pNext = pAllocNew;
        pAllocNew->pNext->pPrev = pAllocNew;
    }

    return pAllocNew;
}