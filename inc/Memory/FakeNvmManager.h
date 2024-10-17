#ifndef FAKE_NVM_MANAGER
#define FAKE_NVM_MANAGER

/// FakeNvmManager.h

#include "NvmManager.h"
#include "Utils.h"

class FakeNvmManager:NvmManager
{
public:

    static NvmManager* getInstance(bool reset = false);

    bool initialize() override { return true; }
    bool read(uint32_t address, size_t size, void *pBuf) override;
    bool write(uint32_t address, size_t size, void *pBuf) override;

private:
    FakeNvmManager();
    ~FakeNvmManager() = default;

    DeviceInfo *pSampleDevice_;
};

#endif