#ifndef BLUETOOTH_DEVICE
#define BLUETOOTH_DEVICE

/// BluetoothManager.h

#include <string>

class BluetoothDevice {
public:
    BluetoothDevice(std::string macAddress);
    ~BluetoothDevice() = default;

    virtual bool connect() { return false; };
    virtual bool disconnect() { return false; }
    virtual bool sendData(const std::string& data) const { return false; };
    virtual const std::string receiveCommand() { return ""; };

    bool getConnected() { return isConnected_; }

protected:
    std::string macAddress_;
    bool isConnected_;
};

#endif