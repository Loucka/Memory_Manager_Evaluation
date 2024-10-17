#ifndef BLUETOOTH_MANAGER
#define BLUETOOTH_MANAGER

/// BluetoothManager.h

#include <string>
#include <vector>

#include "BluetoothDevice.h"
#include "NvmManager.h"
#include "Utils.h"

/// @brief Manages functionality surrounding a singular bluetooth device connection.
class BluetoothManager 
{
public:
    
    /// @brief Static function for interacting with the singleton instance.
    /// @param reset True if the instance is to be reset.
    /// @return Pointer to the singleton instance.
    static BluetoothManager* getInstance(bool reset = false);

    /// @brief Accessor for current device info.
    /// @return Pointer to the current device info instance.
    DeviceInfo* getDeviceInfo() { return pDeviceInfo_;}

    //#TODO: This is wrong, the phone does scanning while this application awaits to be paired to.
    /// @brief Communicates with the attached bluetooth peripheral to scan for potential pairing devices.
    /// @return A list of available devices for pairing.
    std::vector<DeviceInfo> scan();

    /// @brief Pairs a designated bluetooth device.
    /// @param device DeviceInfo possessing required information for establishing a valid connection.
    /// @return True if the operation is successful.
    bool pairDevice(const DeviceInfo device);

    /// @brief Unpairs a currently connected device.
    /// @return True if the operation is successful.
    bool unpairDevice();

    /// @brief Pushes data down to a connected bluetooth device.
    /// @param data String data to be sent.
    /// @return True if the operation is successful.
    bool sendData(const std::string& data) const;

    /// @brief Polls the connected bluetooth device for any received data.
    /// @return String representation of the received data.
    //#TODO: This is better handled by an async event callback.
    std::string receiveCommand();

private:
    BluetoothManager();
    ~BluetoothManager();

    static BluetoothManager *pInstance_; ///< Pointer to singleton instance.

    const uint32_t SETTINGS_ADDRESS = 0x00;             ///< Pseudo address representing location of persistent bluetooth data.
    const size_t SETTINGS_SIZE = sizeof(DeviceInfo);    ///< Size of settings object.

    DeviceInfo *pDeviceInfo_;   ///< Pointer to current device info instance.
    BluetoothDevice *pDevice_;  ///< Pointer to bluetooth device generated from device info.
    NvmManager *pNvm_;          ///< Pointer to NVM singleton instance.

    //#TODO: Explore supporting multiple devices.
    //std::map<std::string, BluetoothDevice> devices_;

    /// @brief Saves settings.
    /// @return True if operation is successful.
    bool saveSettings();
    
    /// @brief Loads settings
    /// @return True if operation is successful.
    bool loadSettings();
};

#endif