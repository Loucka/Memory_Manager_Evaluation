/// Utils.cpp

#include "Utils.h"
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <iostream>

void log_info(const std::string &prefix, const std::string &message)
{
    std::cout << prefix + ": " + message << std::endl;
}

void relocate_alloc(AllocInfo *pAlloc, void *pDest)
{
    // Begin with the actual relocation.
    std::rotate((char *)pDest, (char *)pAlloc->pPoolStart, (char *)(pAlloc->pPoolStart) + pAlloc->size);

    // Next, update the allocation info to reference the new start address.
    pAlloc->pPoolStart = pDest;
}

void* get_alloc_end(AllocInfo *pAlloc)
{
    assert(nullptr != pAlloc);
    return (char *)pAlloc->pPoolStart + pAlloc->size;
}

size_t get_mem_gap(void *pStart, void *pEnd)
{
    return (char *)pEnd - (char *)pStart;
}

size_t get_alloc_frag(AllocInfo *pAlloc, void *&pFragStart)
{
    assert(nullptr != pAlloc && nullptr != pAlloc->pNext);

    return get_end_frag(pAlloc, pAlloc->pNext->pPoolStart, pFragStart);
}

size_t get_end_frag(AllocInfo *pAlloc, void *pEnd, void *&pFragStart)
{
    assert(nullptr != pAlloc);

    pFragStart = get_alloc_end(pAlloc);
    return get_mem_gap(pFragStart, pEnd);
}

std::string blocks_to_string (size_t size)
{
    return std::to_string(size) + " blocks.";
}