#ifndef NVM_MANAGER
#define NVM_MANAGER

/// NvmManager.h

#include <string>
#include <cstdint>

class NvmManager
{
public:
    NvmManager();
    ~NvmManager() = default;

    virtual bool initialize() { return true; }
    virtual bool read(uint32_t address, size_t size, void *pBuf);
    virtual bool write(uint32_t address, size_t size, void *pBuf);

protected:
    static NvmManager *pInstance_;
};

#endif