#ifndef MEMORY_POOL
#define MEMORY_POOL

/// MemoryPool.h

#include "Utils.h"

#include <vector>
#include <mutex>

/// @brief Provides a fixed-size memory pool implementation.
class MemoryPool 
{
public:

    explicit MemoryPool(size_t size);
    ~MemoryPool();

    /// @brief Allocates a number of blocks within the pool.
    /// @param size The number of blocks to be allocated.
    /// @return Pointer to the new allocation instance.
    AllocInfo* allocate(size_t size);

    /// @brief Deallocates an existing allocation from the managed list.
    /// @param pAlloc Reference pointer to the allocation instance to be removed.
    /// @return True if operation is successful.
    bool deallocate(AllocInfo *&pAlloc);

    /// @brief Compacts the pool by removing leading and inter allocation fragments.
    void compact();

    /// @brief Generates a report surrounding the state of the memory pool
    /// @return PoolInfo instance possessing report information.
    PoolInfo report() const;

private:

    AllocInfo *pAllocHead_;        ///< Pointer to first AllocInfo instance in linked list.

    size_t freeBlocks_;            ///< Tracks number of "free" blocks in the pool.
    size_t poolSize_;              ///< Fixed size of the pool.
    std::vector<char> poolBlocks_; ///< Pool of char blocks for use.
    std::mutex mutex_;             ///< Mutex to be used for thread safety.
    void *pPoolStart_;             ///< Pointer to beginning of memory pool.
    void *pPoolEnd_;               ///< Pointer to end of memory pool.

    /// @brief Locates a suitable fragment within the memory pool.
    /// @param pAlloc Reference to allocation pointer for evaluation and modifying.
    /// @param size The requested fragment size.
    void find_fragment(AllocInfo *&pAlloc, size_t size);

    /// @brief Adds a new allocation to the managed list.
    /// @param pAlloc Pointer to the previous allocation.
    /// @param pPoolStart Pointer to new allocation's memory pool address.
    /// @param size Size of the allocation
    /// @return Pointer to the newly created allocation instance.
    AllocInfo* add_alloc_info(AllocInfo *pAlloc, void *pPoolStart, size_t size);
};

#endif