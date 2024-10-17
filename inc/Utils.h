#ifndef UTILS
#define UTILS

/// Utils.h

#include <cstddef>
#include <string>

const std::string COMMAND_PREFIX = "$";
const std::string COMMAND_ALLOCATE = COMMAND_PREFIX + "ALLOC";
const std::string COMMAND_DEALLOCATE = COMMAND_PREFIX + "DEALLOC";
const std::string COMMAND_COMPACT = COMMAND_PREFIX + "COMPACT";
const std::string COMMAND_REPORT = COMMAND_PREFIX + "REPORT";

/// @brief Enum indicating the varying levels of verbosity permitted for event logging.
/// All levels up to and including the set value will be printed.
enum Verbosity
{
    VERB_INFO,
    VERB_WARNING,
    VERB_ERROR
};

/// @brief Structure to be used for tracking individual allocations relative to their neighbor(s)
struct AllocInfo
{
    void *pPoolStart; ///< Pointer to address of allocation start.
    size_t size;      ///< Size of the allocation.

    AllocInfo *pPrev; ///< Pointer to previous allocation struct, if present.
    AllocInfo *pNext; ///< Pointer to next allocation struct, if present.
};

/// @brief Struture to be used for providing general diagnostics surrounding a given memory pool.
struct PoolInfo
{
    size_t totalBlocks; ///< Total number of blocks in the pool. 
    size_t freeBlocks;  ///< Number of free blocks in the pool.

    size_t fragCount;   ///< Number of fragmentations present in the pool.
    size_t largestFrag; ///< Size of the largest fragmentation in the pool.
    size_t smallestFrag;///< Size of the smallest fragmentation in the pool.

    std::string toString()
    {
        //#TODO: Expand toString()
        return "totalBlocks: " + std::to_string(totalBlocks) + ", freeBlocks: " + std::to_string(freeBlocks);
    }
};

/// @brief Structure to be used for representation of a stored BLE device's information.
struct DeviceInfo
{
    bool autoConnect;       ///< Whether or not an auto connection is to be attempted.
    std::string deviceName; ///< Name of the BLE device.
    std::string macAddress; ///< MAC address of the BLE device.
};

/// @brief Prints out a message consisting of a prefix followed by a detailed message.
/// @param prefix The prefix to be used for printing.
/// @param message The message body to be used for printing.
void log_info(const std::string &prefix, const std::string &message);

/// @brief Relocates an allocation to a provided address.
/// @param pAlloc Pointer to the allocation info struct to be relocated
/// @param pDest Pointer to the allocation's new pool start address.
void relocate_alloc(AllocInfo *pAlloc, void *pDest);

/// @brief Evaluates a given allocation struct to determine the address of the final block.
/// @param pAlloc Pointer to the allocation info struct instance for evaluation.
/// @return Pointer to the address of the allocation's final block.
void* get_alloc_end(AllocInfo *pAlloc);

/// @brief Determines the size between two provided addresses.
/// @param pStart Pointer to the start address in memory to be evaluated. 
/// @param pEnd Pointer to the end address in memory to be evaluated.
/// @return Number of bytes between the provided addresses.
size_t get_mem_gap(void *pStart, void *pEnd);

/// @brief Determines the size and start address of memory fragmentation
///        between an existing allocation and its forward neighbor.
/// @param pAlloc Pointer to the allocation info struct instance for evaluation.
/// @param pFragStart Reference to pointer representing the start of the potential memory fragment.
/// @return Size of "free" memory between provided allocation and its forward neighbor.
size_t get_alloc_frag(AllocInfo *pAlloc, void *&pFragStart);

/// @brief Determines the size and start address of memory fragmentation
///        between an existing allocation and a given end address.
/// @param pAlloc Pointer to the allocation info struct instance for evaluation.
/// @param pEnd Pointer to address representing the end of the memory pool to be used.
/// @param pFragStart Reference to pointer representing the start of the memory fragment.
/// @return Size of "free" memory between provided allocation and end address.
size_t get_end_frag(AllocInfo *pAlloc, void *pEnd, void *&pFragStart);

std::string blocks_to_string(size_t blocks);

#endif