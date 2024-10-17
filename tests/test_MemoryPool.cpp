/// test_MemoryPool.cpp

#include <iostream>
#include <cassert>

#include "MemoryPool.h"

void testAllocation()
{
    size_t blocks = 1024;
    uint16_t count = 1;

    MemoryPool pool(blocks);
    AllocInfo *pAllocA = pool.allocate(1);
    AllocInfo *pAllocB = nullptr;

    // Confirm initial allocation.
    assert(nullptr != pAllocA);

    // Next, confirm proper spacing b/w allocations up to and including the final block.
    while (count < blocks)
    {
        pAllocB = pool.allocate(1);

        assert(nullptr != pAllocB);
        assert(1 == get_mem_gap(pAllocA->pPoolStart, pAllocB->pPoolStart));

        pAllocA = pAllocB;
        count++;
    }

    // Finally, confirm a failed allocation.
    assert(nullptr == pool.allocate(1));
}

void testDeallocation()
{
    MemoryPool pool(1024);

    // Capture initial allocation.
    AllocInfo *pTest = pool.allocate(10);
    void *pPoolA = pTest->pPoolStart;

    // Provide second allocation.
    pool.allocate(20);

    // Deallocate initial to produce a fragment in the beginning of pool.
    pool.deallocate(pTest);
    assert(nullptr == pTest);

    // Confirm use of the fragment for next allocation of smaller size.
    assert(pPoolA == pool.allocate(5)->pPoolStart);
}

void verifyAllocationData(char *pExpected, void *pSource, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        assert(*pExpected == *static_cast<char *>(pSource));

        pExpected++;
        pSource = static_cast<char *>(pSource) + 1;
    }
}

void testCompaction()
{
    MemoryPool pool(1024);
    char allocB[] = "CPFYKGQJDH";
    char allocD[] = "JAGOATSHCN";
    size_t allocSize = 10;
    size_t expectedSize = allocSize;

    // Provide four allocations with caching. Following allocation,
    // two fragments will be created in the pool for later compaction handling.
    AllocInfo *pAllocA = pool.allocate(allocSize);
    void *pPoolA = pAllocA->pPoolStart;
    AllocInfo *pAllocB = pool.allocate(allocSize);
    AllocInfo *pAllocC = pool.allocate(allocSize);
    AllocInfo *pAllocD = pool.allocate(allocSize);

    // Deallocate A and C to produce fragments.
    pool.deallocate(pAllocA);
    pool.deallocate(pAllocC);

    // Populate B and D with sample data.
    memcpy(pAllocB->pPoolStart, allocB, allocSize);
    memcpy(pAllocD->pPoolStart, allocD, allocSize);

    // Compact and verify location of relocated pool allocations.
    pool.compact();
    assert(pPoolA == pAllocB->pPoolStart);
    assert(allocSize == get_mem_gap(pAllocB->pPoolStart, pAllocD->pPoolStart));

    // Verify integrity of allocated data.
    verifyAllocationData(allocB, pAllocB->pPoolStart, allocSize);
    verifyAllocationData(allocD, pAllocD->pPoolStart, allocSize);
}

void verifyReport(PoolInfo poolInfo, size_t expectedBlocks, size_t expectedFree)
{
    assert(expectedBlocks == poolInfo.totalBlocks);
    assert(expectedFree == poolInfo.freeBlocks);
}

void testReporting()
{
    size_t poolSize = 1024;
    MemoryPool pool(poolSize);
    verifyReport(pool.report(), poolSize, poolSize);

    AllocInfo *pAlloc = pool.allocate(512);
    pool.allocate(512);
    verifyReport(pool.report(), poolSize, 0);
    pool.deallocate(pAlloc);
    verifyReport(pool.report(), poolSize, 512);
}

int main() 
{
    log_info("MEMPOOL", "----------------BEGINNING TESTS------------------");

    testAllocation();
    log_info("MEMPOOL", "----------------ALLOCATION PASSED------------------");

    testDeallocation();
    log_info("MEMPOOL", "----------------DEALLOCATION PASSED------------------");

    testCompaction();
    log_info("MEMPOOL", "----------------COMPACTION PASSED------------------");

    testReporting();
    log_info("MEMPOOL", "----------------REPORTING PASSED------------------");

    //#TODO: Implement more tests.
    //      - Automatic compaction following failed allocation.
    //      - Multi threaded pool use.

    log_info("MEMPOOL", "----------------END TESTS------------------");
    return 0;
}