/// test_MemoryManager.cpp

#include <cassert>

#include "MemoryManager.h"
#include "Utils.h"

void testAllocation() 
{
    MemoryManager manager(1024);
    size_t allocSize = 256;

    AllocInfo *pAlloc = manager.allocate(allocSize);
    assert(pAlloc != nullptr);
    assert(allocSize == pAlloc->size);
}

void testDeallocation()
{
    MemoryManager manager(1024);
    AllocInfo *pAlloc = manager.allocate(256);

    manager.deallocate(pAlloc);
    //#TODO: Evaluate logs for success.
}

int main() 
{
    log_info("MEMMANAGER", "----------------BEGINNING TESTS------------------");

    testAllocation();
    log_info("MEMMANAGER", "----------------ALLOCATION PASSED------------------");

    testDeallocation();
    log_info("MEMMANAGER", "----------------DEALLOCATION PASSED------------------");

    //#TODO: Test compaction?
    //#TODO: Test Bluetooth interactions.

    log_info("MEMMANAGER", "----------------END TESTS------------------");
    return 0;
}