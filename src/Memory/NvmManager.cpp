/// NvmManager.cpp

#include "NvmManager.h"

// Singleton
NvmManager *NvmManager::pInstance_ = nullptr;

NvmManager::NvmManager()
{
    // Construction
}

bool NvmManager::read(uint32_t address, size_t size, void *pBuf)
{
    return false;
}

bool NvmManager::write(uint32_t address, size_t size, void *pBuf)
{
    return false;
}