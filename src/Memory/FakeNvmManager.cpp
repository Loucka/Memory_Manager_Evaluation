/// FakeNvmManager.cpp

#include "FakeNvmManager.h"

NvmManager* FakeNvmManager::getInstance(bool reset) 
{
    if (reset && nullptr != pInstance_)
    {
        delete pInstance_;
        pInstance_ = nullptr;
    }
    else if (!reset &&  nullptr == pInstance_) { pInstance_ = new FakeNvmManager(); }

    return pInstance_;
}

FakeNvmManager::FakeNvmManager()
{
    // Hardcode sample device information.
    pSampleDevice_ = new DeviceInfo();

    pSampleDevice_->autoConnect = true;
    pSampleDevice_->deviceName = "Test_Device";
    pSampleDevice_->macAddress = "01:23:45:67:89:AB";
}

bool FakeNvmManager::read(uint32_t address, size_t size, void *pBuf)
{
    memcpy(pBuf, pSampleDevice_, size);
    return true;
}

bool FakeNvmManager::write(uint32_t address, size_t size, void *pBuf)
{
    //# Pretend to save.
    return true;
}