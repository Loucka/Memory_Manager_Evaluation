/// BluetoothManager.cpp

#include "BluetoothManager.h"
#include "FakeNvmManager.h"
#include "SimulatedBluetoothDevice.h"
#include "EventLogger.h"
#include <iostream>
#include <cassert>

// Singleton
BluetoothManager *BluetoothManager::pInstance_ = nullptr;

BluetoothManager *BluetoothManager::getInstance(bool reset)
{
    if (reset && nullptr != pInstance_) 
    { 
        delete pInstance_;
        pInstance_ = nullptr;
    }
    else if (!reset && nullptr == pInstance_) { pInstance_ = new BluetoothManager(); }

    return pInstance_;
}

BluetoothManager::BluetoothManager()
{
    pDevice_ = nullptr;
    pDeviceInfo_ = new DeviceInfo();

    //#TODO: Generate instance via factory.
    pNvm_ = FakeNvmManager::getInstance();
    assert(nullptr != pNvm_);

    // Load settings on construction and, if necessary, pair the saved device.
    if (loadSettings() && pDeviceInfo_->autoConnect) { pairDevice(*pDeviceInfo_); }
}

BluetoothManager::~BluetoothManager()
{
    if (nullptr != pDevice_) { pDevice_->disconnect(); }
    free(pDevice_);
    free(pDeviceInfo_);

    FakeNvmManager::getInstance(true);
}

std::vector<DeviceInfo> BluetoothManager::scan()
{
    //#TODO: Look to peripheral for available devices.

    // For now, just return a sample list.
    return std::vector<DeviceInfo> 
    { 
        { false, "Device_1", "2C:54:91:88:C9:E3" },
        { false, "Device_2", "1C:14:11:18:19:A3" }
    };
}

bool BluetoothManager::pairDevice(const DeviceInfo device)
{
    //#TODO: Evaluate return of disconnect to verify disconnection.
    if (nullptr != pDevice_ && pDevice_->getConnected()) { unpairDevice(); }

    //#TODO: Generate instance via factory.
    pDevice_ = (BluetoothDevice *)new SimulatedBluetoothDevice(device.macAddress);

    if (pDevice_->connect())
    {
        if (device.macAddress != pDeviceInfo_->macAddress)
        {
            pDeviceInfo_->autoConnect = true;
            pDeviceInfo_->deviceName = device.deviceName;
            pDeviceInfo_->macAddress = device.macAddress;
            saveSettings();
        }

        return true;
    }
    else { return false; }
}

bool BluetoothManager::unpairDevice()
{
    if (nullptr != pDevice_)
    {
        pDevice_->disconnect();
        pDeviceInfo_->autoConnect = false;
        saveSettings();

        delete pDevice_;
        pDevice_ = nullptr;

        return true;
    }
    else { return false; }
}

bool BluetoothManager::sendData(const std::string& data) const
{
    return nullptr != pDevice_ && pDevice_->getConnected() ? pDevice_->sendData(data) : false;
}

std::string BluetoothManager::receiveCommand() 
{
    return nullptr != pDevice_ && pDevice_->getConnected() ? pDevice_->receiveCommand() : "";
}

bool BluetoothManager::saveSettings() 
{
    bool success = pNvm_->write(SETTINGS_ADDRESS, SETTINGS_SIZE, pDeviceInfo_);
    EventLogger *pLog = EventLogger::getInstance();

    if (success) { pLog->logEvent(VERB_INFO, "Bluetooth Settings Saved Successfully!"); }
    else         { pLog->logEvent(VERB_ERROR, "Failed to Save Bluetooth Settings!"); }

    return success;
}

bool BluetoothManager::loadSettings() 
{
    bool success = pNvm_->read(SETTINGS_ADDRESS, SETTINGS_SIZE, pDeviceInfo_);
    EventLogger *pLog = EventLogger::getInstance();

    if (success) { pLog->logEvent(VERB_INFO, "Bluetooth Settings Loaded Successfully!"); }
    else         { pLog->logEvent(VERB_ERROR, "Failed to Load Bluetooth Settings!"); }

    return success;
}